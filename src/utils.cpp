#include "utils.h"

namespace Utils {
    std::string getFileName(const char* path) {
        std::string p(path);

        size_t idx = p.find_last_of('/');

        if (idx == std::string::npos) {
            idx = 0;
        }

        return p.substr(idx == 0 ? idx : idx + 1, p.size());
    }
}
