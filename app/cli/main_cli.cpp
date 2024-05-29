#include <iostream>
#include <opencv2/opencv.hpp>
#include <CLI/CLI.hpp>
#include "ImageSensor/ImageSensor.h"
#include "CFAPattern/CFAPattern.h"
#include "SceneGenerator/SceneGenerator.h"
#include "ISP/ISP.h"

int main(int argc, char **argv)
{
    // Parse command-line arguments using CLI11
    CLI::App app{"Image Sensor Simulation"};

    int width = SceneGenerator::DEFAULT_WIDTH;
    int height = SceneGenerator::DEFAULT_HEIGHT;
    int bitDepth = ImageSensor::DEFAULT_BIT_DEPTH;
    double noiseLevel = 0.5;
    std::string cfaPatternStr = CFAPattern::DEFAULT_CFA_PATTERN;
    std::vector<std::string> colorWeights;
    std::string patternType = "gradient"; // Default pattern type

    app.add_option("-w,--width", width, "Sensor width in pixels")->default_val(width);
    app.add_option("-j,--height", height, "Sensor height in pixels")->default_val(height);
    app.add_option("-b,--bitdepth", bitDepth, "Bit depth of the sensor")->default_val(bitDepth);
    app.add_option("-n,--noise", noiseLevel, "Noise level (standard deviation of Gaussian noise)")->default_val(noiseLevel);
    app.add_option("-c,--cfapattern", cfaPatternStr, "CFA pattern (e.g., RCCB)")->default_val(cfaPatternStr);
    app.add_option("--color-weight", colorWeights, "Change existing color weight (e.g., R:0.25)");
    app.add_option("-p,--pattern", patternType, "Pattern type (gradient, checkerboard, slanted-edge, radial-lines)")->default_val(patternType);

    CLI11_PARSE(app, argc, argv);

    // Create the CFA pattern object
    CFAPattern cfaPattern(cfaPatternStr, width, height);
    cfaPattern.updateColorWeights(colorWeights);

    // Create a sample scene based on the specified pattern type
    cv::Mat scene;
    if (patternType == "gradient")
    {
        scene = SceneGenerator::generateGradient(width, height);
    }
    else if (patternType == "checkerboard")
    {
        scene = SceneGenerator::generateCheckerboard(width, height, 8, 8);
    }
    else if (patternType == "slanted-edge")
    {
        scene = SceneGenerator::generateSlantedEdge(width, height, 20.0);
    }
    else if (patternType == "radial-lines")
    {
        scene = SceneGenerator::generateRadialLines(width, height, 16);
    }
    else
    {
        std::cerr << "Unknown pattern type: " << patternType << std::endl;
        return 1;
    }

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

    // Perform ISP operations
    // output = ISP::autoWhiteBalance(output);
    // output = ISP::denoise(output);
    // output = ISP::blackLevelCompensation(output, 64);            // Example black level value
    // cv::Mat shadingMap = cv::Mat::ones(height, width, CV_64FC1); // Example shading map (needs to be customized)
    // output = ISP::lensShadingCorrection(output, shadingMap);

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
