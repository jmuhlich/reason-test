[@react.component]
let make = () => {
  let url = ReasonReactRouter.useUrl();
  switch (url.path) {
  | ["user", id] => <UserPage id />
  | [] => <MainPage />
  | _ => <NotFoundPage />
  };
};
