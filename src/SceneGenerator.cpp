#include "SceneGenerator/SceneGenerator.h"

cv::Mat SceneGenerator::generateGradient(int width, int height)
{
    cv::Mat gradient(height, width, CV_64FC1);

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            gradient.at<double>(i, j) = static_cast<double>(i) / height;
        }
    }

    return gradient;
}

cv::Mat SceneGenerator::generateCheckerboard(int width, int height, int squaresPerRow, int squaresPerCol)
{
    cv::Mat checkerboard(height, width, CV_64FC1);

    int squareWidth = width / squaresPerRow;
    int squareHeight = height / squaresPerCol;

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            int row = i / squareHeight;
            int col = j / squareWidth;
            if ((row + col) % 2 == 0)
            {
                checkerboard.at<double>(i, j) = 1.0; // White
            }
            else
            {
                checkerboard.at<double>(i, j) = 0.0; // Black
            }
        }
    }

    return checkerboard;
}

cv::Mat SceneGenerator::generateSlantedEdge(int width, int height, double angle)
{
    cv::Mat slantedEdge(height, width, CV_64FC1, cv::Scalar(0));

    // Calculate the slant line
    double radians = angle * CV_PI / 180.0;
    double tanAngle = tan(radians);

    int centerX = width / 2;
    int centerY = height / 2;

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            double relativeX = x - centerX;
            double relativeY = y - centerY;

            double edgePosition = relativeY * tanAngle;

            // Apply the sharp light-to-dark transition
            if (relativeX <= edgePosition)
            {
                slantedEdge.at<double>(y, x) = 1.0; // Light side
            }
            else
            {
                slantedEdge.at<double>(y, x) = 0.0; // Dark side
            }
        }
    }

    return slantedEdge;
}

cv::Mat SceneGenerator::generateRadialLines(int width, int height, int numLines)
{
    cv::Mat radialLines(height, width, CV_64FC1, cv::Scalar(0));

    cv::Point center(width / 2, height / 2);
    double angleStep = 360.0 / numLines;

    for (int i = 0; i < numLines; ++i)
    {
        double angle = i * angleStep * CV_PI / 180.0;
        int x = static_cast<int>(center.x + width * cos(angle));
        int y = static_cast<int>(center.y + height * sin(angle));

        cv::line(radialLines, center, cv::Point(x, y), cv::Scalar(1), 1);
    }

    return radialLines;
}
