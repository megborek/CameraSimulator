#include "ISP.h"
#include <opencv2/photo.hpp> // For denoising functions

cv::Mat ISP::autoWhiteBalance(const cv::Mat &input)
{
    cv::Mat balanced;
    cv::xphoto::createSimpleWB()->balanceWhite(input, balanced);
    return balanced;
}

cv::Mat ISP::denoise(const cv::Mat &input)
{
    cv::Mat denoised;
    cv::fastNlMeansDenoising(input, denoised, 30.0, 7, 21);
    return denoised;
}

cv::Mat ISP::blackLevelCompensation(const cv::Mat &input, double blackLevel)
{
    cv::Mat compensated;
    input.convertTo(compensated, -1, 1, -blackLevel);
    return compensated;
}

cv::Mat ISP::lensShadingCorrection(const cv::Mat &input, const cv::Mat &shadingMap)
{
    cv::Mat corrected;
    cv::divide(input, shadingMap, corrected, 1, CV_64FC1);
    corrected.convertTo(corrected, input.type());
    return corrected;
}
