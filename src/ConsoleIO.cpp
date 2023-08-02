#include <ConsoleIO.h>
#include <StringUtil.h>
#include <filesystem>
#include <termcolor/termcolor.hpp>

namespace fs = std::filesystem;

void printToConsole(const std::string& message, const std::string& color, const bool flush) {
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

void requestAndProcessInput(const std::string &requestMsg, std::string &input, bool canCancel) {
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