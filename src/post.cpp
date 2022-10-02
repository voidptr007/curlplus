#include "curlplus.hpp"
#include "http.hpp"
#include "file.hpp"

using namespace curlplus;
using namespace std;

CURLcode Easy::post(const std::string &url, const std::string &postData, std::string &result, std::string &respHeader, const std::string &reqHeader)
{
    result.clear();

    // capture result from GET request into string
    curl_easy_setopt(m_ptrCurl, CURLOPT_WRITEFUNCTION, str_write_callback);

    // Set a pointer to string where results should be captured
    curl_easy_setopt(m_ptrCurl, CURLOPT_WRITEDATA, &result);

    // set post option
    // Note - setting CURLOPT_POST to 1L is not requried with POSTFIELDS
    curl_easy_setopt(m_ptrCurl, CURLOPT_POSTFIELDS, postData.c_str());
    curl_easy_setopt(m_ptrCurl, CURLOPT_POSTFIELDSIZE, (long)postData.length());

    // run curl
    CURLcode ret = this->run(url, respHeader, reqHeader);

    // reset curl options
    reset_curl_options();

    return ret;
}

CURLcode Easy::postFile(const std::string &url, const std::string &filepath, std::string &result, std::string &respHeader, const std::string &reqHeader)
{
    result.clear();

    struct File file = {filepath.c_str(), File::FileMode::R};
    if (!file.stream)
    {
        return CURLE_BAD_FUNCTION_ARGUMENT;
    }

    // capture result from GET request into string
    curl_easy_setopt(m_ptrCurl, CURLOPT_WRITEFUNCTION, str_write_callback);

    // Set a pointer to string where results should be captured
    curl_easy_setopt(m_ptrCurl, CURLOPT_WRITEDATA, &result);

    // set post option
    curl_easy_setopt(m_ptrCurl, CURLOPT_POST, 1L);

    // this must be explicitly set to NULL to avoid using url-form-encodded
    curl_easy_setopt(m_ptrCurl, CURLOPT_POSTFIELDS, NULL);

    /* we want to use our own read function */
    curl_easy_setopt(m_ptrCurl, CURLOPT_READFUNCTION, file_read_callback);

    /* pointer to pass to our read function */
    curl_easy_setopt(m_ptrCurl, CURLOPT_READDATA, &file);

    // run curl
    CURLcode ret = this->run(url, respHeader, reqHeader);

    // reset curl options
    reset_curl_options();

    return ret;
}