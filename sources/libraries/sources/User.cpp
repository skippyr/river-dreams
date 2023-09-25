#include "User.hpp"

using namespace RiverDreams::FileSystem;
using namespace RiverDreams::Users;

bool User::IsRootUser()
{
    return !getuid();
}

bool User::OwnsCurrentDirectory()
{
    unsigned int userId = getuid();
    return !(userId && userId != Directory(".").GetUserId());
}
