#include "GitRepository.hpp"

using namespace RiverDreams;

std::string GitRepository::SearchForRootDirectoryPath(std::string searchDirectoryPath)
{
    DIR* directoryStream = opendir(searchDirectoryPath.c_str());
    if (!directoryStream)
    {
        return "";
    }
    bool isRootRepository = false;
    for (struct dirent* directoryEntry; (directoryEntry = readdir(directoryStream));)
    {
        if (!std::strcmp(directoryEntry->d_name, ".git"))
        {
            isRootRepository = true;
            break;
        }
    }
    closedir(directoryStream);
    std::string parentDirectoryPath = Path::GetParentDirectory(searchDirectoryPath);
    return isRootRepository ? searchDirectoryPath : this->SearchForRootDirectoryPath(parentDirectoryPath);
}

GitRepository::GitRepository()
{
    std::string pwd         = EnvironmentVariables::GetPWD();
    this->rootDirectoryPath = this->SearchForRootDirectoryPath(pwd);
}

std::string GitRepository::GetRootDirectoryPath()
{
    return this->rootDirectoryPath;
}

std::string GitRepository::GetBranch()
{
    size_t      quantityOfCharactersToIgnore = 16;
    size_t      maximumBranchLength          = 255;
    std::string headFilePath                 = this->rootDirectoryPath + "/.git/HEAD";
    std::string readMode                     = "r";
    std::FILE*  headFileStream               = std::fopen(headFilePath.c_str(), readMode.c_str());
    if (!headFileStream)
    {
        return "";
    }
    size_t      characterIndex = 0;
    std::string branch;
    for (char character; (character = std::fgetc(headFileStream)) != EOF && character != '\n' &&
                         characterIndex < quantityOfCharactersToIgnore + maximumBranchLength;
         characterIndex++)
    {
        if (characterIndex < quantityOfCharactersToIgnore)
        {
            continue;
        }
        branch += character;
    }
    std::fclose(headFileStream);
    return branch;
}
