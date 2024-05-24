cmake_minimum_required(VERSION 3.10)

# Project name and version
project(ImageSensorSimulation VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenCV REQUIRED)
find_package(CLI11 REQUIRED)

# Include directories
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add subdirectories
add_subdirectory(src)
add_subdirectory(app)

# Define the executable
add_executable(ImageSensorSimulation app/main.cpp)

# Link libraries
target_link_libraries(ImageSensorSimulation 
PRIVATE 
ImageSensorLibrary
CFAPatternLibrary
SceneGeneratorLibrary
CLI11::CLI11
${OpenCV_LIBS}
${CONAN_LIBS}
)
