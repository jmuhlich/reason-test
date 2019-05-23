open Belt;

type user = {
  id: int,
  first_name: string,
  last_name: string,
  avatar: string,
}
and usersResponse = {
  page: int,
  per_page: int,
  total: int,
  total_pages: int,
  data: array(user),
}
and userResponse = {data: user};

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

type apiResult('a) = Js.Promise.t(Result.t('a, string));

module ApiClient = {
  let getUser = (id: int): apiResult(user) =>
    Js.Promise.(
      Fetch.fetch("https://reqres.in/api/users/" ++ string_of_int(id))
      |> then_(Fetch.Response.json)
      |> then_(json => {
           let response = Decode.userResponse(json);
           let user = response.data;
           resolve(Result.Ok(user));
         })
      |> catch(_err => resolve(Result.Error({j|API error (user=$id)|j})))
    );
};

type state = {
  user: option(user),
  error: option(string),
};

type action =
  | ApiResponse(user)
  | ApiError(string);

let handleClick = dispatch =>
  ApiClient.getUser(Random.int(25) + 1)
  |> Js.Promise.then_(result => {
       switch (result) {
       | Result.Ok(user) => dispatch(ApiResponse(user))
       | Result.Error(message) => dispatch(ApiError(message))
       };
       Js.Promise.resolve();
     })
  |> ignore;

[@react.component]
let make = () => {
  let (state, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | ApiResponse(newUser) => {user: Some(newUser), error: None}
        | ApiError(message) => {...state, error: Some(message)}
        },
      {user: None, error: None},
    );

  <div onClick={_ => handleClick(dispatch)}>
    {
      React.string(
        "Click this text to call the API (requests for user id > 12 are \
         expected to fail)",
      )
    }
    {
      switch (state.user) {
      | Some(user) =>
        <p>
          <img src={user.avatar} />
          <br />
          {React.string(user.first_name ++ " " ++ user.last_name)}
        </p>
      | None => ReasonReact.null
      }
    }
    {
      switch (state.error) {
      | Some(error) =>
        <p
          style={ReactDOMRe.Style.make(~color="red", ~fontStyle="italic", ())}>
          {React.string("There was a problem retrieving the data: ")}
          {React.string(error)}
        </p>
      | None => ReasonReact.null
      }
    }
  </div>;
};
