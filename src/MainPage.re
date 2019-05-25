[@react.component]
let make = () =>
  <>
    <h1> {React.string("Hello world!")} </h1>
    <Link href="/user/7"> {React.string("View user 7")} </Link>
  </>;
