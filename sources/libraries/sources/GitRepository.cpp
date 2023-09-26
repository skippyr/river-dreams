#include "GitRepository.hpp"

using namespace RiverDreams::Environment;
using namespace RiverDreams::FileSystem::VersionControl;

std::string GitRepository::SearchForRootDirectoryPath(std::string searchDirectoryPath)
{
    if (searchDirectoryPath == "/")
    {
        return "";
    }
    return Directory(searchDirectoryPath).IsRepositoryRoot()
               ? searchDirectoryPath
               : SearchForRootDirectoryPath(Path::GetParent(searchDirectoryPath));
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
    File          headFile               = File(Path(rootDirectoryPath).Join(".git").Join("HEAD")
                                                                                    .ToString());
    std::string   branch;
    for (char character; (character = headFile.GetCharacter()) != EOF && character != '\n' &&
                         branch.length() <= maximumBranchLength;)
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
