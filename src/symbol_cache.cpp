#include "symbol_cache.h"
#include <dlfcn.h>

void *SymbolCache::get(char *key)
{
    // 先在缓存中查找
    auto it = cacheMap.find(key);
    if (it != cacheMap.end())
    {
        // 如果缓存中有，则返回缓存中的值
        return it->second;
    }
    else
    {
        // 如果缓存中没有，调用DobbySymbolResolver查找
        void *value = DobbySymbolResolver(nullptr, key);
        if (value != nullptr)
        {
            cacheMap[key] = value;
            return value;
        }
        // 如果DobbySymbolResolver也没有找到，返回0
        return nullptr;
    }
}
//从动态库中查询
void *SymbolCache::getFromDylib(char *key)
{
    // 先在缓存中查找
    auto it = cacheMap.find(key);
    if (it != cacheMap.end())
    {
        // 如果缓存中有，则返回缓存中的值
        return it->second;
    }
    else
    {
        // 如果缓存中没有，调用dlsym查找
        void *value = dlsym(RTLD_NEXT, key);
        if (value != nullptr)
        {
            cacheMap[key] = value;
            return value;
        }
        // 如果dlsym也没有找到，返回0
        return nullptr;
    }
}