cmake_minimum_required(VERSION 3.30) 
project(ECHOES_BACKEND)

# Set C++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_TOOLCHAIN_FILE "/Users/ryanlacey/Downloads/vcpkg/scripts/buildsystems/vcpkg.cmake")

if(WIN32)
    set(CMAKE_TOOLCHAIN_FILE "C:/Program Files/vcpkg/scripts/buildsystems/vcpkg.cmake")
    set(Boost_DIR "C:/Users/tadhg/OneDrive/Desktop/boost_install/boots-test/boost_1_86_0/stage/lib/cmake/Boost-1.86.0")
    set(CURL_INCLUDE_DIR "C:/Users/tadhg/OneDrive/Desktop/boost_install/curl/include")
    set(CURL_LIBRARY "C:/Users/tadhg/OneDrive/Desktop/boost_install/curl/lib/libcurl.a")
    set(nlohmann_json_DIR "C:/Program Files/vcpkg/installed/x64-windows/share/nlohmann_json")
    #target_include_directories(ECHOES_BACKEND PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party/asio/include)
    
endif()


find_package(CURL REQUIRED)
find_package(ODBC REQUIRED)
find_package(nlohmann_json REQUIRED)


# Find and include Boost
find_package(Boost REQUIRED COMPONENTS system filesystem)


# Define the executable
add_executable(ECHOES_BACKEND src/main.cpp src/server.cpp src/dbConnection.cpp)

target_link_libraries(ECHOES_BACKEND
    PRIVATE
    Boost::system
    Boost::filesystem
    #Boost::beast
    CURL::libcurl
    ODBC::ODBC
    nlohmann_json::nlohmann_json
)

# # Include directories for asio (added stand alone outside of boost)
# target_include_directories(ECHOES_BACKEND PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party/asio/include)

# Include directories for Crow 
target_include_directories(ECHOES_BACKEND PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party/crow)


