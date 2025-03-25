PROJECT(CompoISP)

CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

# ===== Step 1 : Set Cross Compiler Path =====

SET(_SDK_PATH_ "/hdd/System/RV1126/RV1126_RV1109_LINUX_SDK_V2.2.5.1_20230530/")
SET(_COMPILER_PATH_ "${_SDK_PATH_}buildroot/output/rockchip_rv1126_rv1109/host/")

SET(CMAKE_C_COMPILER ${_COMPILER_PATH_}bin/arm-linux-gnueabihf-gcc)
SET(CMAKE_CXX_COMPILER ${_COMPILER_PATH_}bin/arm-linux-gnueabihf-g++)

# ===== Step 2 : Set Flags =====

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s -O3 -lrt -pthread")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s -O3 -lrt -pthread")

# ===== Step 3 : Include & Lib =====

SET(_INCLUDE_PATH_ ${_COMPILER_PATH_}arm-buildroot-linux-gnueabihf/sysroot/usr/include/)
SET(_LIB_PATH_ ${_COMPILER_PATH_}arm-buildroot-linux-gnueabihf/sysroot/usr/lib/)

LINK_DIRECTORIES(
    ${_LIB_PATH_}
    ${_SDK_PATH_}buildroot/oem/usr/lib/
    ${_SDK_PATH_}buildroot/oem/ui/jp_ui2024011601/qt5_usr_lib/lib/
)

INCLUDE_DIRECTORIES(
    ${_INCLUDE_PATH_}
    ${_SDK_PATH_}external/rkmedia/examples/
    ${_SDK_PATH_}external/rkmedia/include/rkmedia/
    ${_SDK_PATH_}external/camera_engine_rkaiq/include/common
    ${_SDK_PATH_}external/camera_engine_rkaiq/include/uAPI/
    ${_SDK_PATH_}external/camera_engine_rkaiq/include/xcore/
    ${_SDK_PATH_}external/camera_engine_rkaiq/include/algos/
    ${_SDK_PATH_}external/camera_engine_rkaiq/include/iq_parser/
    ${_SDK_PATH_}external/linux-rga/include/
    ${_SDK_PATH_}external/isp2-ipc/interface/
    ${_SDK_PATH_}external/CallFunIpc
)

# ===== Step 4 : Set Application Name =====

SET(APP_NAME "compoISP")

# ===== Step 5 : Add Subdirectory =====

ADD_SUBDIRECTORY(src bin)

# ===== Step 6: Generate Version Info =====

# Branch
EXECUTE_PROCESS(
    COMMAND git rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Last commit ID
EXECUTE_PROCESS(
    COMMAND git rev-parse HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_ID
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# User
EXECUTE_PROCESS(
    COMMAND git config user.name
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_USER
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Git Clean
EXECUTE_PROCESS(
    COMMAND git diff-index --quiet HEAD --
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    RESULT_VARIABLE GIT_DIRTY   
)
IF(GIT_DIRTY EQUAL 0)
    SET(GIT_CLEAN "Clean")
ELSE()
    SET(GIT_CLEAN "Dirty")
ENDIF()

# Host
EXECUTE_PROCESS(
    COMMAND hostname
    OUTPUT_VARIABLE COMPILE_HOST
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# User
EXECUTE_PROCESS(
    COMMAND whoami
    OUTPUT_VARIABLE COMPILE_USER
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Time
STRING(TIMESTAMP COMPILE_TIME "%Y-%m-%d %H:%M:%S")

CONFIGURE_FILE(
    ${CMAKE_SOURCE_DIR}/others/version/version.h.in
    ${CMAKE_SOURCE_DIR}/others/version/version.h
)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/others/)