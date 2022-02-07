#include <iostream>
#include <fstream>
#include <string>
// #include <opencv2/opencv.hpp>

#define MAX_WIDTH 10000
#define MAX_HEIGHT 10000

struct Image
{
    int16_t r = 0;
    int16_t g = 0;
    int16_t b = 0;
} checkerboard[MAX_WIDTH][MAX_HEIGHT];

// void generate_cat(int row, int col)
// {
//     cv::Mat3b img_bgr = cv::imread("cat.jpeg");

//     for (int r = 0; r < img_bgr.rows && r < row; ++r)
//     {
//         for (int c = 0; c < img_bgr.cols && r < col; ++c)
//         {
//             // access RGB values for each pixel
//             cv::Vec3b pixel = img_bgr(r, c);
//             checkerboard[r][c].b = pixel[0];
//             checkerboard[r][c].g = pixel[1];
//             checkerboard[r][c].r = pixel[2];
//         }
//     }
// }

int main(int argc, char **argv)
{
    // check that the number of command arguments is correct
    if (argc != 2)
    {
        std::cout << "Incorrect number parameters! Usage: ./simpleppm input_filename" << std::endl;
    }

    std::string words[5];
    int32_t width;
    int32_t height;
    int32_t pixel_counter = 0;
    int32_t input_idx = 0;
    std::string input_file = argv[1];
    std::string output_file = input_file + ".ppm";
    std::ofstream output_stream(output_file, std::ios::out);
    std::ifstream input_stream(input_file, std::ios::in);
    // check whether the file is opened successfully
    if (!input_stream.is_open())
    {
        std::cerr << "Could not open the file '"
             << input_file << "'" << std::endl;
        return -1;
    }
    // read from the input file and check input correctness
    while (input_stream >> words[input_idx++])
    {
        if (input_idx > 3) {
            std::cout << "Unexpected input file format: Must contain only three words." << std::endl;
            return 0;
        }
    }
    // check keyword
    if (words[0] != "imsize")
    {
        std::cout << "Missing the keyword imsize!" << std::endl;
        return 0;
    }
    // check whethre width and height are indicated numerically in the input file
    try
    {
        width = std::stoi(words[1]);
        height = std::stoi(words[2]);
    }
    catch (...)
    {
        std::cout << "Conversion failure, width or height not specified correctly!" << std::endl;
        return 0;
    }
    // check whether width and height are both positive
    if (width < 1 || height < 1 || width > MAX_WIDTH || width > MAX_HEIGHT)
    {
        std::cout << "Input file error! Weight and height must be positive and less than 10000!" << std::endl;
        return 0;
    }
    // create an RGB checkerboard
    for (uint32_t x = 0; x < width; x++)
    {
        for (uint32_t y = 0; y < height; y++)
        {
            checkerboard[x][y].r = (x + y) % 255;
            checkerboard[x][y].g = (x - y) % 255;
            checkerboard[x][y].b = 0;
        }
    }

    // Uncomment the following line to generate a cat image in ppm format
    // generate_cat(height, width);

    // create a ppm image in P3 format
    output_stream << "P3" << std::endl
        << width << " "
        << height << std::endl
        << 255 << std::endl;
    // fill in color for each pixel
    for(uint32_t x = 0; x < width; x++)
    {
        for (uint32_t y = 0; y < height; y++)
        {
            output_stream << checkerboard[x][y].r << " "
                          << checkerboard[x][y].g << " "
                          << checkerboard[x][y].b << " ";
            pixel_counter++;
            // start a new line if 4 pixels are filled
            if (pixel_counter % 4 == 0)
            {
                pixel_counter = 0;
                output_stream << std::endl;
            }
        }
    }
    // indication of successfuly execution
    std::cout << "PPM image created successfully!" << std::endl;

    output_stream.close();
    input_stream.close();

    return 0;
}