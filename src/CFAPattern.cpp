#include "CFAPattern.h"

CFAPattern::CFAPattern(const std::string &patternString, int width, int height)
    : cfaPattern(height, width, CV_8UC1)
{
    // Initialize default color weights
    colorWeights[RED] = 0.299;
    colorWeights[GREEN] = 0.587;
    colorWeights[BLUE] = 0.114;
    colorWeights[CLEAR] = 1.0;

    // Validate the pattern string length
    if (patternString.length() != 4)
    {
        throw std::invalid_argument("CFA pattern string must be 4 characters long (2x2 pattern)");
    }

    // Populate the CFA pattern matrix
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            int patternIndex = (i % 2) * 2 + (j % 2);
            char patternChar = patternString[patternIndex];
            switch (patternChar)
            {
            case 'R':
                cfaPattern.at<uchar>(i, j) = RED;
                break;
            case 'G':
                cfaPattern.at<uchar>(i, j) = GREEN;
                break;
            case 'B':
                cfaPattern.at<uchar>(i, j) = BLUE;
                break;
            case 'C':
                cfaPattern.at<uchar>(i, j) = CLEAR;
                break;
            default:
                throw std::invalid_argument("Invalid CFA pattern character");
            }
        }
    }
}

const cv::Mat &CFAPattern::getPattern() const
{
    return cfaPattern;
}

void CFAPattern::setColorWeight(Color color, double weight)
{
    colorWeights[color] = weight;
}

double CFAPattern::getColorWeight(Color color) const
{
    return colorWeights.at(color);
}

bool CFAPattern::isValidCFAPatternChar(char ch) const
{
    return ch == 'R' || ch == 'G' || ch == 'B' || ch == 'C';
}
