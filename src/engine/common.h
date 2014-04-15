#ifndef COMMON_H
#define COMMON_H

#include <string>
using std::string;
#include <iostream>
#include <assert.h>

#define SINGLETON(classname) \
private: \
    classname(); \
    classname(const classname&); \
    classname& operator=(const classname&); \

#endif // COMMON_H
