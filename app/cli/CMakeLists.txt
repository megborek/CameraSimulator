# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add executable for the CLI application
add_executable(CameraSimulator_CLI main_cli.cpp)

# Link core library and other dependencies
find_package(OpenCV REQUIRED)
find_package(CLI11 REQUIRED)
target_link_libraries(CameraSimulator_CLI core ${OpenCV_LIBS} CLI11::CLI11)
