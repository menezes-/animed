#pragma once

#include <string>

//tirado de: http://stackoverflow.com/questions/3613284/c-stdstring-to-boolean
bool toBool(std::string &str);

bool toInt(const std::string &str, int &value);

bool toFloat(const std::string &str, float &value);

bool fileExists(const std::string &string);

bool startsWith(const std::string &target, const std::string &value);

bool startsWith(const std::string &target, const char *path);

void toLower(std::string& value);

bool dirExists(const char *path);

bool dirExists(const std::string& path);

template<class ContainerT>
inline void tokenize(const std::string &str, ContainerT &tokens,
                     const std::string &delimiters = " ", bool trimEmpty = false) {
    std::string::size_type pos, lastPos = 0;

    using value_type = typename ContainerT::value_type;
    using size_type  = typename ContainerT::size_type;

    while (true) {
        pos = str.find_first_of(delimiters, lastPos);
        if (pos == std::string::npos) {
            pos = str.length();

            if (pos != lastPos || !trimEmpty)
                tokens.push_back(value_type(str.data() + lastPos,
                                            (size_type) pos - lastPos));

            break;
        }
        else {
            if (pos != lastPos || !trimEmpty)
                tokens.push_back(value_type(str.data() + lastPos,
                                            (size_type) pos - lastPos));
        }

        lastPos = pos + 1;
    }
}