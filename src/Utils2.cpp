#include "../include/Utils2.hpp"
#include <algorithm>
#include <sstream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>


bool toBool(std::string &str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    std::istringstream is(str);
    bool b;
    is >> std::boolalpha >> b;
    return b;
}

bool toInt(const std::string &str, int &value) {
    try {
        value = std::stoi(str);
        return true;
    } catch (const std::exception &e) {
        return false;
    }
}

bool fileExists(const std::string &string) {
    std::ifstream f(string);
    return f.good();
}

bool startsWith(const std::string &target, const std::string &value) {
    return target.length() <= value.length()
           && std::equal(value.begin(), value.end(), target.begin());
}


bool startsWith(const std::string &target, const char *path) {
    return startsWith(target, std::string{path});
}

bool toFloat(const std::string &str, float &value) {
    try {
        value = std::stof(str);
        return true;
    } catch (const std::exception &e) {
        return false;
    }
}

void toLower(std::string &value) {
    std::transform(value.begin(), value.end(), value.begin(), ::tolower);
}

bool dirExists(const char *path) {
    struct stat info;

    if (stat(path, &info) != 0)
        return false;
    else return (info.st_mode & S_IFDIR) != 0;
}

bool dirExists(const std::string &path) {
    return dirExists(path.c_str());
}









