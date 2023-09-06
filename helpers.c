#include "helpers.h"
#include <math.h>
#include <stdio.h>

int gx_rgbtRed;
int gx_rgbtBlue;
int gx_rgbtGreen;

int gy_rgbtRed;
int gy_rgbtBlue;
int gy_rgbtGreen;

long total_rgbtRed = 0;
long total_rgbtBlue = 0;
long total_rgbtGreen = 0;

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // convert it into black and white
    // if rgb are same we get a lighter(higher value of rgb) and darker(lower value of rgb)
    // calculate the average of rgb values
    // and set every rgb to that average value (the result is a shade of grey)
    // if average is not int we need to round the value
    float average = 0;

    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            average = (image[row][column].rgbtRed + image[row][column].rgbtBlue + image[row][column].rgbtGreen) / 3.0;
            average = round(average);
            image[row][column].rgbtRed = average;
            image[row][column].rgbtBlue = average;
            image[row][column].rgbtGreen = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // for every row swap the pixels on horizontally opposite side
    RGBTRIPLE temp[height][width];

    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width / 2; column++)
        {
            temp[row][column] = image[row][column];
            image[row][column] = image[row][(width - 1) - column];
            image[row][(width - 1) - column] = temp[row][column];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // compute new value of a pixel by averageing the value of the pixel around it ie in one colomn and row
    // caluclate avg red avg green and avg blue and give that value to the pixel
    RGBTRIPLE average;
    // long total_rgbtRed = 0;
    // long total_rgbtBlue = 0;
    // long total_rgbtGreen = 0;
    RGBTRIPLE copy_image[height][width];
    long valid_pixels = 0;

    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            for (int row_offset = -1; row_offset <= 1; row_offset++)
            {
                for (int column_offset = -1; column_offset <= 1; column_offset++)
                {
                    if ((row + row_offset >= 0 && row + row_offset < height) &&
                        (column + column_offset >= 0 && column + column_offset < width))
                    {
                        total_rgbtRed += image[row + row_offset][column + column_offset].rgbtRed;
                        total_rgbtBlue += image[row + row_offset][column + column_offset].rgbtBlue;
                        total_rgbtGreen += image[row + row_offset][column + column_offset].rgbtGreen;
                        valid_pixels++;
                    }
                }
            }
            average.rgbtRed = round((float) total_rgbtRed / valid_pixels);
            average.rgbtBlue = round((float) total_rgbtBlue / valid_pixels);
            average.rgbtGreen = round((float) total_rgbtGreen / valid_pixels);

            copy_image[row][column].rgbtRed = average.rgbtRed;
            copy_image[row][column].rgbtBlue = average.rgbtBlue;
            copy_image[row][column].rgbtGreen = average.rgbtGreen;

            total_rgbtRed = 0;
            total_rgbtBlue = 0;
            total_rgbtGreen = 0;
            valid_pixels = 0;
        }
    }
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            image[row][column].rgbtRed = copy_image[row][column].rgbtRed;
            image[row][column].rgbtBlue = copy_image[row][column].rgbtBlue;
            image[row][column].rgbtGreen = copy_image[row][column].rgbtGreen;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // -1(255) + 0 (255) + 1(255) ...... if gx = 0 meaning there is no change in red value, gx is for moving left to right
    // -1(255) + -2(255) + -1(255) ......if gy = 0 meaning there is a no change in red value, gy is for moving top to bottom
    // compute gx and gy for each channel of red, green, blue
    // for the pixel at border treat them as 0
    // compute each new channel as the square root of Gx^2 + Gy^2; squaring means we end up a positive value

    // long gx_rgbtRed = 0;
    // long gy_rgbtRed = 0;

    RGBTRIPLE copy_image[height][width];

    int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

    int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            for (int row_offset = -1; row_offset <= 1; row_offset++)
            {
                for (int column_offset = -1; column_offset <= 1; column_offset++)
                {
                    if ((row + row_offset >= 0 && row + row_offset < height) &&
                        (column + column_offset >= 0 && column + column_offset < width))
                    {
                        int weight_x = gx[row_offset + 1][column_offset + 1];
                        gx_rgbtRed += weight_x * image[row + row_offset][column + column_offset].rgbtRed;
                        gx_rgbtBlue += weight_x * image[row + row_offset][column + column_offset].rgbtBlue;
                        gx_rgbtGreen += weight_x * image[row + row_offset][column + column_offset].rgbtGreen;

                        int weight_y = gy[row_offset + 1][column_offset + 1];
                        gy_rgbtRed += weight_y * image[row + row_offset][column + column_offset].rgbtRed;
                        gy_rgbtBlue += weight_y * image[row + row_offset][column + column_offset].rgbtBlue;
                        gy_rgbtGreen += weight_y * image[row + row_offset][column + column_offset].rgbtGreen;
                    }
                }
            }

            total_rgbtRed = round(sqrt(pow(gx_rgbtRed, 2) + pow(gy_rgbtRed, 2)));
            if (total_rgbtRed > 255)
            {
                total_rgbtRed = 255;
            }
            total_rgbtBlue = round(sqrt(pow(gx_rgbtBlue, 2) + pow(gy_rgbtBlue, 2)));
            if (total_rgbtBlue > 255)
            {
                total_rgbtBlue = 255;
            }
            total_rgbtGreen = round(sqrt(pow(gx_rgbtGreen, 2) + pow(gy_rgbtGreen, 2)));
            if (total_rgbtGreen > 255)
            {
                total_rgbtGreen = 255;
            }

            copy_image[row][column].rgbtRed = total_rgbtRed;
            copy_image[row][column].rgbtBlue = total_rgbtBlue;
            copy_image[row][column].rgbtGreen = total_rgbtGreen;

            gx_rgbtRed = 0;
            gx_rgbtBlue = 0;
            gx_rgbtGreen = 0;

            gy_rgbtRed = 0;
            gy_rgbtBlue = 0;
            gy_rgbtGreen = 0;
        }
    }

    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            image[row][column].rgbtRed = copy_image[row][column].rgbtRed;
            image[row][column].rgbtBlue = copy_image[row][column].rgbtBlue;
            image[row][column].rgbtGreen = copy_image[row][column].rgbtGreen;
        }
    }
    return;
}
