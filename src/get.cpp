#include <cassert>
#include "curlplus.hpp"
#include "http.hpp"
#include "file.hpp"

using namespace curlplus;
using namespace std;

CURLcode Easy::get(const std::string &url, std::string &result, std::string &respHeader, const std::string &reqHeader)
{
    result.clear();

    // capture result from GET request into string
    curl_easy_setopt(m_ptrCurl, CURLOPT_WRITEFUNCTION, str_write_callback);

    // Set a pointer to string where results should be captured
    curl_easy_setopt(m_ptrCurl, CURLOPT_WRITEDATA, &result);

    // Set http GET request
    curl_easy_setopt(m_ptrCurl, CURLOPT_HTTPGET, 1L);

    // run curl
    CURLcode ret = this->run(url, respHeader, reqHeader);

    // reset curl options
    reset_curl_options();

    return ret;
}

CURLcode Easy::getFile(const std::string &url, const std::string &saveasfile, std::string &respHeader, const std::string &reqHeader)
{
    if (saveasfile.empty())
    {
        return CURLE_BAD_FUNCTION_ARGUMENT;
    }
    struct File file = {saveasfile.c_str(), File::FileMode::NONE};

    /* Define our callback to get called when there's data to be written */
    curl_easy_setopt(m_ptrCurl, CURLOPT_WRITEFUNCTION, file_write_callback);

    /* Set a pointer to our struct to pass to the callback */
    curl_easy_setopt(m_ptrCurl, CURLOPT_WRITEDATA, &file);

    // Set http GET request
    curl_easy_setopt(m_ptrCurl, CURLOPT_HTTPGET, 1L);

    // run curl
    CURLcode ret = this->run(url, respHeader, reqHeader);

    // reset curl options
    reset_curl_options();

    return ret;
}
