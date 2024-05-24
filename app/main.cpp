#include <iostream>
#include <opencv2/opencv.hpp>
#include <CLI/CLI.hpp>
#include "ImageSensor.h"
#include "CFAPattern.h"
#include "SceneGenerator.h"

int main(int argc, char **argv)
{
    // Parse command-line arguments using CLI11
    CLI::App app{"Image Sensor Simulation"};

    int width = ImageSensor::DEFAULT_WIDTH;
    int height = ImageSensor::DEFAULT_HEIGHT;
    int bitDepth = ImageSensor::DEFAULT_BIT_DEPTH;
    double noiseLevel = 0.5; // Set a higher noise level for testing
    std::string cfaPatternStr = CFAPattern::DEFAULT_CFA_PATTERN;
    std::vector<std::string> colorWeights;

    app.add_option("-w,--width", width, "Sensor width in pixels")->default_val(width);
    app.add_option("-j,--height", height, "Sensor height in pixels")->default_val(height);
    app.add_option("-b,--bitdepth", bitDepth, "Bit depth of the sensor")->default_val(bitDepth);
    app.add_option("-n,--noise", noiseLevel, "Noise level (standard deviation of Gaussian noise)")->default_val(noiseLevel);
    app.add_option("-c,--cfapattern", cfaPatternStr, "CFA pattern (e.g., RCCB)")->default_val(cfaPatternStr);
    app.add_option("--color-weight", colorWeights, "Change existing color weight (e.g., R:0.25)");

    CLI11_PARSE(app, argc, argv);

    // Create the CFA pattern object
    CFAPattern cfaPattern(cfaPatternStr, width, height);
    cfaPattern.updateColorWeights(colorWeights);

    // Create a sample scene (a gradient image) using SceneGenerator
    cv::Mat scene = SceneGenerator::generateGradient(width, height);

    // Create an ImageSensor object with the desired bit depth and dimensions
    ImageSensor sensor(bitDepth, width, height);
    sensor.captureLight(scene);

    // Simulate optical diffraction by applying a Gaussian PSF
    cv::Mat psf = cv::getGaussianKernel(7, 1.5, CV_64F) * cv::getGaussianKernel(7, 1.5, CV_64F).t();
    sensor.applyDiffraction(psf);

    // Add noise to the sensor data
    sensor.addNoise(noiseLevel);

    // Apply the custom CFA pattern
    sensor.applyCFA(cfaPattern);

    // Demosaic the sensor data to produce a full-color image
    cv::Mat output;
    sensor.demosaic(output, cfaPatternStr);

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
