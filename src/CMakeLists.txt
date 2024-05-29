# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add core source files
set(CORE_SOURCES
    ImageSensor.cpp
    CFAPattern.cpp
    SceneGenerator.cpp
    ISP.cpp
)

set(CORE_HEADERS
    ${CMAKE_SOURCE_DIR}/include/ImageSensor/ImageSensor.h
    ${CMAKE_SOURCE_DIR}/include/CFAPattern/CFAPattern.h
    ${CMAKE_SOURCE_DIR}/include/SceneGenerator/SceneGenerator.h
    ${CMAKE_SOURCE_DIR}/include/ISP/ISP.h
)

# Create a library for core components
add_library(core ${CORE_SOURCES} ${CORE_HEADERS})

# Link necessary libraries to the core
find_package(OpenCV REQUIRED COMPONENTS core highgui imgproc photo)
target_link_libraries(core ${OpenCV_LIBS})
