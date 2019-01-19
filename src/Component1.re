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

/* This is the basic component. */
let component = ReasonReact.statelessComponent("Component1");

/* Your familiar handleClick from ReactJS. This mandatorily takes the payload,
   then the `self` record, which contains state (none here), `handle`, `reduce`
   and other utilities */
let handleClick = (_event, _self) => {
  Js.log("clicked!");
  let _ =
    Js.Promise.(
      Fetch.fetch("https://reqres.in/api/users")
      |> then_(Fetch.Response.json)
      |> then_(json => Decode.usersResponse(json) |> Js.log |> resolve)
      |> catch(err => Js.log2("API error: ", err) |> resolve)
    );
  ();
};

/* `make` is the function that mandatorily takes `children` (if you want to use
   `JSX). `message` is a named argument, which simulates ReactJS props. Usage:

   `<Component1 message="hello" />`

   Which desugars to

   `ReasonReact.element(Component1.make(~message="hello", [||]))` */
let make = (~message, _children) => {
  ...component,
  render: self =>
    <div onClick=(self.handle(handleClick))>
      (ReasonReact.string(message))
    </div>,
};

let a: int = 123;
