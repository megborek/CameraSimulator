#include "ImageSensor.h"
#include <iostream>

// Constructor with bit depth and dimensions parameters
ImageSensor::ImageSensor(int bitDepth, int width, int height)
    : bitDepth(bitDepth), width(width), height(height), distribution(0.0, 1.0)
{
    // Determine the OpenCV type based on the bit depth
    if (bitDepth == 8)
    {
        cvType = CV_8UC1;
    }
    else if (bitDepth == 32)
    {
        cvType = CV_32FC1;
    }
    else if (bitDepth == 64)
    {
        cvType = CV_64FC1;
    }
    else if (bitDepth == 10 || bitDepth == 12 || bitDepth == 14 || bitDepth == 16)
    {
        cvType = CV_16UC1;
    }
    else
    {
        throw std::invalid_argument("Unsupported bit depth");
    }

    // Initialize the sensor matrix with the determined type
    sensor = cv::Mat::zeros(height, width, cvType);
}

// Capture light into the sensor
void ImageSensor::captureLight(const cv::Mat &scene)
{
    if (bitDepth == 8)
    {
        scene.convertTo(sensor, cvType, 255.0); // Scale to 8-bit range
    }
    else
    {
        scene.convertTo(sensor, cvType, 65535.0); // Scale to full range for higher bit depths
    }
}

// Add noise to the sensor
void ImageSensor::addNoise(double noiseLevel)
{
    cv::Mat noise(sensor.size(), CV_64FC1);            // Create a noise matrix of the same size as the sensor
    std::normal_distribution<double> d(0, noiseLevel); // Normal distribution with mean 0 and standard deviation noiseLevel

    // Fill the noise matrix with random values from the normal distribution
    for (int i = 0; i < noise.rows; ++i)
    {
        for (int j = 0; j < noise.cols; ++j)
        {
            noise.at<double>(i, j) = d(generator);
        }
    }

    // Debug information to check the noise values
    double minNoise, maxNoise;
    cv::minMaxLoc(noise, &minNoise, &maxNoise);
    std::cout << "Noise min value: " << minNoise << ", max value: " << maxNoise << std::endl;

    // Print the first 10x10 section of the noise matrix
    std::cout << "Noise matrix (first 10x10 values):" << std::endl;
    for (int i = 0; i < std::min(10, noise.rows); ++i)
    {
        for (int j = 0; j < std::min(10, noise.cols); ++j)
        {
            std::cout << noise.at<double>(i, j) << " ";
        }
        std::cout << std::endl;
    }

    // Add the noise to the sensor data
    cv::Mat temp;
    sensor.convertTo(temp, CV_64FC1); // Convert the sensor data to double for precision

    // Print the first 10x10 section of the image matrix
    std::cout << "Image matrix (first 10x10 values):" << std::endl;
    for (int i = 0; i < std::min(10, temp.rows); ++i)
    {
        for (int j = 0; j < std::min(10, temp.cols); ++j)
        {
            std::cout << temp.at<double>(i, j) << " ";
        }
        std::cout << std::endl;
    }

    temp += noise; // Add the noise to the sensor data

    std::cout << "Image matrix (first 10x10 values):" << std::endl;
    for (int i = 0; i < std::min(10, temp.rows); ++i)
    {
        for (int j = 0; j < std::min(10, temp.cols); ++j)
        {
            std::cout << temp.at<double>(i, j) << " ";
        }
        std::cout << std::endl;
    }
    temp.convertTo(sensor, cvType); // Convert back to the original type
}

// Apply Color Filter Array with CLEAR pixel option
void ImageSensor::applyCFA(const CFAPattern &cfaPattern)
{
    cv::Mat temp;
    sensor.convertTo(temp, CV_64FC1); // Convert to double for processing
    const cv::Mat &pattern = cfaPattern.getPattern();
    for (int i = 0; i < temp.rows; ++i)
    {
        for (int j = 0; j < temp.cols; ++j)
        {
            CFAPattern::Color color = static_cast<CFAPattern::Color>(pattern.at<uchar>(i, j));
            double value = temp.at<double>(i, j);
            switch (color)
            {
            case CFAPattern::RED:
                value *= cfaPattern.getColorWeight(CFAPattern::RED);
                break;
            case CFAPattern::GREEN:
                value *= cfaPattern.getColorWeight(CFAPattern::GREEN);
                break;
            case CFAPattern::BLUE:
                value *= cfaPattern.getColorWeight(CFAPattern::BLUE);
                break;
            case CFAPattern::CLEAR:
                // No modification needed for clear pixels
                break;
            }
            temp.at<double>(i, j) = value;
        }
    }
    temp.convertTo(sensor, cvType); // Convert back to the original type
}

// Demosaic the sensor data
void ImageSensor::demosaic(cv::Mat &output, const std::string &cfaPatternStr)
{
    cv::Mat temp;
    sensor.convertTo(temp, CV_8UC1, 255.0 / ((bitDepth == 8) ? 255.0 : 65535.0)); // Scale down for demosaicing

    int conversionCode;
    if (cfaPatternStr == "RCCB" || cfaPatternStr == "RGGB")
    {
        conversionCode = cv::COLOR_BayerBG2BGR;
    }
    else if (cfaPatternStr == "GRBG")
    {
        conversionCode = cv::COLOR_BayerGR2BGR;
    }
    else if (cfaPatternStr == "GBRG")
    {
        conversionCode = cv::COLOR_BayerGB2BGR;
    }
    else if (cfaPatternStr == "BGGR")
    {
        conversionCode = cv::COLOR_BayerRG2BGR;
    }
    else
    {
        std::cerr << "Warning: Unsupported CFA pattern string for demosaicing. Defaulting to cv::COLOR_BayerBG2BGR." << std::endl;
        conversionCode = cv::COLOR_BayerBG2BGR;
    }

    cv::cvtColor(temp, output, conversionCode);
    output.convertTo(output, cvType, ((bitDepth == 8) ? 255.0 : 65535.0) / 255.0); // Scale up to original type
}

// Apply optical diffraction by convolving with a PSF
void ImageSensor::applyDiffraction(const cv::Mat &psf)
{
    cv::Mat temp;
    sensor.convertTo(temp, CV_64FC1);  // Convert to double for processing
    cv::filter2D(temp, temp, -1, psf); // Convolve with the PSF
    temp.convertTo(sensor, cvType);    // Convert back to the original type
}
