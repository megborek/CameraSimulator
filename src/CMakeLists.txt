# Add the source files for the ImageSensor library
add_library(ImageSensorLibrary ImageSensor.cpp)

# Link OpenCV to the ImageSensor library
target_link_libraries(ImageSensorLibrary PRIVATE ${OpenCV_LIBS})
