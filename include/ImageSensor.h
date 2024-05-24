#ifndef IMAGESENSOR_H
#define IMAGESENSOR_H

#include <opencv2/opencv.hpp>
#include <random>
#include "CFAPattern.h"

class ImageSensor
{
public:
    // Default values
    static constexpr int DEFAULT_BIT_DEPTH = 16;
    static constexpr int DEFAULT_WIDTH = 640;
    static constexpr int DEFAULT_HEIGHT = 480;
    static constexpr double DEFAULT_NOISE_LEVEL = 0.05;

    // Constructor: Initializes the sensor array and random noise generator with specified bit depth and dimensions
    ImageSensor(int bitDepth = DEFAULT_BIT_DEPTH, int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT);

    /**
     * @brief Captures light from a scene into the sensor array.
     * @param scene cv::Mat representing the scene light intensity (single channel, double type).
     */
    void captureLight(const cv::Mat &scene);

    /**
     * @brief Adds Gaussian noise to the sensor data.
     * @param noiseLevel Standard deviation of the Gaussian noise to be added.
     */
    void addNoise(double noiseLevel = DEFAULT_NOISE_LEVEL);

    /**
     * @brief Applies a Color Filter Array (CFA) to the sensor data, including the option for clear pixels.
     * @param cfaPattern CFAPattern object defining the CFA pattern.
     */
    void applyCFA(const CFAPattern &cfaPattern);

    /**
     * @brief Demosaics the sensor data to produce a full-color image.
     * @param output cv::Mat to store the demosaiced image (3 channels, appropriate type based on bit depth).
     * @param cfaPatternStr The CFA pattern string.
     */
    void demosaic(cv::Mat &output, const std::string &cfaPatternStr);

    /**
     * @brief Simulates optical diffraction by applying a point spread function (PSF).
     * @param psf cv::Mat representing the point spread function.
     */
    void applyDiffraction(const cv::Mat &psf);

private:
    cv::Mat sensor;                                // Sensor data array
    std::default_random_engine generator;          // Random number generator for noise
    std::normal_distribution<double> distribution; // Gaussian distribution for noise
    int bitDepth;                                  // Bit depth of the sensor
    int cvType;                                    // OpenCV type corresponding to the bit depth
    int width;                                     // Width of the sensor
    int height;                                    // Height of the sensor
};

#endif // IMAGESENSOR_H
