[@react.component]
let make = () => {
  let url = ReasonReactRouter.useUrl();
  switch (url.path) {
  | ["canonical", id] => <CanonicalPage id />
  | [] => <MainPage />
  | _ => <NotFoundPage />
  };
};
