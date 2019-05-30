[@react.component]
let make = (~canonical: Models.canonical) =>
  <div>
    <p>
      (
        React.string(
          canonical.name
          ++ " (id="
          ++ string_of_int(canonical.id)
          ++ ", type="
          ++ canonical.type_
          ++ ")",
        )
      )
    </p>
  </div>;
