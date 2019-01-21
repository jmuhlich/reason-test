[%%debugger.chrome];

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

type state = {user: option(user)};

type action =
  | ApiResponse(user);

/* This is the basic component. */
let component = ReasonReact.reducerComponent("Component1");

/* Your familiar handleClick from ReactJS. This mandatorily takes the payload,
   then the `self` record, which contains state (none here), `handle`, `reduce`
   and other utilities */
let handleClick = (_event, self) => {
  Js.log("clicked!");
  let _ =
    Js.Promise.(
      Fetch.fetch("https://reqres.in/api/users/2")
      |> then_(Fetch.Response.json)
      |> then_(json => {
           let response = Decode.userResponse(json);
           let user = response.data;
           self.ReasonReact.send(ApiResponse(user));
           resolve();
         })
      |> catch(err => Js.log2("API error: ", err) |> resolve)
    );
  ();
};

let str = ReasonReact.string;

/* `make` is the function that mandatorily takes `children` (if you want to use
   `JSX). `message` is a named argument, which simulates ReactJS props. Usage:

   `<Component1 message="hello" />`

   Which desugars to

   `ReasonReact.element(Component1.make(~message="hello", [||]))` */
let make = (~message, _children) => {
  ...component,
  initialState: () => {user: None},
  reducer: (action, _state) =>
    switch (action) {
    | ApiResponse(newUser) => ReasonReact.Update({user: Some(newUser)})
    },
  render: self =>
    <div onClick=(self.handle(handleClick))>
      (str(message))
      (
        switch (self.state.user) {
        | Some(user) =>
          <p>
            <img src=user.avatar />
            <br />
            (str(user.first_name ++ " " ++ user.last_name))
          </p>
        | None => <p />
        }
      )
    </div>,
};
