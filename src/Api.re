open Models;

module Decode = {
  let user = json =>
    Json.Decode.{
      id: json |> field("id", int),
      first_name: json |> field("first_name", string),
      last_name: json |> field("last_name", string),
      avatar: json |> field("avatar", string),
    };
  let userResponse = json => Json.Decode.{data: json |> field("data", user)};
  let usersResponse = json =>
    Json.Decode.{
      page: json |> field("page", int),
      per_page: json |> field("per_page", int),
      total: json |> field("total", int),
      total_pages: json |> field("total_pages", int),
      data: json |> field("data", array(user)),
    };
};

type apiResult('a) = Js.Promise.t(Belt.Result.t('a, string));

let getUser = (id: int): apiResult(user) =>
  Js.Promise.(
    Fetch.fetch("https://reqres.in/api/users/" ++ string_of_int(id))
    |> then_(Fetch.Response.json)
    |> then_(json => {
         let response = Decode.userResponse(json);
         let user = response.data;
         resolve(Belt.Result.Ok(user));
       })
    |> catch(_err => resolve(Belt.Result.Error({j|API error (user=$id)|j})))
  );
