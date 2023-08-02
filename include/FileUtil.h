#pragma once
#include <filesystem>

namespace fs = std::filesystem;

bool validateAndSetPath(fs::path &basePath, const std::string &basePathStr);

void cleanProject(const fs::path& projectPath);

bool validateUProject(const fs::path &basePath);

void replaceInFile(const fs::path& path, const std::string& oldName, const std::string& newName);