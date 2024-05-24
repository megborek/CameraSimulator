# Add the source files for the ImageSensor and CFAPattern libraries
add_library(ImageSensorLibrary ImageSensor.cpp)
add_library(CFAPatternLibrary CFAPattern.cpp)
add_library(SceneGeneratorLibrary SceneGenerator.cpp)
add_library(ISPLibrary ISP.cpp)

# Link OpenCV to the ImageSensor and CFAPattern libraries
target_link_libraries(ImageSensorLibrary PRIVATE ${OpenCV_LIBS})
target_link_libraries(CFAPatternLibrary PRIVATE ${OpenCV_LIBS})
target_link_libraries(SceneGeneratorLibrary PRIVATE ${OpenCV_LIBS})
target_link_libraries(ISPLibrary PRIVATE ${OpenCV_LIBS})