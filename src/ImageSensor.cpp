#include "ImageSensor.h"

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
    scene.convertTo(sensor, cvType, 65535.0); // Scale to full range for higher bit depths
}

// Add noise to the sensor
void ImageSensor::addNoise(double noiseLevel)
{
    cv::Mat noise(sensor.size(), CV_64FC1); // Always use double for noise generation
    std::normal_distribution<double> d(0, noiseLevel);
    for (int i = 0; i < noise.rows; ++i)
    {
        for (int j = 0; j < noise.cols; ++j)
        {
            noise.at<double>(i, j) = d(generator);
        }
    }

    cv::Mat temp;
    sensor.convertTo(temp, CV_64FC1); // Convert sensor to double for addition
    temp += noise;
    temp.convertTo(sensor, cvType); // Convert back to the original type
}

// Apply Color Filter Array with CLEAR pixel option
void ImageSensor::applyCFA(const cv::Mat &cfaPattern)
{
    cv::Mat temp;
    sensor.convertTo(temp, CV_64FC1); // Convert to double for processing
    for (int i = 0; i < temp.rows; ++i)
    {
        for (int j = 0; j < temp.cols; ++j)
        {
            Color color = static_cast<Color>(cfaPattern.at<uchar>(i, j));
            double value = temp.at<double>(i, j);
            switch (color)
            {
            case RED:
                value *= 0.299;
                break;
            case GREEN:
                value *= 0.587;
                break;
            case BLUE:
                value *= 0.114;
                break;
            case CLEAR:
                // No modification needed for clear pixels
                break;
            }
            temp.at<double>(i, j) = value;
        }
    }
    temp.convertTo(sensor, cvType); // Convert back to the original type
}

// Demosaic the sensor data
void ImageSensor::demosaic(cv::Mat &output)
{
    cv::Mat temp;
    sensor.convertTo(temp, CV_8UC1, 255.0 / 65535.0); // Scale down for demosaicing
    cv::cvtColor(temp, output, cv::COLOR_BayerBG2BGR);
    output.convertTo(output, cvType, 65535.0 / 255.0); // Scale up to original type
}

// Apply optical diffraction by convolving with a PSF
void ImageSensor::applyDiffraction(const cv::Mat &psf)
{
    cv::Mat temp;
    sensor.convertTo(temp, CV_64FC1);  // Convert to double for processing
    cv::filter2D(temp, temp, -1, psf); // Convolve with the PSF
    temp.convertTo(sensor, cvType);    // Convert back to the original type
}

// Generate CFA pattern matrix based on input string
cv::Mat ImageSensor::generateCFAPattern(const std::string &patternString, int width, int height)
{
    cv::Mat cfaPattern(height, width, CV_8UC1);
    int patternLength = patternString.length();
    if (patternLength != 4)
    {
        throw std::invalid_argument("CFA pattern string must be 4 characters long (2x2 pattern)");
    }

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            int patternIndex = (i % 2) * 2 + (j % 2);
            char patternChar = patternString[patternIndex];
            switch (patternChar)
            {
            case 'R':
                cfaPattern.at<uchar>(i, j) = ImageSensor::RED;
                break;
            case 'G':
                cfaPattern.at<uchar>(i, j) = ImageSensor::GREEN;
                break;
            case 'B':
                cfaPattern.at<uchar>(i, j) = ImageSensor::BLUE;
                break;
            case 'C':
                cfaPattern.at<uchar>(i, j) = ImageSensor::CLEAR;
                break;
            default:
                throw std::invalid_argument("Invalid CFA pattern character");
            }
        }
    }
    return cfaPattern;
}
