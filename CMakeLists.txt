cmake_minimum_required(VERSION 3.30) # Use a valid CMake version. CMake 3.30 does not exist.
project(ECHOES_BACKEND)

# Set C++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)

    set(Boost_DIR "C:/Users/tadhg/OneDrive/Desktop/boost_install/boots-test/boost_1_86_0/stage/lib/cmake/Boost-1.86.0")
    set(CURL_INCLUDE_DIR "C:/Users/tadhg/OneDrive/Desktop/boost_install/curl/include")
    set(CURL_LIBRARY "C:/Users/tadhg/OneDrive/Desktop/boost_install/curl/lib/libcurl.a")
    
endif()

# Find and include Boost
find_package(Boost REQUIRED COMPONENTS system filesystem)

# Check if Boost was found
if(Boost_FOUND)
    message(STATUS "Boost found: ${Boost_INCLUDE_DIRS}")
else()
    message(FATAL_ERROR "Boost not found")
endif()


# Define the executable
add_executable(ECHOES_BACKEND src/main.cpp)

# Link Boost libraries to the executable, others aren't here because theyre 'Header only libs'
target_link_libraries(ECHOES_BACKEND
    PRIVATE
    Boost::system
    Boost::filesystem
)

# Include directories for asio (added stand alone outside of boost)
target_include_directories(ECHOES_BACKEND PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party/asio/include)

# Include directories for Crow 
target_include_directories(ECHOES_BACKEND PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party/crow)


