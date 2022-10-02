#ifndef STRPP_HPP
#define STRPP_HPP

#include <string>
#include <vector>
#include <regex>

class strpp final
{
public:
    /// @brief checks if string 1 contains string 1
    /// @param str1 string 1
    /// @param str2 string 2
    /// @return true, if contains
    static bool contains(const std::string &str1, const std::string &str2);

    /// @brief Trims a string from the beginning
    /// @param str string to be trimmed (this will not be modified)
    /// @param chars delimiters to be trimmed
    /// @return trimmed string
    static std::string trimBeginning(const std::string &str, const std::string &chars = "\t\n\v\f\r ");

    /// @brief Trims a string from the end
    /// @param str string to be trimmed (this will not be modified)
    /// @param chars delimiters to be trimmed
    /// @return trimmed string
    static std::string trimEnd(const std::string &str, const std::string &chars = "\t\n\v\f\r ");

    /// @brief Trims a string from both the ends
    /// @param str string to be trimmed (this will not be modified)
    /// @param chars delimiters to be trimmed
    /// @return trimmed string
    static std::string trim(const std::string &str, const std::string &chars = "\t\n\v\f\r ");

    /// @brief Checks if str 1 begins with str 2
    /// @param str1 string to be checked
    /// @param str2 string to be found at the beginning of str1
    /// @return true, if found
    static bool beginsWith(const std::string &str1, const std::string &str2);

    /// @brief Checks of str1 ends with str 2
    /// @param str1 string to be checked
    /// @param str2 string to be found at the end of str 1
    /// @return true, if found
    static bool endswith(const std::string &str1, const std::string &str2);

    /// @brief Extract string between delimiters
    /// @param src_str string from which a substring is to be extracted
    /// @param begin_delim begin delimiter
    /// @param end_delim end delimiter
    /// @return substring
    static std::string btwnDelimiters(const std::string &src_str, const std::string begin_delim, const std::string end_delim);

    /// @brief Erases spaces in the string
    /// @param str input string in which spaces will be erased
    /// @return string without spaces
    static std::string eraseSpaces(const std::string &str);

    /// @brief Splits an input string using delimiter
    /// @param str string to be split
    /// @param delimit split delimiter
    /// @return vector of split strings
    static std::vector<std::string> split(const std::string str, const std::string &delimit);

    /// @brief Converts a string to lower case
    /// @param str string to be converted to lower case (this stirng is not modified)
    /// @return string in lower case
    static std::string toLower(const std::string &str);

    /// @brief Converts a string to upper case
    /// @param str string to be converted to upper case (this stirng is not modified)
    /// @return string in upper case
    static std::string toUpper(const std::string &str);

    /// @brief Converts a string to int32
    /// @param str string to be converted to int32
    /// @return value in int32
    static int32_t toInt32(const std::string &str, int32_t init = -1);

    /// @brief Checks if string is a number
    /// @param str string to be checked
    /// @return true, if a number
    static bool isNumber(std::string token);
};

#endif