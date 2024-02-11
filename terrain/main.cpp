#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <time.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "include/stb_image_write.h"

#include "include/PerlinNoise.hpp"
#include "include/tools.hpp"

//Filter headers
#include "include/filters/filters_gradient.hpp"
#include "include/filters/filters_posterize.hpp"

//General settings
#define BATCH_MODE   true
#define BATCH_COUNT  20
#define VERBOSE_MODE false

//Black recalculation threshold:
//Used to determine if there's too little terrain visible and then create new terrain.
//The threshold represents the total percentage of pixels that are black (0.99f - 0.0f).
#define BLACK_RECALC_THRESHOLD_ENABLE true
#define BLACK_RECALC_THRESHOLD        0.8f

//Image settings
#define IMAGE_NAME   "images/terrain"
#define IMAGE_EXT    ".png"
#define IMAGE_WIDTH  640
#define IMAGE_HEIGHT 480

//Noise settings
#define NOISE_STRENGTH  0.0047f
#define NOISE_OCTAVES   7
#define SEED_MAX_VALUE  999999

//Terrain settings
#define TERRAIN_SCALE      0.69f
#define TERRAIN_PADDING    10
#define TERRAIN_MIN_HEIGHT 0
#define TERRAIN_MAX_HEIGHT 0

//Filters and their settings
#define ENABLE_FILTERS true

    //Gradients
    #define FILTERS_GRADIENTS_ENABLE true

    //Posterize
    #define FILTERS_POSTERIZE_ENABLE false
    #define POSTERIZE_LEVELS         8


const static unsigned int STRIDE = (sizeof(unsigned char) * IMAGE_WIDTH);

void write_image(unsigned char** pixel_data, const char* image_name)
{
	//Create a one-dimensional array, copying all data from "pixel_data"
	//sequentially. Passing just "*pixel_data" produces a correct image but
	//if BATCH_MODE is "true" then all subsequent images get corrupted
	//unless a normal one-dimensional array is passed.
	unsigned char* new_data = new unsigned char[IMAGE_WIDTH*IMAGE_HEIGHT];
	unsigned int index = 0;
	for(int y = IMAGE_HEIGHT-1; y >= 0; y--)
	{
		for(int x = 0; x < IMAGE_WIDTH; x++)
		{
			new_data[index] = pixel_data[y][x];
			index += 1;
		}
	}

	int result = stbi_write_png(image_name, IMAGE_WIDTH, IMAGE_HEIGHT, 1, new_data, STRIDE);
	if(result == 0)
	{
		std::cout << "Error: Could not write image" << std::endl;
	}

	else
	{
		if(VERBOSE_MODE)
		{
			std::cout << "Terrain image written successfully" << std::endl;
		}
	}

    delete[] new_data;
}

unsigned char** create_pixel_data(const siv::PerlinNoise &perlin_instance)
{
	//Initialize the pixel data as a 2D array of unsigned chars
	unsigned char** pixels = new unsigned char*[IMAGE_HEIGHT];
	for(int y = 0; y < IMAGE_HEIGHT; y++)
	{
		pixels[y] = new unsigned char[IMAGE_WIDTH];
		for(int x = 0; x < IMAGE_WIDTH; x++)
		{
			pixels[y][x] = 255;
		}
	}

	//Create height data for each column along the x-axis of the image
	int* terrain_data = new int[IMAGE_WIDTH];
    int min_height;
    int max_height;

	//Optional capping of the min and max heights
    if(TERRAIN_MIN_HEIGHT <= 0) { min_height = 0; }
    else { min_height = TERRAIN_MIN_HEIGHT; }

    if(TERRAIN_MAX_HEIGHT <= 0) { max_height = IMAGE_HEIGHT; }
    else { max_height = TERRAIN_MAX_HEIGHT; }

	//Create height using 1-dimensional perlin noise with octaves
	for(int x = 0; x < IMAGE_WIDTH; x++)
	{
		float input_factor = (x * IMAGE_WIDTH * NOISE_STRENGTH) / IMAGE_WIDTH;
		int height = (perlin_instance.octave1D(input_factor, NOISE_OCTAVES) * IMAGE_HEIGHT) * TERRAIN_SCALE;
		height += TERRAIN_PADDING;	

		if(height > max_height) { height = max_height; }
		else if(height < min_height) { height = min_height; }

		terrain_data[x] = height;
	}

	//Assign pixel data with terrain_data in mind. All pixels are white by default.
	for(int y = 0; y < IMAGE_HEIGHT; y++)
	{
		for(int x = 0; x < IMAGE_WIDTH; x++)
		{
			//If the y-coordinate of the current pixel is above "height"
			//then make the pixel black.
			if(y >= terrain_data[x])
			{
				pixels[y][x] = 0;
			}
		}
	}

	delete[] terrain_data;
	return pixels;
}


void generate_terrain_and_write_image(const int &image_number=1)
{
	//Seed and initialize the perlin instance
	siv::PerlinNoise::seed_type seed = rand()%SEED_MAX_VALUE;
	siv::PerlinNoise perlin_instance{seed};

	unsigned char** pixel_data = create_pixel_data(perlin_instance);

	if(BLACK_RECALC_THRESHOLD_ENABLE)
	{
		if(BLACK_RECALC_THRESHOLD > 0.0f && BLACK_RECALC_THRESHOLD <= 0.99f)
		{

			float current_black_percentage = tools::calculate_black_percentage(pixel_data, IMAGE_WIDTH, IMAGE_HEIGHT);
			while(current_black_percentage >= BLACK_RECALC_THRESHOLD)
			{
				//Reseed the perlin instance to prevent infinite loop
				seed = rand()%SEED_MAX_VALUE;
				perlin_instance.reseed(seed);				

				pixel_data = create_pixel_data(perlin_instance);
				current_black_percentage = tools::calculate_black_percentage(pixel_data, IMAGE_WIDTH, IMAGE_HEIGHT);	
			}
		}
	}

    if(ENABLE_FILTERS)
    {
        if(FILTERS_GRADIENTS_ENABLE)
        {
            filters::gradient(pixel_data, IMAGE_WIDTH, IMAGE_HEIGHT);
        }

        if(FILTERS_POSTERIZE_ENABLE && POSTERIZE_LEVELS >= 1)
        {
            filters::posterize(POSTERIZE_LEVELS, pixel_data, IMAGE_WIDTH, IMAGE_HEIGHT, VERBOSE_MODE);
        }
    }
    
	std::string filename = "";
	filename += std::string(IMAGE_NAME) + "_" + std::to_string(image_number) + std::string(IMAGE_EXT);
	write_image(pixel_data, filename.c_str());
	
	for(int y = 0; y < IMAGE_HEIGHT; y++)
	{
		delete[] pixel_data[y];
	}
	
	delete[] pixel_data;
}

int main()
{
	srand(time(NULL));

	if(BLACK_RECALC_THRESHOLD_ENABLE && BLACK_RECALC_THRESHOLD > 0.0f && BLACK_RECALC_THRESHOLD <= 0.99f)
	{
		std::cout << "[INFO] Black recalculation threshold enabled (" << static_cast<int>(BLACK_RECALC_THRESHOLD * 100) << "%, image(s) may take longer to generate)" << std::endl;
	}

	if(!BATCH_MODE)
	{
		generate_terrain_and_write_image();
	}

	else
	{
		int max_iterations = BATCH_COUNT;
		if(BATCH_COUNT <= 0) { max_iterations = 2; }	

		for(int i = 0; i < max_iterations; i++)
		{
			generate_terrain_and_write_image(i+1);
		}
	}

	return 0;
}
