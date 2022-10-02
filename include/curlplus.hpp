#ifndef CURLPLUS_HPP
#define CURLPLUS_HPP

#include <curl/curl.h>
#include "strpp.hpp"

using namespace std;

/// @brief namespace to underlying curl wrapper methods
namespace curlplus
{
#define print(format, args...) printf("[%s:%s():%d] " format "\n", __FILE__, __func__, __LINE__, ##args)
#define PATH_CA_DIR "/etc/ssl/certs/"
#define PATH_DEFAULT_CACERT "/etc/ssl/certs/ca-certificates.crt"

    /// @brief callback to write data received from server to a local file
    /// @param buffer holds data received from the server
    /// @param size size in bytes per block
    /// @param nmemb number of blocks received
    /// @param stream FILE pointer
    /// @return number of bytes written to local file
    extern size_t file_write_callback(void *buffer, size_t size, size_t nmemb, void *stream);

    /// @brief callback to write data received from server to a string
    /// @param buffer holds data received from the server
    /// @param size size in bytes per block
    /// @param nitems number of blocks received
    /// @param userdata pointer to a string where received data is written to
    /// @return number of bytes written to string
    extern size_t str_write_callback(char *buffer, size_t size, size_t nitems, void *userdata);

    /// @brief callback to read content of a the file and send to destination
    /// @param dest destination where a local file content is to be stored
    /// @param size number of bytes per block
    /// @param nmemb number of blocks
    /// @param userp pointer to local file whose content is to be read
    /// @return number of bytes read
    size_t file_read_callback(char *dest, size_t size, size_t nmemb, void *userp);

    // tls versions
    enum TLS
    {
        None,
        v1_2,
        v1_3,
    };

    /// @brief Wrapper class for curl easy operations
    class Easy
    {
    public:
        Easy();
        virtual ~Easy();

        /// @brief initializes curl instance
        /// @return true, if successful
        bool init();

        /// @brief sets security parameters for the curl session
        /// @param clientCert full path to client certificate (PEM)
        /// @param clientKey full path to client private key
        /// @param caCert full path to server's CA cert (optional - defaults to /etc/ssl/certs/)
        /// @param tlsv TLS version to be used, defaults to TLS 1.3
        void setSecurity(const std::string &clientCert, const std::string &clientKey,
                         const std::string &caCert = PATH_DEFAULT_CACERT,
                         const TLS tlsv = TLS::v1_3);

        /// @brief closes curl session
        void close();

        /// @brief makes GET request
        /// @param url URL
        /// @param result result
        /// @param respHeader HTTP response header received
        /// @param reqHeader HTTP request header to be sent
        /// @return CURLcode
        CURLcode get(const std::string &url, std::string &result, std::string &respHeader, const std::string &reqHeader = "");

        /// @brief makes a GET request to download a file
        /// @param url URL
        /// @param saveasfile save the downloaded file as the name provided, must includee absolute path
        /// @param respHeader HTTP reponse header received
        /// @param reqHeader HTTP request header to be sent
        /// @return CURLCode
        CURLcode getFile(const std::string &url, const std::string &saveasfile, std::string &respHeader, const std::string &reqHeader = "");

        /// @brief makes a PUT request
        /// @param url URL
        /// @param filepath absoute path of local file to be put
        /// @param respHeader HTTP reponse header received
        /// @param reqHeader HTTP request header to be sent
        /// @return CURLCode
        CURLcode putFile(const std::string &url, const std::string &filepath, std::string &respHeader, const std::string &reqHeader = "");

        /// @brief post data to URL
        /// @param url URL
        /// @param postData data to be posted
        /// @param result result received from server
        /// @param respHeader HTTP response header received from server
        /// @param reqHeader HTTP request header sent to server
        /// @return CURLCode
        CURLcode post(const std::string &url, const std::string &postData, std::string &result, std::string &respHeader, const std::string &reqHeader = "");

        /// @brief post a local file content to server
        /// @param url URL
        /// @param filepath full path to the local file to be posted to server
        /// @param result result received from server
        /// @param respHeader HTTP response header received from server
        /// @param reqHeader HTTP request header sent to server
        /// @return CURLCode
        CURLcode postFile(const std::string &url, const std::string &filepath, std::string &result, std::string &respHeader, const std::string &reqHeader = "");

        /// @brief get HTTP response code
        /// @return HTTP response code
        long _getResponseCode();

        /// @brief set user credentials, in case of FTP
        /// @param username username
        /// @param password password in plain text
        void _setUserCreds(const std::string &username, const std::string &password);

        /// @brief set certificate paths
        /// @param clientCert full path to client certificate
        /// @param clientKey full path to client private key
        /// @param caCert full path CA certificate (optional, defaults /etc/ssl/certs/ca-certificates.crt)
        void _setCerts(const std::string &clientCert, const std::string &clientKey, const std::string &caCert = PATH_DEFAULT_CACERT);

        /// @brief set path to directory containing CA certs
        /// @param pathCADir full path to CA certs directory (optional, defaults to /etc/ssl/certs/)
        void _setCADir(const std::string &pathCADir = PATH_CA_DIR);

        /// @brief set TLS version to be used
        /// @param tlsv TLS version
        void _setTLS(const TLS &tlsv);

        /// @brief sets timeout for curl connection
        /// @param timeoutms timeout in milliseconds
        void _setTimeout(const int64_t &timeoutms);

        /// @brief set lower limit for speed and duration beyond which connetion will be closed
        /// @param lowBwSpeed lower limit of acceptable speed
        /// @param lowBwDuration duration for which speed should be below lowBwSpeed to close connection
        void _setLowBandwidthTimout(const int64_t &lowBwSpeed, const int64_t &lowBwDuration);

        /// @brief set cipher suites to be used
        /// @param cipher cipher suites
        void _setCipher(const string &cipher);

        /// @brief set curl verbose mode
        /// @param  on for verbose, off otherwise
        void _setVerbose(bool);

        /// @brief forcefully close connection at the end of the operation, by default curl keeps connection alive
        /// @param  true, to close connection
        void _forceCloseConnection(bool);

        /// @brief get data transfer metrics in terms of bytes per second and time in micro seconds
        /// @param speed_byte_per_sec speed in terms of bytes per secon
        /// @param time_micro_sec duration in micro seconds
        void _getTxMetrics(uint64_t &speed_byte_per_sec, uint64_t &time_micro_sec);

    private:
        CURL *m_ptrCurl;
        CURLcode run(const std::string &url, std::string &respHeader, const std::string &reqHeader = "");
        CURLcode set_http_headers(struct curl_slist **retHrdList, const std::string &url, std::string &respHeader, const std::string &reqHeader = "");
        void reset_curl_options();
        std::string get_protocol(const std::string &url);
    };
}
#endif