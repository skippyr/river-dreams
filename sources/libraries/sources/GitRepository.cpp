#include "GitRepository.hpp"

using namespace RiverDreams::Environment;
using namespace RiverDreams::FileSystem::VersionControl;

std::string GitRepository::SearchForRootDirectoryPath(std::string searchPath)
{
    if (searchPath == "/")
    {
        return "";
    }
    return Directory(searchPath).IsRepositoryRoot() ? searchPath
                                                    : SearchForRootDirectoryPath(Path::GetParent(searchPath));
}

GitRepository::GitRepository()
{
    rootDirectoryPath = SearchForRootDirectoryPath(EnvironmentVariables::GetPWD());
}

std::string GitRepository::GetRootDirectoryPath()
{
    return rootDirectoryPath;
}

std::string GitRepository::GetBranch()
{
    unsigned long maximumBranchLength    = 255;
    unsigned long totalOfSlashesToIgnore = 2;
    File          headFile               = File(rootDirectoryPath + "/.git/HEAD");
    std::string   branch;
    for (char character;
         (character = headFile.GetCharacter()) != EOF && character != '\n' && branch.length() <= maximumBranchLength;)
    {
        if (!totalOfSlashesToIgnore)
        {
            branch += character;
        }
        else if (character == '/')
        {
            totalOfSlashesToIgnore -= 1;
        }
    }
    return branch;
}
