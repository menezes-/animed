#pragma once

#include <string>
#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>

//tirado de: http://stackoverflow.com/questions/3613284/c-stdstring-to-boolean
bool toBool(std::string &str);

bool toInt(const std::string &str, int &value);

bool toFloat(const std::string &str, float &value);

bool startsWith(const std::string &target, const std::string &value);

bool startsWith(const std::string &target, const char *path);

void toLower(std::string &value);

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


//implementação de make_unique, tirado de http://stackoverflow.com/a/17902439/482238
namespace std {
    template<class T>
    struct _Unique_if {
        typedef unique_ptr<T> _Single_object;
    };

    template<class T>
    struct _Unique_if<T[]> {
        typedef unique_ptr<T[]> _Unknown_bound;
    };

    template<class T, size_t N>
    struct _Unique_if<T[N]> {
        typedef void _Known_bound;
    };

    template<class T, class... Args>
    typename _Unique_if<T>::_Single_object
    make_unique(Args &&... args) {
        return unique_ptr<T>(new T(std::forward<Args>(args)...));
    }

    template<class T>
    typename _Unique_if<T>::_Unknown_bound
    make_unique(size_t n) {
        typedef typename remove_extent<T>::type U;
        return unique_ptr<T>(new U[n]());
    }

    template<class T, class... Args>
    typename _Unique_if<T>::_Known_bound
    make_unique(Args &&...) = delete;
    //teste
}
