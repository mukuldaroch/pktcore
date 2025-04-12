cmake_minimum_required(VERSION 3.10)
project(pktcore)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include your include/ directory for headers
set(HEADER_FILES
    include/full_header.h
    include/mini_header.h
    include/splitter.h
    include/combiner.h
)

# Find OS-specific dependencies
if(WIN32)
    set(SYSTEM_LIBS ws2_32)  # Windows networking library (if needed)
elseif(UNIX)
    set(SYSTEM_LIBS pthread)  # Linux threading
endif()

# Source files
set(SOURCES
    src/main.cpp
    # src/splitter.cpp
    # src/combiner.cpp
)

# Add executable
add_executable(pktcore ${SOURCES} ${HEADER_FILES})

# Link system libraries
target_include_directories(pktcore PRIVATE ${CMAKE_SOURCE_DIR}/include)
