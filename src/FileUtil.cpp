#include <FileUtil.h>
#include <ConsoleIO.h>
#include <StringUtil.h>
#include <fstream>
#include <array>

bool validateAndSetPath(fs::path &basePath, const std::string &basePathStr) {
    if (basePathStr.empty()) {
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