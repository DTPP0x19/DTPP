#ifndef SYMBOL_CACHE_H
#define SYMBOL_CACHE_H

#include <unordered_map>
#include "dobby.h"
class SymbolCache {
private:
    std::unordered_map<char *, void *> cacheMap;

public:
    void * getFromDylib(char *key);
    void * get(char *key);
};

#endif