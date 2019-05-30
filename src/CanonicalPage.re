type state = {
  canonical: option(Models.canonical),
  error: option(string),
};

type action =
  | ApiResponse(Models.canonical)
  | ApiError(string);

[@react.component]
let make = (~id: string) => {
  let id = int_of_string(id);

  let (state, dispatch) =
    React.useReducer(
      (_state, action) =>
        switch (action) {
        | ApiResponse(canonical) => {
            canonical: Some(canonical),
            error: None,
          }
        | ApiError(message) => {canonical: None, error: Some(message)}
        },
      {canonical: None, error: None},
    );

  React.useEffect1(
    () => {
      Api.getCanonical(id)
      |> Js.Promise.then_(result => {
           switch (result) {
           | Belt.Result.Ok(canonical) => dispatch(ApiResponse(canonical))
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
    (
      switch (state.canonical) {
      | Some(canonical) => <Canonical canonical />
      | None => ReasonReact.null
      }
    )
    (
      switch (state.error) {
      | Some(error) =>
        <p
          style=(ReactDOMRe.Style.make(~color="red", ~fontStyle="italic", ()))>
          (React.string("There was a problem retrieving the data: " ++ error))
        </p>
      | None => ReasonReact.null
      }
    )
    (
      prev_id > 0 ?
        <Link href={j|/canonical/$prev_id|j}>
          (React.string({j|< Canonical $prev_id|j}))
        </Link> :
        ReasonReact.null
    )
    <span
      style=(ReactDOMRe.Style.make(~display="inline-block", ~width="1em", ()))
    />
    <Link href={j|/canonical/$next_id|j}>
      (React.string({j|Canonical $next_id >|j}))
    </Link>
  </>;
};
