/* This is the basic component. */
[@react.component]
let make = (~message) =>
  <div onClick={_ => Js.log("clicked!")}> {React.string(message)} </div>;
