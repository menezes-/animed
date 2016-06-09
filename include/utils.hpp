#pragma once

namespace constants {
    const char PATH_SEPARATOR =
#if defined(WIN32) || defined(_WIN32)
            '\\';
#else
    '/';
#endif

}
