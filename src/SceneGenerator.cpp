#include "SceneGenerator.h"

cv::Mat SceneGenerator::generateGradient(int width, int height)
{
    cv::Mat scene(height, width, CV_64FC1);
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            scene.at<double>(i, j) = static_cast<double>(i) / height;
        }
    }
    return scene;
}

// Other sample scene generation functions can be implemented here
