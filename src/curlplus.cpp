#include <cassert>
#include "curlplus.hpp"
#include "http.hpp"
#include "file.hpp"

using namespace curlplus;
using namespace std;

namespace curlplus
{
    Easy::Easy()
    {
        m_ptrCurl = NULL;
        curl_global_init(CURL_GLOBAL_DEFAULT);
    }

    Easy::~Easy()
    {
        close();
        curl_global_cleanup();
    }

    void Easy::_setCADir(const std::string &pathCADir)
    {
        // Directory holding multiple CA certificates to verify the peer
        curl_easy_setopt(m_ptrCurl, CURLOPT_CAPATH, pathCADir.c_str());
    }

    void Easy::_setCerts(const std::string &clientCert, const std::string &clientKey, const std::string &caCert)
    {
        // File holding one or more certificates to verify the peer with
        curl_easy_setopt(m_ptrCurl, CURLOPT_CAINFO, caCert.c_str());

        // file name of your client certificate
        curl_easy_setopt(m_ptrCurl, CURLOPT_SSLCERT, clientCert.c_str());

        // file name of your private key
        curl_easy_setopt(m_ptrCurl, CURLOPT_SSLKEY, clientKey.c_str());

        // set default CA DIR
        _setCADir();
        return;
    }

    void Easy::_setUserCreds(const std::string &username, const std::string &password)
    {
        if (username.empty() || password.empty())
        {
            return;
        }
        curl_easy_setopt(m_ptrCurl, CURLOPT_USERNAME, username.c_str());
        string userpwd = username + ":" + password;
        curl_easy_setopt(m_ptrCurl, CURLOPT_USERPWD, userpwd.c_str());
    }

