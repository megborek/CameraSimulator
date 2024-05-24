#ifndef ISP_H
#define ISP_H

#include <opencv2/opencv.hpp>

class ISP
{
public:
    /**
     * @brief Performs auto white balance on the input image.
     * @param input Input image in the Bayer domain.
     * @return White balanced image.
     */
    static cv::Mat autoWhiteBalance(const cv::Mat &input);

    /**
     * @brief Performs denoising on the input image.
     * @param input Input image in the Bayer domain.
     * @return Denoised image.
     */
    static cv::Mat denoise(const cv::Mat &input);

    /**
     * @brief Performs black level compensation on the input image.
     * @param input Input image in the Bayer domain.
     * @param blackLevel Black level value to be subtracted.
     * @return Image with black level compensation.
     */
    static cv::Mat blackLevelCompensation(const cv::Mat &input, double blackLevel);

    /**
     * @brief Performs lens shading correction on the input image.
     * @param input Input image in the Bayer domain.
     * @param shadingMap Shading map for correction.
     * @return Lens shading corrected image.
     */
    static cv::Mat lensShadingCorrection(const cv::Mat &input, const cv::Mat &shadingMap);
};

#endif // ISP_H
