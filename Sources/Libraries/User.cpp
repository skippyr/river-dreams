#include "User.hpp"

using namespace RiverDreams::FileSystem;
using namespace RiverDreams::Users;

bool User::IsRoot()
{ return (!getuid()); }

bool User::OwnsPWD()
{
    unsigned userIdentifier = getuid();
    return (!userIdentifier || userIdentifier == Directory(".").GetUserIdentifier());
}
