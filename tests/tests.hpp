#ifndef TESTS_HPP
#define TESTS_HPP

#include "curlplus.hpp"

using namespace std;
using namespace curlplus;

class Tests
{
private:
    curlplus::Easy curlAgent;
    void initTests();
    void closeTests();
    void post_urlenc(const string &url);
    void post_json(const string &url);

public:
    Tests();
    void post(const string &url);
    void postFile(const string &url);
    void get(const string &url);
    void getFile(const string &url);
};

#endif