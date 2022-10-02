#include "http.hpp"
#include "strpp.hpp"

map<HttpStatusCode, string> Http::m_mapHttpErrors = {
    {INVALID, "Http connection closed before data transfer"},
    {OK, to_string(OK) + "OK"},
    {OK_CREATED, to_string(OK_CREATED) + "-Created"},
    {OK_ACCEPTED, to_string(OK_ACCEPTED) + "-Accepted"},
    {OK_NO_CONTENT, to_string(OK_NO_CONTENT) + "-No content"},
    {OK_PARTIAL_CONTENT, to_string(OK_PARTIAL_CONTENT) + "-Partial content"},
    {OK_IM_USED, to_string(OK_IM_USED) + "-IM used"},
    {OK_MAX, to_string(OK_MAX) + "-MAX"},
    {MOVED_PERMANENTLY, to_string(MOVED_PERMANENTLY)+ "-Moved Permanently"},
    {MOVED_TEMPORARILY, to_string(MOVED_TEMPORARILY) + "-Requested resource has been temporarily moved to a different URI"},
    {BAD_REQUEST, to_string(BAD_REQUEST) + "-Bad request"},
    {UNAUTHORIZED, to_string(UNAUTHORIZED) + "-Unauthorized access"},
    {FORBIDDEN, to_string(FORBIDDEN) + "-Forbidden"},
    {NOT_FOUND, to_string(NOT_FOUND) + "-Resource not found"},
    {REQUEST_TIMEOUT, to_string(REQUEST_TIMEOUT) + "-Request timed out"},
    {INTERNAL_SERVER_ERROR, to_string(INTERNAL_SERVER_ERROR) + "- Internal server error"},
    {NOT_IMPLEMENTED, to_string(NOT_IMPLEMENTED) + "-Not implemented"},
    {BAD_GATEWAY, to_string(BAD_GATEWAY) + "-Bad gateway"},
    {SERVICE_UNAVAILABLE, to_string(SERVICE_UNAVAILABLE) + "- Service unavailable"},
    {GATEWAY_TIMEOUT, to_string(GATEWAY_TIMEOUT) + "-Gatewy timed out"},
};
map<string, string> Http::m_mapMIMEType = {
    {"all", "*/*"},
    {"bin", "application/octet-stream"},
    {"bz", "application/x-bzip"},
    {"bz2", "application/x-bzip2"},
    {"css", "text/css"},
    {"csv", "text/csv"},
    {"der", "application/x-x509-ca-cert"},
    {"gif", "image/gif"},
    {"htm", "text/html"},
    {"html", "text/html"},
    {"jpeg", "image/jpeg"},
    {"jpg", "image/jpg"},
    {"js", "application/js"},
    {"json", "application/json"},
    {"pdf", "application/pdf"},
    {"png", "image/png"},
    {"tar", "application/x-tar"},
    {"tif", "image/tiff"},
    {"tiff", "image/tiff"},
    {"txt", "text/plain"},
    {"wav", "audio/x-wav"},
    {"xml", "application/xml"},
    {"zip", "application/zip"},
    {"7z", "application/x-7z-compressed"},
    {"url-enc", "application/x-www-form-urlencoded"}};

string Http::getErrorMsg(const HttpStatusCode &httpCode)
{
    if (m_mapHttpErrors.find(httpCode) == m_mapHttpErrors.end())
    {
        return to_string(httpCode) + "-Undefined";
    }
    return m_mapHttpErrors[httpCode];
}

string Http::getMIMEType(const string &extension)
{
    if (extension.empty() || (m_mapMIMEType.find(extension) == m_mapMIMEType.end()))
    {
        return m_mapMIMEType["all"];
    }
    return m_mapMIMEType[extension];
}

string Http::getRespFor(const string &tag, const string &header)
{
    string respValue = "";
    size_t pos = header.find(tag);
    if (pos == string::npos)
    {
        return respValue;
    }
    respValue = header.substr(pos);
    respValue = strpp::trim(strpp::btwnDelimiters(respValue, ":", "\r\n")); // remove rest of the string after \r\n
    return respValue;
}

void HttpRequest::addReqHeader(const std::string &header)
{
    string hdrKey = strpp::trim(strpp::split(header, ":")[0]);
    string hdrValue = strpp::trim(strpp::split(header, ":")[1]);
    if (m_mapReqHeader.find(hdrKey) == m_mapReqHeader.end())
    {
        // new header, insert
        m_mapReqHeader.insert({hdrKey, hdrValue});
    }
    else
    {
        // existing header, update
        m_mapReqHeader[hdrKey] += ", " + hdrValue;
    }
}

string HttpRequest::getReqHeader()
{
    string headerStr = "";
    for (map<string, string>::iterator it = m_mapReqHeader.begin(); it != m_mapReqHeader.end(); it++)
    {
        headerStr += string(it->first) + ": " + string(it->second) + "\n";
    }
    clearReqHeader(); // clear map to prevent reuse of stale info in next header
    return strpp::trim(headerStr);
}

void HttpRequest::clearReqHeader()
{
    m_mapReqHeader.clear();
}
