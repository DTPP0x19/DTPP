cmake_minimum_required(VERSION 3.13)

# 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 17)

# cross compile
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# Dobby
project(dtpp VERSION 1.0 LANGUAGES CXX)
#add_subdirectory(${PROJECT_SOURCE_DIR}/lib/Dobby)
# Fix pthread include in aarch64 architechture
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads)
link_libraries(${CMAKE_THREAD_LIBS_INIT})

include_directories(${PROJECT_SOURCE_DIR}/include)
link_libraries(${PROJECT_SOURCE_DIR}/lib/Dobby/build/cmake-build-linux-arm64/libdobby.a)

# Qt 配置
# 生成可执行文件

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC OFF)

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Core Quick LinguistTools)
find_package(Qt5 REQUIRED COMPONENTS Core Quick LinguistTools)

set(TS_FILES assets/dtpp-qt_zh_CN.ts)
set(DTPP_UI_SOURCES
        src/main.cpp
        src/symbol_cache.cpp
        lib/tlog/tlog.cpp
        src/hook.cpp
        src/llmchat.cpp
        src/fio.cpp
        assets/qml.qrc
)
add_library(dtpp SHARED ${DTPP_UI_SOURCES})
# 加载器与主程序分离以减少LD_PRELOAD带来的性能开销
set(DTPP_LOADER
        loader/loader.cpp
)
add_library(dtppl SHARED ${DTPP_LOADER})
# 优化加载器大小
set_target_properties(dtppl PROPERTIES COMPILE_FLAGS "-Ofast")
target_link_options(dtppl PRIVATE -s -Wl,--gc-sections -Wl,--strip-all)

target_link_libraries(dtpp Qt5::Core Qt5::Quick -ldl)

include(GNUInstallDirs)
#qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

# DTPP
link_libraries(dl)



