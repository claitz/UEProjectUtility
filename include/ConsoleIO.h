#pragma once
#include <string>

void printToConsole(const std::string& message, const std::string& color = "default", bool flush = false);

void requestAndProcessInput(const std::string &requestMsg, std::string &input, bool canCancel = false);