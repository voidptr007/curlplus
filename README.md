# Project: curlplus
curlplus is a C++ wrapper over curl easy APIs. 
It provides APIs to perform get, put and post operations using curl easy APIs.

## Code structure
src directory contains all the wrapper source code for curl easy APIs. 
tests directory contains test methods and entry point main function.

## Dependencies
Requires libcurl version 7.58 or above with openssl 1.1.1.

## How to build
Project can be built using cmake. 
Clone the project. Goto project root directory.
Create a build directory at the same level as src. 
Goto build directory and run cmake ..
Run make
Run curlplus_test

```
cd simplicurl
mkdir build
cd build
cmake ..
make
./curlplus_test
```
## Examples
Refer tests for illustration of usage of curlplus library.
