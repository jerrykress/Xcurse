#include "Converter.h"

namespace Xcurse
{
    /**
     * @brief Convert wstring to string
     *
     * @param wstr
     * @return std::string
     */
    std::string to_string(std::wstring wstr)
    {
        return strconverter.to_bytes(wstr);
    }

    /**
     * @brief Convert string to wstring
     *
     * @param str
     * @return std::wstring
     */
    std::wstring to_wstring(std::string str)
    {
        return strconverter.from_bytes(str);
    }
}