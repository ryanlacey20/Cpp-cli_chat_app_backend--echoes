cmake_minimum_required(VERSION 3.30) 
project(ECHOES_BACKEND)

# Set C++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_PREFIX_PATH "third_party/boost_1_86_0")


set(CMAKE_TOOLCHAIN_FILE "/Users/ryanlacey/Downloads/vcpkg/scripts/buildsystems/vcpkg.cmake")

find_package(CURL REQUIRED)
find_package(ODBC REQUIRED)


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
)

# # Include directories for asio (added stand alone outside of boost)
# target_include_directories(ECHOES_BACKEND PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party/asio/include)

# Include directories for Crow 
target_include_directories(ECHOES_BACKEND PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party/crow)


