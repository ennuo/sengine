cmake_minimum_required(VERSION 3.25.2)

set(CMAKE_POLICY_DEFAULT_CMP0048 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

# Zlib
set(SKIP_INSTALL_ALL ON)
add_subdirectory(zlib)

# Set variables for find_package(ZLIB) since it doesn't seem to work when built manually?
# Unless I'm just missing something.
set(ZLIB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/zlib" "${CMAKE_CURRENT_BINARY_DIR}/zlib")
set(ZLIB_LIBRARY zlibstatic)
set(ZLIB_FOUND TRUE)
set(ZLIB_LIBRARIES zlibstatic)

# Assimp
set(ASSIMP_BUILD_ZLIB OFF)
set(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(assimp)

# Crossguid
set(CROSSGUID_TESTS)
add_subdirectory(crossguid)

# Other
add_subdirectory(glm)
add_subdirectory(stb)
add_subdirectory(glad)
add_subdirectory(entt)
add_subdirectory(nativefiledialog-extended)
add_subdirectory(imgui)
add_subdirectory(fmt)
add_subdirectory(easyloggingpp)

# Link everything into a single interface library
add_library(thirdparty INTERFACE)
target_include_directories(thirdparty INTERFACE 
    .
    base64 
    boost 
    dlg
    freetype
)
target_link_libraries(thirdparty INTERFACE
    assimp
    glm
    stb
    glad
    EnTT
    crossguid
    nfd
    imgui
    zlibstatic
    fmt
    easyloggingpp
)