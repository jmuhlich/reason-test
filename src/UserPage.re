type state = {
  user: option(Models.user),
  error: option(string),
};

type action =
  | ApiResponse(Models.session)
  | ApiError(string);

[@react.component]
let make = (~id: string) => {
  let id = int_of_string(id);

  let (state, dispatch) =
    React.useReducer(
      (_state, action) =>
        switch (action) {
        | ApiResponse(session) => {user: Some(session.user), error: None}
        | ApiError(message) => {user: None, error: Some(message)}
        },
      {user: None, error: None},
    );

  React.useEffect1(
    () => {
      Api.login("jlm26", "hunter2")
      |> Js.Promise.then_(result => {
           switch (result) {
           | Belt.Result.Ok(user) => dispatch(ApiResponse(user))
           | Belt.Result.Error(message) => dispatch(ApiError(message))
           };
           Js.Promise.resolve();
         })
      |> ignore;
      None;
    },
    [|id|],
  );

  let prev_id = id - 1;
  let next_id = id + 1;
  <>
    {
      switch (state.user) {
      | Some(user) => <User user />
      | None => ReasonReact.null
      }
    }
    {
      switch (state.error) {
      | Some(error) =>
        <p
          style={ReactDOMRe.Style.make(~color="red", ~fontStyle="italic", ())}>
          {React.string("There was a problem retrieving the data: " ++ error)}
        </p>
      | None => ReasonReact.null
      }
    }
    {
      prev_id > 0 ?
        <Link href={j|/user/$prev_id|j}>
          {React.string({j|< User $prev_id|j})}
        </Link> :
        ReasonReact.null
    }
    <span
      style={ReactDOMRe.Style.make(~display="inline-block", ~width="1em", ())}
    />
    <Link href={j|/user/$next_id|j}>
      {React.string({j|User $next_id >|j})}
    </Link>
  </>;
};
