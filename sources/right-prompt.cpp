#include <dirent.h>
#include <sys/stat.h>

#include <cstring>
#include <iostream>

struct DirectoryEntriesStatus {
    size_t totalOfHidden, totalOfSymlinks, totalOfExecutables;
};

static void getDirectoryEntriesStatus(struct DirectoryEntriesStatus *status) {
    std::memset(status, 0, sizeof(struct DirectoryEntriesStatus));
    DIR *stream = opendir(".");
    if (!stream) {
        return;
    }
    for (struct dirent *entry; (entry = readdir(stream));) {
        if (!std::strcmp(entry->d_name, ".") ||
            !std::strcmp(entry->d_name, "..")) {
            continue;
        }
        std::string path = "./" + std::string(entry->d_name);
        struct stat metadata;
        if (lstat(path.c_str(), &metadata)) {
            continue;
        }
        if (entry->d_name[0] == '.') {
            status->totalOfHidden++;
        }
        if (S_ISLNK(metadata.st_mode)) {
            status->totalOfSymlinks++;
        }
        if (!S_ISDIR(metadata.st_mode) && metadata.st_mode & S_IXUSR) {
            status->totalOfExecutables++;
        }
    }
    closedir(stream);
}

static void printDirectoryEntryStatus(size_t status, int color,
                                      std::string symbol) {
    if (status) {
        std::cout << " %F{" << color << "}" << symbol << "%f" << status;
    }
}

static void printJobsStatus() {
    std::cout << "%(1j. %F{5} %f %j.)";
}

int main() {
    DirectoryEntriesStatus status;
    getDirectoryEntriesStatus(&status);
    printDirectoryEntryStatus(status.totalOfHidden, 1, " ");
    printDirectoryEntryStatus(status.totalOfExecutables, 2, "󱖏 ");
    printDirectoryEntryStatus(status.totalOfSymlinks, 4, "󰌷 ");
    printJobsStatus();
    std::cout << std::endl;
    return EXIT_SUCCESS;
}
