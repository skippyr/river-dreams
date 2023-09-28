#include "GitRepository.hpp"

using namespace RiverDreams::FileSystem::VersionControl;

std::string GitRepository::SearchForRootDirectory(std::string searchDirectory)
{
    if (Path::IsRoot(searchDirectory))
    { return (""); }
    return (Directory(searchDirectory).IsRepositoryRoot()
                ? searchDirectory
                : SearchForRootDirectory(Path::GetParent(searchDirectory)));
}

GitRepository::GitRepository()
{ rootDirectory = SearchForRootDirectory(); }

std::string GitRepository::GetRootDirectory()
{ return (rootDirectory); }

std::string GitRepository::GetBranch()
{
    unsigned long maximumBranchLength    = 255;
    unsigned long totalOfSlashesToIgnore = 2;
    File          headFile               = File(Path(rootDirectory).Join(".git").Join("HEAD")
                                                                                .ToString());
    std::string   branch;
    for (char character; (character = headFile.GetCharacter()) != EOF && character != '\n' &&
                         branch.length() <= maximumBranchLength;)
    {
        if (!totalOfSlashesToIgnore)
        { branch += character; }
        else if (character == '/')
        { totalOfSlashesToIgnore -= 1; }
    }
    return (branch);
}
