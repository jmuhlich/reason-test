open Models;
open Belt;

module Decode = {
  let user = json =>
    Json.Decode.{
      id: json |> field("id", int),
      username: json |> field("username", string),
      roles: json |> field("roles", array(string)),
      full_name: json |> field("full_name", string),
      email: json |> field("email", string),
    };
  let session = json => Json.Decode.{user: json |> field("user", user)};
  let sessionResponse = json =>
    Json.Decode.{session: json |> field("session", session)};
  let canonical = json =>
    Json.Decode.{
      id: json |> field("id", int),
      type_: json |> field("type", string),
      name: json |> field("name", string),
      alternate_names:
        json |> optional(field("alternate_names", array(string))),
    };
  let canonicalResponse = json =>
    Json.Decode.{canonicals: json |> field("canonicals", array(canonical))};
};

module Encode = {
  let loginRequest = r =>
    Json.Encode.(
      object_([
        ("username", string(r.username)),
        ("password", string(r.password)),
      ])
    );
};

type apiResult('a) = Js.Promise.t(Belt.Result.t('a, string));

let login = (username: string, password: string): apiResult(session) => {
  let payload: loginRequest = {username, password};
  let body =
    Fetch.BodyInit.make(payload |> Encode.loginRequest |> Json.stringify);
  let headers = Fetch.HeadersInit.make({"Content-Type": "application/json"});
  let request = Fetch.RequestInit.make(~method_=Post, ~body, ~headers, ());
  Js.Promise.(
    Fetch.fetchWithInit("/api/v0/login", request)
    |> then_(Fetch.Response.json)
    |> then_(json => {
         let response = Decode.sessionResponse(json);
         let session = response.session;
         resolve(Belt.Result.Ok(session));
       })
    |> catch(_err =>
         resolve(Belt.Result.Error({j|API error (username=$username)|j}))
       )
  );
};

let getCanonical = (id: int): apiResult(canonical) => {
  let headers = Fetch.HeadersInit.make({"Content-Type": "application/json"});
  let request = Fetch.RequestInit.make(~method_=Get, ~headers, ());
  Js.Promise.(
    Fetch.fetchWithInit("/api/v0/canonical/" ++ string_of_int(id), request)
    |> then_(Fetch.Response.json)
    |> then_(json => {
         let response = Decode.canonicalResponse(json);
         let canonical = Array.getExn(response.canonicals, 0);
         resolve(Belt.Result.Ok(canonical));
       })
    |> catch(_err =>
         resolve(Belt.Result.Error({j|API error (canonical id=$id)|j}))
       )
  );
};