    void Easy::_setTLS(const TLS &tlsv)
    {
        // Set TLS version - default is always 1.3
        curl_easy_setopt(m_ptrCurl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_3);
        if (tlsv == TLS::v1_2)
        {
            curl_easy_setopt(m_ptrCurl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);
        }

        // Set the default value: strict certificate check please
        curl_easy_setopt(m_ptrCurl, CURLOPT_SSL_VERIFYPEER, 1L);

        // Set the default value: strict name check please */
        curl_easy_setopt(m_ptrCurl, CURLOPT_SSL_VERIFYHOST, 2L);

        /* require use of SSL for this, or fail */
        curl_easy_setopt(m_ptrCurl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
    }

    void Easy::_setTimeout(const int64_t &timeoutms)
    {
        // complete within timeoutms milliseconds
        curl_easy_setopt(m_ptrCurl, CURLOPT_TIMEOUT_MS, timeoutms);
    }

    void Easy::_setLowBandwidthTimout(const int64_t &lowBwSpeed, const int64_t &lowBwDuration)
    {
        /* abort if slower than lowBwSpeed bytes/sec during lowBwDuration seconds */
        curl_easy_setopt(m_ptrCurl, CURLOPT_LOW_SPEED_TIME, lowBwDuration);
        curl_easy_setopt(m_ptrCurl, CURLOPT_LOW_SPEED_LIMIT, lowBwSpeed);
    }

    void Easy::_setCipher(const string &cipher)
    {
        curl_easy_setopt(m_ptrCurl, CURLOPT_SSL_CIPHER_LIST, cipher.c_str());
    }

    void Easy::_setVerbose(bool verbose)
    {
        // set verbose
        curl_easy_setopt(m_ptrCurl, CURLOPT_VERBOSE, (verbose ? 1L : 0L));
        return;
    }

    void Easy::_forceCloseConnection(bool close)
    {
        int64_t connection_state = (close) ? 1L : 0L;
        // Close the connection when done with the transfer
        curl_easy_setopt(m_ptrCurl, CURLOPT_FORBID_REUSE, connection_state);
    }

    void Easy::_getTxMetrics(uint64_t &speed_byte_per_sec, uint64_t &time_micro_sec)
    {
        time_micro_sec = 0;
        speed_byte_per_sec = 0;
        curl_off_t txspeed, txtotaltime;
        if (curl_easy_getinfo(m_ptrCurl, CURLINFO_SPEED_UPLOAD_T, &txspeed) == CURLE_OK)
        {
            speed_byte_per_sec = (uint64_t)txspeed;
        }
        if (curl_easy_getinfo(m_ptrCurl, CURLINFO_TOTAL_TIME_T, &txtotaltime) == CURLE_OK)
        {
            time_micro_sec = (uint64_t)txtotaltime;
        }
    }

    long Easy::_getResponseCode()
    {
        long response_code;
        curl_easy_getinfo(m_ptrCurl, CURLINFO_RESPONSE_CODE, &response_code);
        return response_code;
    }

    //------------------------------------------------------------------------------------------------
    void Easy::reset_curl_options()
    {
        // reset header
        curl_easy_setopt(m_ptrCurl, CURLOPT_HTTPHEADER, NULL);
        curl_easy_setopt(m_ptrCurl, CURLOPT_HEADERFUNCTION, NULL);
        curl_easy_setopt(m_ptrCurl, CURLOPT_HEADERDATA, NULL);

        // reset get options
        curl_easy_setopt(m_ptrCurl, CURLOPT_HTTPGET, 0L);
        curl_easy_setopt(m_ptrCurl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(m_ptrCurl, CURLOPT_WRITEDATA, NULL);

        // reset put options
        curl_easy_setopt(m_ptrCurl, CURLOPT_UPLOAD, 0L);
        curl_easy_setopt(m_ptrCurl, CURLOPT_READDATA, NULL);
        curl_easy_setopt(m_ptrCurl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)0L);

        // reset post options
        curl_easy_setopt(m_ptrCurl, CURLOPT_POSTFIELDS, NULL);
        curl_easy_setopt(m_ptrCurl, CURLOPT_POSTFIELDSIZE, 0L);

        // reset url
        curl_easy_setopt(m_ptrCurl, CURLOPT_URL, NULL);
    }

    CURLcode Easy::set_http_headers(struct curl_slist **retHrdList, const std::string &url, std::string &respHeader, const std::string &reqHeader)
    {
        *retHrdList = NULL;
        // first clear the header (header set from previous calls)
        curl_easy_setopt(m_ptrCurl, CURLOPT_HTTPHEADER, *retHrdList);
        // Add custom headers
        if (!reqHeader.empty() && strpp::contains(get_protocol(url), "http"))
        {
            // set http req header. input header will haev \n separated headers.
            std::vector<std::string> headers = strpp::split(reqHeader, "\n");
            for (auto hdr : headers)
            {
                *retHrdList = curl_slist_append(*retHrdList, hdr.c_str());
            }
            curl_easy_setopt(m_ptrCurl, CURLOPT_HTTPHEADER, *retHrdList);
        }
        // send default/custom headers only to server (not to proxy)
        curl_easy_setopt(m_ptrCurl, CURLOPT_HEADEROPT, CURLHEADER_SEPARATE);

        // capture http response header
        if (!respHeader.empty())
        {
            curl_easy_setopt(m_ptrCurl, CURLOPT_HEADERFUNCTION, str_write_callback);
            curl_easy_setopt(m_ptrCurl, CURLOPT_HEADERDATA, &respHeader);
        }
        return CURLE_OK;
    }

    CURLcode Easy::run(const std::string &url, std::string &respHeader, const std::string &reqHeader)
    {
        struct curl_slist *headerList = NULL;

        // set headers
        set_http_headers(&headerList, url, respHeader, reqHeader);

        // Set URL
        CURLcode ret = curl_easy_setopt(m_ptrCurl, CURLOPT_URL, url.c_str());

        // run
        ret = curl_easy_perform(m_ptrCurl);
        if (CURLE_OK != ret)
        {
            print("curl_perform failed with curl error code:%d - %s for %s", ret, curl_easy_strerror(ret), url.c_str());
        }

        // clear all headers
        curl_slist_free_all(headerList);

        return ret;
    }

    std::string Easy::get_protocol(const std::string &url)
    {
        return strpp::toLower((strpp::split(url, "://")).front());
    }

    //------------------------------------------
    bool Easy::init()
    {
        if (!m_ptrCurl)
        {
            m_ptrCurl = curl_easy_init();
        }
        return (m_ptrCurl != NULL) ? true : false;
    }

    void Easy::setSecurity(const std::string &clientCert, const std::string &clientKey,
                           const std::string &caCert, const TLS tlsv)
    {
        _setCerts(clientCert, clientKey, caCert);
        _setTLS(tlsv);
    }


    void Easy::close()
    {
        if (m_ptrCurl)
        {
            curl_easy_reset(m_ptrCurl);
            curl_easy_cleanup(m_ptrCurl);
            m_ptrCurl = NULL;
        }
    }

    size_t str_write_callback(char *buffer, size_t size, size_t nitems, void *userdata)
    {
        std::string *header = (std::string *)userdata;
        header->append(buffer, nitems * size);
        return nitems * size;
    }

    size_t file_write_callback(void *buffer, size_t size, size_t nmemb, void *stream)
    {
        struct File *out = (struct File *)stream;
        if (!out->stream)
        {
            /* open file for writing */
            out->stream = fopen(out->filename, "wb");
            if (!out->stream)
                return -1; /* failure, cannot open file to write */
        }
        return fwrite(buffer, size, nmemb, out->stream);
    }

    size_t file_read_callback(char *dest, size_t size, size_t nmemb, void *userp)
    {
        struct File *file_to_send = (struct File *)userp;
        size_t readSize = fread(dest, size, nmemb, file_to_send->stream);
        return readSize; /* no more data left to deliver */
    }
};