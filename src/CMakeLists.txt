FILE(
    GLOB_RECURSE SRC_LIST 
    ./*.c
    ./*.cpp
)

# Exe output path
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

ADD_EXECUTABLE(${APP_NAME} ${SRC_LIST})

# Link lib and so
TARGET_LINK_LIBRARIES(
    ${APP_NAME}
    libeasymedia.so
    libsample_common_isp.so
    librga.so
    librkaiq.so
    libCallFunIpc.so
    libjson-c.so
    libmicrohttpd.so
)