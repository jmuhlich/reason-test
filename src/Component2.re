[@react.component]
let make = (~greeting) => {
  let (count, setCount) = React.useState(() => 0);
  let (show, setShow) = React.useState(() => true);

  <div>
    <button onClick={_ => setCount(count => count + 1)}>
      {
        React.string(
          "You've clicked this " ++ string_of_int(count) ++ " times(s)",
        )
      }
    </button>
    <button onClick={_ => setShow(show => !show)}>
      {React.string("Toggle greeting")}
    </button>
    {show ? React.string(greeting) : React.null}
  </div>;
};
