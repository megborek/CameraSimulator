#ifndef IMAGESENSOR_H
#define IMAGESENSOR_H

#include <opencv2/opencv.hpp>
#include <random>
#include <string>

class ImageSensor
{
public:
    // Enumeration for Color Filter Array pattern including a clear filter
    enum Color
    {
        RED,
        GREEN,
        BLUE,
        CLEAR
    };

    // Constructor: Initializes the sensor array and random noise generator with specified bit depth and dimensions
    ImageSensor(int bitDepth, int width, int height);

    /**
     * @brief Captures light from a scene into the sensor array.
     * @param scene cv::Mat representing the scene light intensity (single channel, double type).
     */
    void captureLight(const cv::Mat &scene);

    /**
     * @brief Adds Gaussian noise to the sensor data.
     * @param noiseLevel Standard deviation of the Gaussian noise to be added.
     */
    void addNoise(double noiseLevel);

    /**
     * @brief Applies a Color Filter Array (CFA) to the sensor data, including the option for clear pixels.
     * @param cfaPattern 2D array defining the CFA pattern (RED, GREEN, BLUE, CLEAR).
     */
    void applyCFA(const cv::Mat &cfaPattern);

    /**
     * @brief Demosaics the sensor data to produce a full-color image.
     * @param output cv::Mat to store the demosaiced image (3 channels, appropriate type based on bit depth).
     */
    void demosaic(cv::Mat &output);

    /**
     * @brief Simulates optical diffraction by applying a point spread function (PSF).
     * @param psf cv::Mat representing the point spread function.
     */
    void applyDiffraction(const cv::Mat &psf);

    /**
     * @brief Generates a CFA pattern matrix based on the input string.
     * @param patternString String representing the CFA pattern (e.g., "RCCB").
     * @param width Width of the CFA pattern.
     * @param height Height of the CFA pattern.
     * @return cv::Mat representing the CFA pattern.
     */
    static cv::Mat generateCFAPattern(const std::string &patternString, int width, int height);

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
