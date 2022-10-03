#pragma once

#include <string>
#include <codecvt>
#include <locale>

namespace Xcurse
{
    using convert_t = std::codecvt_utf8<wchar_t>;

    static std::wstring_convert<convert_t, wchar_t> strconverter;

    std::string to_string(std::wstring wstr);

    std::wstring to_wstring(std::string str);

}