#include <iostream>
#include <filesystem>
#include <stdlib.h>
#include <fstream>
#include "tests.hpp"
#include "http.hpp"

Tests::Tests()
{
}

void Tests::initTests()
{
    curlAgent.init();
    curlAgent._setVerbose(false);
    curlAgent._setTLS(TLS::v1_3);
    curlAgent._forceCloseConnection(true);
}

void Tests::closeTests()
{
    curlAgent.close();
}

void Tests::get(const string &url)
{
    initTests();

    string httpResponse{};
    string result;
    HttpRequest header;
    header.addReqHeader("Accept: application/json");
    if (curlAgent.get(url, result, httpResponse, header.getReqHeader()) != CURLE_OK)
    {
        cout << "GET Failed. Http code: " << curlAgent._getResponseCode() << " - " << Http::getErrorMsg((HttpStatusCode)curlAgent._getResponseCode()) << endl;
    }
    else
    {
        cout << "GET Passed" << endl;
    }

    closeTests();
}

void Tests::getFile(const string &url)
{
    initTests();

    string httpResponse{};
    srand(time(NULL));
    int32_t rndNum = rand() % 100000;
    string localFilePath = "/tmp/getFile" + to_string(rndNum);
    filesystem::path fpath = localFilePath;

    HttpRequest header;
    header.addReqHeader("Accept: application/json");

    if ((curlAgent.getFile(url, localFilePath, httpResponse, header.getReqHeader()) != CURLE_OK) ||
        curlAgent._getResponseCode() != HttpStatusCode::OK || (!filesystem::exists(fpath)))
    {
        cout << "GETFILE Failed. Http code: " << curlAgent._getResponseCode() << " - " << Http::getErrorMsg((HttpStatusCode)curlAgent._getResponseCode()) << endl;
    }
    else
    {
        cout << "GETFILE Passed" << endl;
        filesystem::remove(fpath);
    }

    closeTests();
}

void Tests::post(const string &url)
{
    post_urlenc(url);
    post_json(url);
}

void Tests::post_urlenc(const string &url)
{
    initTests();

    string httpResponse{};
    string result;
    string postData = "Key1=ValueOfKey1&Key2=ValueOfKey2";
    if (curlAgent.post(url, postData, result, httpResponse) != CURLE_OK ||
        curlAgent._getResponseCode() != HttpStatusCode::OK)
    {
        cout << "POST (URL-ENC) Failed. Http code: " << curlAgent._getResponseCode() << " - " << Http::getErrorMsg((HttpStatusCode)curlAgent._getResponseCode()) << endl;
    }
    else
    {
        cout << "POST (URL-ENC) Passed" << endl;
    }

    closeTests();
}

void Tests::post_json(const string &url)
{
    initTests();

    string httpResponse{};
    HttpRequest header;
    string result;
    string postData = "{Key1: ValueOfKey1, Key2: ValueOfKey2}";
    header.addReqHeader("Content-Type: application/json");
    if (curlAgent.post(url, postData, result, httpResponse, header.getReqHeader()) != CURLE_OK ||
        curlAgent._getResponseCode() != HttpStatusCode::OK)
    {
        cout << "POST (JSON) Failed. Http code: " << curlAgent._getResponseCode() << " - " << Http::getErrorMsg((HttpStatusCode)curlAgent._getResponseCode()) << endl;
    }
    else
    {
        cout << "POST (JSON) Passed" << endl;
    }

    closeTests();
}

void Tests::postFile(const string &url)
{
    initTests();

    string httpResponse{};
    string result;

    // create a temp file to post
    srand(time(NULL));
    int32_t rndNum = rand() % 100000;
    string localFilePath = "/tmp/postFile" + to_string(rndNum);
    std::ofstream file_to_put(localFilePath);
    file_to_put << "This is sample text file to test post - " << rndNum << endl;
    file_to_put.close();

    HttpRequest header;
    header.addReqHeader("Accept: application/json");
    header.addReqHeader("Content-Type: text/plain");

    if ((curlAgent.postFile(url, localFilePath, result, httpResponse, header.getReqHeader()) != CURLE_OK) ||
        curlAgent._getResponseCode() != HttpStatusCode::OK)
    {
        cout << "POSTFILE Failed. Http code: " << curlAgent._getResponseCode() << " - " << Http::getErrorMsg((HttpStatusCode)curlAgent._getResponseCode()) << endl;
    }
    else
    {
        cout << "POSTFILE Passed" << endl;
        filesystem::path fpath = localFilePath;
        filesystem::remove(fpath);
    }

    closeTests();
}