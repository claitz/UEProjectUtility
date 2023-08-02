#include <filesystem>
#include <iostream>
#include <string>
#include <algorithm>
#include <limits>
#include <ConsoleIO.h>
#include <FileUtil.h>
#include <ProjectRenaming.h>

int main()
{
    std::string basePathStr = fs::current_path().string();
    fs::path basePath;
    std::string oldName;
    std::string oldNamePath;
    std::string newName;
    std::string newNamePath;

    while (true) {
        printToConsole(" __  __     ______     ______   __  __    \n"
                       "/\\ \\/\\ \\   /\\  ___\\   /\\  == \\ /\\ \\/\\ \\   \n"
                       "\\ \\ \\_\\ \\  \\ \\  __\\   \\ \\  _-/ \\ \\ \\_\\ \\  \n"
                       " \\ \\_____\\  \\ \\_____\\  \\ \\_\\    \\ \\_____\\ \n"
                       "  \\/_____/   \\/_____/   \\/_/     \\/_____/ \n", "blue");
        printToConsole("=========== UEProjectUtility v0.3 ===========", "blue");
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
