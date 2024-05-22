#ifndef CFAPATTERN_H
#define CFAPATTERN_H

#include <opencv2/opencv.hpp>
#include <string>
#include <map>

class CFAPattern
{
public:
    // Enumeration for default Color Filter Array patterns
    enum Color
    {
        RED,
        GREEN,
        BLUE,
        CLEAR
    };

    // Constructor: Initializes the CFA pattern matrix based on the input string and dimensions
    CFAPattern(const std::string &patternString, int width, int height);

    /**
     * @brief Gets the CFA pattern matrix.
     * @return cv::Mat representing the CFA pattern.
     */
    const cv::Mat &getPattern() const;

    /**
     * @brief Sets the weight for an existing color.
     * @param color The color to set the weight for.
     * @param weight The new weight for the color.
     */
    void setColorWeight(Color color, double weight);

    /**
     * @brief Gets the weight for a specific color.
     * @param color The color to get the weight for.
     * @return The weight for the color.
     */
    double getColorWeight(Color color) const;

private:
    cv::Mat cfaPattern;                   // CFA pattern matrix
    std::map<Color, double> colorWeights; // Map to store default color weights

    /**
     * @brief Checks if the given character is a valid CFA pattern character.
     * @param ch The character to check.
     * @return True if the character is valid, false otherwise.
     */
    bool isValidCFAPatternChar(char ch) const;
};

#endif // CFAPATTERN_H
