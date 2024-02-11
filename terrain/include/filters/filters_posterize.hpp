#ifndef FILTERS_POSTERIZE_HPP
#define FILTERS_POSTERIZE_HPP

#include <algorithm>
#include <iostream>
#include <vector>

#include "../tools.hpp"

namespace filters
{
    void posterize(const int &posterize_levels, unsigned char** pixel_data, const int &image_width, const int &image_height, const bool &verbose_mode=false)
    {
        std::vector<unsigned char> unique_colors;
        for(int y = image_height-1; y >= 0; y--)
        {
            for(int x = 0; x < image_width; x++)
            {
                //Loop through unique_colors and determine if
                //the current color is in the vector or not.
                bool color_exists_in_vector = false;
                for(int i = 0; i < unique_colors.size(); i++)
                {
                    if(unique_colors[i] == pixel_data[y][x])
                    {
                        color_exists_in_vector = true;
                    }
                }

                if(!color_exists_in_vector)
                {
                    unique_colors.push_back(pixel_data[y][x]);
                }
            }
        }

        if(verbose_mode)
        {
            std::cout << "(Posterize): Unique colors found in image: " << unique_colors.size() << std::endl;
            std::sort(unique_colors.begin(), unique_colors.end());
        }
        
        //Retrieve an X amount of colors from "unique_colors"
        //where X is defined by POSTERIZE_LEVELS,
        //"color_step" is the rough step between each color
        //to make retrieval easier.
        int color_step = unique_colors.size() / posterize_levels;
        int current_step = unique_colors.size()-1;
        unsigned int colors_added = 0;
        unsigned char new_colors[posterize_levels];

        for(int i = unique_colors.size()-1; i >= 0; i--)
        {
            if(colors_added < posterize_levels)
            {
                new_colors[colors_added] = unique_colors[current_step];
                colors_added += 1;

                current_step -= color_step;
                if(current_step < 0) { current_step = 0; }
            }
            
            else
            {
                break;
            }
        }

        //Important: sort "new_colors"
        std::sort(new_colors, new_colors + posterize_levels);
        for(int y = image_height-1; y >= 0; y--)
        {
            for(int x = 0; x < image_width; x++)
            {
                //Round each pixel in the original image data to the closest
                //corresponding value in "new_colors".
                pixel_data[y][x] = tools::round_color_to_nearest(pixel_data[y][x], new_colors, posterize_levels);
            }
        }
    }
}

#endif
