#include <iostream>
#include "tests.hpp"
#include "curlplus.hpp"
#include "http.hpp"

using namespace std;

int main()
{
    Tests test;
    test.get("<specify get url here>");
    test.getFile("<specify get url here>");
    test.post("<specify post url here>");
    test.postFile("<specify post url here>");
    return 0;
}