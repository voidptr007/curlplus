#ifndef HTTP_HPP
#define HTTP_HPP

#include <string>
#include <map>

using namespace std;

enum HttpStatusCode
{
    INVALID = 0,
    OK = 200,
    OK_CREATED = 201,
    OK_ACCEPTED = 202,
    OK_NO_CONTENT = 204,
    OK_PARTIAL_CONTENT = 206,
    OK_IM_USED = 226,
    OK_MAX = 299,
    MOVED_PERMANENTLY = 301,
    MOVED_TEMPORARILY = 302,
    BAD_REQUEST = 400,
    UNAUTHORIZED = 401,
    FORBIDDEN = 403,
    NOT_FOUND = 404,
    REQUEST_TIMEOUT = 408,
    INTERNAL_SERVER_ERROR = 500,
    NOT_IMPLEMENTED = 501,
    BAD_GATEWAY = 502,
    SERVICE_UNAVAILABLE = 503,
    GATEWAY_TIMEOUT = 504,
};

class Http
{
public:
    /// @brief get http error description for a http status code
    /// @param httpCode http status code
    /// @return http error description
    static string getErrorMsg(const HttpStatusCode &httpCode);

    /// @brief gets MIME type for a given file extention
    /// @param extension file extention without .
    /// @return MIME type
    static string getMIMEType(const string &extension);

    /// @brief gets http response for given response-tag
    /// @param headertag tag
    /// @param header http response header
    /// @return value for tag
    static string getRespFor(const string &headertag, const string &header);

private:
    static map<HttpStatusCode, string> m_mapHttpErrors;
    static map<string, string> m_mapMIMEType;
};

class HttpRequest : public Http
{
public:
    HttpRequest() {}
    HttpRequest(const std::string &header)
    {
        addReqHeader(header);
    }

    ~HttpRequest()
    {
        clearReqHeader();
    }

    void addReqHeader(const std::string &header);
    string getReqHeader();
    void clearReqHeader();

private:
    map<string, string> m_mapReqHeader;
};

#endif