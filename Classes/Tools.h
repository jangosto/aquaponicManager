#include <string>
#include <vector>

namespace Tools
{
    class StrTools
    {
        public:
            static std::vector<std::string> split(const std::string &text, char sep);
    };
}
