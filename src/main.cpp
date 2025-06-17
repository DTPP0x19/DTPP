#include "tlog.h"
#include <iostream>
#include "hook.h"

__attribute__((constructor)) void init_dtpp()
{
    // 创建并打开一个文件用于写入
    tlog_init("/tmp/dtpp.log", 1024 * 1024, 2, 0, 0);
    tlog_info("👋 DTPP 0.1 Starting!");

    load_resources();
    hook();
}
