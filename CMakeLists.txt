cmake_minimum_required(VERSION 3.10)
project(CameraSimulator)

set(CMAKE_CXX_STANDARD 17)

# Add subdirectories for source and applications
add_subdirectory(src)
add_subdirectory(app/cli)
