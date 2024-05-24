#ifndef SCENEGENERATOR_H
#define SCENEGENERATOR_H

#include <opencv2/opencv.hpp>

class SceneGenerator
{
public:
    // Default values
    static constexpr int DEFAULT_WIDTH = 640;
    static constexpr int DEFAULT_HEIGHT = 480;

    /**
     * @brief Generates a gradient image.
     * @param width Width of the image.
     * @param height Height of the image.
     * @return cv::Mat representing the gradient image.
     */
    static cv::Mat generateGradient(int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT);

    // Other sample scene generation functions can be declared here
};

#endif // SCENEGENERATOR_H
