#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <array>
#include <limits>
#include <termcolor/termcolor.hpp>

namespace fs = std::filesystem;

void printToConsole(const std::string& message, const std::string& color = "default", const bool flush = false) {
    if (color == "red") {
        std::cout << termcolor::red << message << termcolor::reset;
    } else if (color == "green") {
        std::cout << termcolor::green << message << termcolor::reset;
    } else if (color == "cyan") {
        std::cout << termcolor::cyan << message << termcolor::reset;
    } else if (color == "on_blue") {
        std::cout << termcolor::on_blue << termcolor::white << message << termcolor::reset;
    } else if (color == "blue") {
        std::cout << termcolor::blue << message << termcolor::reset;
    } else {
        std::cout << message;
    }

    if (flush) {
        std::cout << std::flush;
    } else {
        std::cout << std::endl;
    }
}


std::string stringReplace(std::string str, const std::string& from, const std::string& to) {
    size_t startPos = 0;
    while((startPos = str.find(from, startPos)) != std::string::npos) {
        str.replace(startPos, from.length(), to);
        startPos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

std::string toUpper(const std::string &s)
{
    std::string result(s);
    std::transform(s.begin(), s.end(), result.begin(),
                   [](unsigned char c){ return std::toupper(c); }
    );
    return result;
}

bool validateAndSetPath(fs::path &basePath, const std::string &basePathStr) {
    if (basePathStr == "") {
        printToConsole("No path specified. Please try again.", "red");
        return false;
    }

    fs::path pathCandidate = fs::absolute(fs::path(basePathStr));
    if (!fs::exists(pathCandidate)) {
        printToConsole("The specified path does not exist. Please try again.", "red");
        return false;
    }
    basePath = pathCandidate;
    return true;
}

void requestAndProcessInput(const std::string &requestMsg, std::string &input, bool canCancel = false) {
    while (true) {
        printToConsole(requestMsg, "green");
        std::getline(std::cin, input);
        if (canCancel && toUpper(input) == "C") {
            throw std::runtime_error("Operation cancelled by user.");
        }
        if (input.empty()) {
            input = fs::current_path().string(); // default to current directory
        }
        break;
    }
}

void cleanProject(const fs::path& projectPath)
{
    fs::path slnPath = projectPath.parent_path() / (projectPath.filename().string() + ".sln");
    std::array<fs::path, 7> dirsToClean = {
            projectPath / ".vs",
            projectPath / "Build",
            projectPath / "Binaries",
            projectPath / "DerivedDataCache",
            projectPath / "Intermediate",
            projectPath / "Platforms",
            projectPath / "Saved"
    };

    for (const auto& dir : dirsToClean) {
        if (fs::exists(dir)) {
            fs::remove_all(dir);
        }
    }
}

bool validateUProject(const fs::path &basePath) {
    for (const auto & entry : fs::directory_iterator(basePath)) {
        if (entry.path().extension() == ".uproject") {
            return true;
        }
    }
    printToConsole("No .uproject file found in the specified directory. Please try again.", "red");
    return false;
}

void replaceInFile(const fs::path& path, const std::string& oldName, const std::string& newName)
{
    std::ifstream fileIn(path);
    std::string content((std::istreambuf_iterator<char>(fileIn)), std::istreambuf_iterator<char>());
    fileIn.close();

    // replacing oldName and oldName_API with newName and newName_API
    content = stringReplace(content, oldName, newName);
    content = stringReplace(content, toUpper(oldName) + "_API", toUpper(newName) + "_API");

    std::ofstream fileOut(path);
    fileOut << content;
    fileOut.close();
}

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
    fs::path uprojectFilePath = oldPath / (oldName + ".uproject");
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

int main()
{
    std::string basePathStr = fs::current_path().string();
    fs::path basePath;
    std::string oldName;
    std::string oldNamePath;
    std::string newName;
    std::string newNamePath;

    while (true) {
        printToConsole("=================== Menu ===================", "blue");
        printToConsole("1. Rename project");
        printToConsole("2. Clean project");
        printToConsole("3. Exit");
        printToConsole("\nEnter your choice: ", "green");

        int choice;
        if (!(std::cin >> choice)) {
            printToConsole("Invalid option. Please try again.", "red");
            std::cin.clear();  // Clear the error state
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;  // Skip the rest of the loop iteration
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                try {
                    requestAndProcessInput("Enter the base path (default is " + fs::current_path().string() + "): [C] Cancel", basePathStr, true);
                    if (!validateAndSetPath(basePath, basePathStr)) {
                        continue;  // Back to the start of the loop if path validation fails
                    }
                    while (true) {
                        requestAndProcessInput("Enter the old project name: [C] Cancel", oldName, true);
                        oldNamePath = (basePath / oldName).string();
                        if (validateUProject(fs::path(oldNamePath))) {
                            break;  // Exit loop if .uproject validation passes
                        }
                    }
                    requestAndProcessInput("Enter the new project name: [C] Cancel", newName, true);
                    newNamePath = (basePath / newName).string();
                    renameProject(oldName, oldNamePath, newName, newNamePath);
                    printToConsole("Project renamed successfully!", "green");
                } catch (const std::exception &e) {
                    printToConsole("Error: " + std::string(e.what()), "red");
                }
                break;
            case 2:
                try {
                    requestAndProcessInput("Enter the base path (default is " + fs::current_path().string() + "): [C] Cancel", basePathStr, true);
                    if (!validateAndSetPath(basePath, basePathStr)) {
                        continue;  // Back to the start of the loop if path validation fails
                    }
                    while (true) {
                        requestAndProcessInput("Enter the project name to clean: [C] Cancel", oldName, true);
                        oldNamePath = (basePath / oldName).string();
                        if (validateUProject(fs::path(oldNamePath))) {
                            break;  // Exit loop if .uproject validation passes
                        }
                    }
                    cleanProject(fs::path(oldNamePath));
                    printToConsole("Project cleaned successfully!", "green");
                } catch (const std::exception &e) {
                    printToConsole("Error: " + std::string(e.what()), "red");
                }
                break;
            case 3:
                return 0;
            default:
                printToConsole("Invalid option. Please try again.", "red");
        }

    }
}
