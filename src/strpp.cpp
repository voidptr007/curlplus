#include <algorithm>
#include <string>
#include "strpp.hpp"

bool strpp::beginsWith(const std::string &str1, const std::string &str2)
{
    return (str1.size() >= str2.size()) && (str1.compare(0, str2.size(), str2)) == 0;
}

bool strpp::endswith(const std::string &str1, const std::string &str2)
{
    return (str1.size() >= str2.size()) &&
           (str1.compare(str1.size() - str2.size(), str2.size(), str2)) == 0;
}

std::string strpp::btwnDelimiters(const std::string &src_str, const std::string begin_delim, const std::string end_delim)
{
    std::string subString{};

    if (src_str.empty())
    {
        return subString;
    }

    uint32_t first_delim_pos = src_str.find(begin_delim);
    uint32_t second_delim_pos = src_str.find(end_delim);
    uint32_t end_of_first_delim_pos = first_delim_pos + begin_delim.length();

    subString = src_str.substr(end_of_first_delim_pos, second_delim_pos - end_of_first_delim_pos);

    return subString;
}

std::string strpp::eraseSpaces(const std::string &str)
{
    std::string result = str;
    if (!result.empty())
    {
        result.erase(remove(result.begin(), result.end(), ' '), result.end());
    }
    return result;
}

std::vector<std::string> strpp::split(const std::string str, const std::string &delimit)
{
    std::vector<std::string> strings{};
    std::string src_str = str;

    if (src_str.empty() || delimit.empty())
    {
        return strings;
    }
    size_t pos = 0U;
    while ((pos = src_str.find(delimit)) != std::string::npos)
    {
        strings.push_back(src_str.substr(0, pos));
        src_str.erase(0, pos + delimit.length());
    }
    strings.push_back(src_str);
    return strings;
}

std::string strpp::trimBeginning(const std::string &str, const std::string &chars)
{
    if (str.empty())
        return str;
    std::string result = str;
    result.erase(0, result.find_first_not_of(chars));
    return result;
}

std::string strpp::trimEnd(const std::string &str, const std::string &chars)
{
    if (str.empty())
        return str;
    std::string result = str;
    result.erase(result.find_last_not_of(chars) + 1);
    return result;
}

std::string strpp::trim(const std::string &str, const std::string &chars)
{
    std::string result = str;
    result = trimEnd(result, chars);
    result = trimBeginning(result, chars);
    return result;
}

std::string strpp::toUpper(const std::string &str)
{
    if (str.empty())
        return str;
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c)
                   { return std::toupper(c); });
    return result;
}

std::string strpp::toLower(const std::string &str)
{
    if (str.empty())
        return str;
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c)
                   { return std::tolower(c); });
    return result;
}

int32_t strpp::toInt32(const std::string &str, int32_t init)
{
    int32_t result = init;
    try
    {
        if (isNumber(str))
        {
            result = std::stoi(str);
        }
    }
    catch (...)
    {
    }
    return result;
}

bool strpp::isNumber(std::string token)
{
    if (token.empty())
        return false;
    return std::regex_match(token, std::regex(("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?")));
}

bool strpp::contains(const std::string &str1, const std::string &str2)
{
    bool found = false;
    if (!str1.empty() && !str2.empty() &&
        str1.find(str2) != std::string::npos)
    {
        found = true;
    }
    return found;
}