#include "User.hpp"

using namespace RiverDreams::FileSystem;
using namespace RiverDreams::Users;

bool User::IsRoot()
{ return !getuid(); }

bool User::OwnsPWD()
{
    unsigned userId = getuid();
    return !(userId && userId != Directory(".").GetUserId());
}
