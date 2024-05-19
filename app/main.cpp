#include <iostream>
#include <opencv2/opencv.hpp>
#include "ImageSensor.h"

int main()
{
    // Define the dimensions of the scene
    const int width = ImageSensor::width;
    const int height = ImageSensor::height;

    // Create a sample scene (a simple gradient image for demonstration)
    cv::Mat scene(height, width, CV_64FC1);
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            scene.at<double>(i, j) = static_cast<double>(i) / height;
        }
    }

    // Define a sample CFA pattern (RCCB)
    cv::Mat cfaPattern(height, width, CV_8UC1);
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            if ((i % 2 == 0) && (j % 2 == 0))
            {
                cfaPattern.at<uchar>(i, j) = ImageSensor::RED;
            }
            else if ((i % 2 == 0) && (j % 2 == 1))
            {
                cfaPattern.at<uchar>(i, j) = ImageSensor::CLEAR;
            }
            else if ((i % 2 == 1) && (j % 2 == 0))
            {
                cfaPattern.at<uchar>(i, j) = ImageSensor::CLEAR;
            }
            else
            {
                cfaPattern.at<uchar>(i, j) = ImageSensor::BLUE;
            }
        }
    }

    // Create an ImageSensor object with the desired bit depth
    int bitDepth = 16; // You can change this to 8, 10, 12, 14, 16, 32, or 64
    ImageSensor sensor(bitDepth);

    // Simulate capturing light from the scene
    sensor.captureLight(scene);

    // Simulate optical diffraction by applying a Gaussian PSF
    cv::Mat psf = cv::getGaussianKernel(7, 1.5, CV_64F) * cv::getGaussianKernel(7, 1.5, CV_64F).t();
    sensor.applyDiffraction(psf);

    // Add noise to the sensor data
    sensor.addNoise(0.05);

    // Apply the CFA pattern
    sensor.applyCFA(cfaPattern);

    // Demosaic the sensor data to produce a full-color image
    cv::Mat output;
    sensor.demosaic(output);

    // Normalize the output image to 8-bit range for display if the bit depth is greater than 8
    cv::Mat output8U;
    if (bitDepth > 8)
    {
        double minVal, maxVal;
        cv::minMaxLoc(output, &minVal, &maxVal);
        output.convertTo(output8U, CV_8UC3, 255.0 / maxVal);
    }
    else
    {
        output.convertTo(output8U, CV_8UC3);
    }

    // Display the original scene and the simulated sensor output
    cv::imshow("Original Scene", scene);
    cv::imshow("Simulated Sensor Output", output8U);

    // Save the images for further inspection
    cv::imwrite("scene.png", scene * 255);      // Save the scene image as grayscale
    cv::imwrite("sensor_output.png", output8U); // Save the simulated output image

    // Wait for a key press indefinitely
    cv::waitKey(0);

    return 0;
}
