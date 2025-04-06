cmake_minimum_required(VERSION 3.10)
project(AITutoringCRM VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(${CMAKE_SOURCE_DIR})

# Create folders if they dont exist
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/logs)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/data)

# Source files
set(SOURCES
    main.cpp
    src/models/User.cpp
    src/models/Admin.cpp
    src/models/Tutor.cpp
    src/models/AIModel.cpp
    src/models/Client.cpp
    src/services/AuthService.cpp
    src/services/AIModelService.cpp
    src/services/ClientService.cpp
)

# Add executable
add_executable(AITutoringCRM ${SOURCES})

# Link any needed libraries
# target_link_libraries(AITutoringCRM ...)

# Installation rules
install(TARGETS AITutoringCRM DESTINATION bin)

# Output message
message(STATUS "CMake configured for AI Tutoring CRM System")