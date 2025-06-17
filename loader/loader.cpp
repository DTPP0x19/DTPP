#include <dlfcn.h>
#include <cstring>
#include <cstdint>
#include <iostream>
typedef int64_t (*o_get_system_info)(int64_t);
bool inited = false;
bool loaded = false;
// __attribute__((constructor)) void onload()
// {
//     if (!loaded)
//     {
//         // 创建并打开一个文件用于写入
//         tlog_init("/tmp/dtpp.log", 1024 * 1024, 2, 0, 0);
//         //tlog_info( "👋 DTPP Loader 0.1 Starting!");
//         loaded = true;
//     }
// }

extern "C" int64_t get_system_info(int64_t sn)
{

    if (!inited)
    {  
        std::string command = "/userdisk/dtpp/injector -n YoudaoDictPen /userdisk/dtpp/libdtpp.so";
        int result = system(command.c_str());
        inited=true;
    }

    o_get_system_info o_s = (o_get_system_info)dlsym(RTLD_NEXT, "get_system_info");
    return o_s(sn);
}
