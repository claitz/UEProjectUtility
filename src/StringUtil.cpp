#include <StringUtil.h>
#include <algorithm>

std::string stringReplace(std::string str, const std::string& from, const std::string& to) {
    size_t startPos = 0;
    while((startPos = str.find(from, startPos)) != std::string::npos) {
        str.replace(startPos, from.length(), to);
        startPos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

std::string toUpper(const std::string &s) {
    std::string result(s);
    std::transform(s.begin(), s.end(), result.begin(),
                   [](unsigned char c){ return std::toupper(c); }
    );
    return result;
}
