
cmake_minimum_required(VERSION 3.4.1)

# 需要引入我们头文件,以这个配置的目录为基准
set( OpenCV_DIR F:/OpenCV-2.4.11-android-sdk/OpenCV-android-sdk/sdk/native/jni )

find_package(OpenCV REQUIRED )
if(OpenCV_FOUND)
    include_directories(${OpenCV_INCLUDE_DIRS})
    message(STATUS "OpenCV library status:")
    message(STATUS "    version: ${OpenCV_VERSION}")
    message(STATUS "    libraries: ${OpenCV_LIBS}")
    message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")
else(OpenCV_FOUND)
    message(FATAL_ERROR "OpenCV library not found")
endif(OpenCV_FOUND)


add_library(
        native-lib
        SHARED
        src/main/cpp/native-lib.cpp
        src/main/cpp/cv_helper.h
        src/main/cpp/cv_helper.cpp)


find_library(
        log-lib
        log)

target_link_libraries(
        native-lib
        ${OpenCV_LIBS}
        jnigraphics
        ${log-lib})