#include <ProjectRenaming.h>
#include <ConsoleIO.h>
#include <FileUtil.h>
#include <StringUtil.h>
#include <filesystem>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;

void renameProject(const std::string& oldName, const std::string& oldNamePath, const std::string& newName, const std::string& newNamePath)
{
    std::string confirm;
    printToConsole("You are about to rename the project '" + oldName + "' to '" + newName + "'. Are you sure? [Y/n]: ");
    std::getline(std::cin, confirm);

    if(!confirm.empty() && (tolower(confirm[0]) == 'n'))
    {
        printToConsole("Operation cancelled.");
        return;
    }

    fs::path oldPath(oldNamePath);
    fs::path newPath(newNamePath);

    // remove old solution file
    fs::path oldSlnPath = oldPath / (oldName + ".sln");
    if (fs::exists(oldSlnPath)) {
        fs::remove(oldSlnPath);
        printToConsole("Removed solution file: " + oldSlnPath.string());
    }

    // process .uproject file
    const fs::path uprojectFilePath = oldPath / (oldName + ".uproject");
    replaceInFile(uprojectFilePath, oldName, newName);
    fs::rename(uprojectFilePath, oldPath / (newName + ".uproject"));

    // first pass: rename files and replace content in the Source directory
    fs::path sourcePath = oldPath / "Source";
    size_t fileCount = 0;
    std::string spinner = "|/-\\";
    for (auto& p : fs::recursive_directory_iterator(sourcePath))
    {
        if (p.is_regular_file()) {
            fs::path newFilePath = p.path();
            std::string oldFileName = p.path().filename().string();

            if(oldFileName.find(oldName) != std::string::npos)
            {
                std::string newFileName = stringReplace(oldFileName, oldName, newName);
                newFilePath = p.path().parent_path() / newFileName;
                fs::rename(p.path(), newFilePath);
            }

            replaceInFile(newFilePath, oldName, newName);
            fileCount++;
            printToConsole("\rProcessing file " + std::to_string(fileCount) + ": " + spinner[fileCount % spinner.size()], "default", true);
        }
    }

    // second pass: collect directories to rename
    std::vector<fs::path> dirsToRename;
    for (auto& p : fs::recursive_directory_iterator(sourcePath))
    {
        if (p.is_directory() && p.path().filename().string().find(oldName) != std::string::npos) {
            dirsToRename.push_back(p.path());
        }
    }

    // rename directories in reverse order to handle nested directories
    for (auto it = dirsToRename.rbegin(); it != dirsToRename.rend(); ++it)
    {
        std::string newDirName = stringReplace(it->filename().string(), oldName, newName);
        fs::path newDirPath = it->parent_path() / newDirName;
        fs::rename(*it, newDirPath);
    }

    // Finally rename the project folder
    fs::rename(oldPath, newPath);

    printToConsole("Cleaning project files..");
    cleanProject(newPath);
    printToConsole("\rFinished cleaning project files.", "default", true);

    printToConsole("All Done!");
}