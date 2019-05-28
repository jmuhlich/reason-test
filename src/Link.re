let onClick = (path, event) =>
  ReactEvent.Mouse.(
    if (button(event) == 0
        && !(
             ctrlKey(event)
             || shiftKey(event)
             || altKey(event)
             || metaKey(event)
           )) {
      ReasonReactRouter.push(path);
      preventDefault(event);
    }
  );

[@react.component]
let make = (~href, ~children) =>
  <a href onClick={onClick(href)}> children </a>;
