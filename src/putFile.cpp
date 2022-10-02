#include "curlplus.hpp"
#include "http.hpp"
#include "file.hpp"

using namespace curlplus;
using namespace std;

CURLcode Easy::putFile(const std::string &url, const std::string &filetoput, std::string &respHeader, const std::string &reqHeader)
{
    struct File file = {filetoput.c_str(), File::FileMode::R};
    if (!file.stream)
    {
        return CURLE_BAD_FUNCTION_ARGUMENT;
    }

    /* tell it to "upload" to the URL */
    curl_easy_setopt(m_ptrCurl, CURLOPT_UPLOAD, 1L);

    /* set where to read from */
    curl_easy_setopt(m_ptrCurl, CURLOPT_READDATA, file.stream);

    /* and give the size of the upload (optional) */
    curl_easy_setopt(m_ptrCurl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)file.getFileSize());

    // run
    CURLcode ret = this->run(url, respHeader, reqHeader);

    // reset options
    reset_curl_options();

    return ret;
}