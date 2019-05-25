let onClick = (path, event) =>
  if (ReactEvent.Mouse.button(event) == 0) {
    ReasonReactRouter.push(path);
    ReactEvent.Mouse.preventDefault(event);
  };

[@react.component]
let make = (~href, ~children) =>
  <a href onClick={onClick(href)}> children </a>;
