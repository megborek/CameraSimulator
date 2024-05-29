#include <opencv2/opencv.hpp>
#include "ISP/ISP.h"

cv::Mat ISP::autoWhiteBalance(const cv::Mat &input)
{
    // Convert input image to float type
    cv::Mat floatImage;
    input.convertTo(floatImage, CV_32FC3);

    // Compute the average color per channel
    cv::Scalar avgColor = cv::mean(floatImage);

    // Compute scaling factors
    double avgGray = (avgColor[0] + avgColor[1] + avgColor[2]) / 3.0;
    cv::Scalar scaleFactors = avgGray / avgColor;

    // Apply scaling factors to each channel
    std::vector<cv::Mat> channels(3);
    cv::split(floatImage, channels);
    channels[0] *= scaleFactors[0];
    channels[1] *= scaleFactors[1];
    channels[2] *= scaleFactors[2];
    cv::merge(channels, floatImage);

    // Convert back to the original type
    cv::Mat balanced;
    floatImage.convertTo(balanced, input.type());

    return balanced;
}

cv::Mat ISP::denoise(const cv::Mat &input)
{
    // Convert input image to CV_8UC3
    cv::Mat input8UC3;
    input.convertTo(input8UC3, CV_8UC3, 1.0 / 256.0); // Assuming the input is 16-bit and needs to be scaled down

    cv::Mat denoised;
    cv::fastNlMeansDenoisingColored(input8UC3, denoised);

    // Convert back to the original bit depth
    cv::Mat output;
    denoised.convertTo(output, input.type(), 256.0); // Scaling back to 16-bit if needed

    return output;
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
    cv::divide(input, shadingMap, corrected, 1.0, input.type());
    return corrected;
}
