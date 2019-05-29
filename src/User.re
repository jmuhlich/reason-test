[@react.component]
let make = (~user: Models.user) =>
  <div>
    <p> {React.string(user.full_name ++ " (" ++ user.username ++ ")")} </p>
  </div>;
