[@react.component]
let make = (~user: Models.user) =>
  <div>
    <p>
      <img src={user.avatar} />
      <br />
      {React.string(user.first_name ++ " " ++ user.last_name)}
    </p>
  </div>;
