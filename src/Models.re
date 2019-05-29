type user = {
  id: int,
  username: string,
  roles: array(string),
  full_name: string,
  email: string,
};

type session = {user};

type loginRequest = {
  username: string,
  password: string,
};

type sessionResponse = {session};
