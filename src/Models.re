/*
 type user = {
   id: int,
   username: string,
   roles: array(string),
   full_name: string,
   email: string,
 }
 and session = {user}
 and sessionResponse = {session};
 */

type user = {
  id: int,
  first_name: string,
  last_name: string,
  avatar: string,
}
and usersResponse = {
  page: int,
  per_page: int,
  total: int,
  total_pages: int,
  data: array(user),
}
and userResponse = {data: user};
