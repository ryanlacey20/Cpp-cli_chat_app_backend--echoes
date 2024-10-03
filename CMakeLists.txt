cmake_minimum_required(VERSION 3.30) 
project(ECHOES_BACKEND)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(unixodbc_DIR "/Users/ryanlacey/Downloads/vcpkg/ports/unixodbc")


# Find and include Boost
find_package(Boost REQUIRED COMPONENTS system filesystem)
#find_package(unixodbc REQUIRED)

# Check if Boost was found
if(Boost_FOUND)
    message(STATUS "Boost found: ${Boost_INCLUDE_DIRS}")
else()
    message(FATAL_ERROR "Boost not found")
endif()



add_executable(ECHOES_BACKEND src/main.cpp src/database.cpp)

# Link Boost libraries to the executable, others aren't here because theyre 'Header only libs'
target_link_libraries(ECHOES_BACKEND
    PRIVATE
    Boost::system
    Boost::filesystem
    #UNIX::odbc
)


# Include directories for asio (added stand alone outside of boost)
target_include_directories(ECHOES_BACKEND PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party/asio/include)

# Include directories for Crow 
target_include_directories(ECHOES_BACKEND PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party/crow)


