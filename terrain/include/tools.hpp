#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <cmath>

namespace tools
{
    //This function uses unsigned chars, a version with integers would require the use of absolute values with std::abs (<cmath>). 
    unsigned char get_nearest_char(const unsigned char &a, const unsigned char &b, const unsigned char &target)
    {
        if(target - a >= b - target)
        {
            return b;
        }

        return a;
    }

    //This function assumes that "pixel_data" is a sorted array
    unsigned char round_color_to_nearest(const unsigned char &target, unsigned char* pixel_data, const int &pixel_data_array_size)
    {
        int low = 0;
        int middle = 0;
        int high = pixel_data_array_size-1;

        if(target <= pixel_data[0])
        {
            return pixel_data[0];
        }

        else if(target >= pixel_data[high])
        {
            return pixel_data[high];
        }

        //Binary search to find number closest to "target" in "array".
        while(low <= high)
        {
            //Determine "middle" point to reduce the
            //search area by half for each iteration.
            middle = (low + high) / 2;

            if(pixel_data[middle] == target)
            {
                return pixel_data[middle];
            }

            //"target" is to the left of "middle".
            if(target < pixel_data[middle])
            {
                //"target" is between middle-1 and middle, determine which
                //of the two is closest to "target".
                if(middle > 0 && target > pixel_data[middle-1])
                {
                    return get_nearest_char(pixel_data[middle-1], pixel_data[middle], target);
                }

                high = middle;
            }

            //"target" is to the right of "middle".
            else
            {
                //"target" is between high-1 and middle+1, determine which
                //of the two is closest to "target".
                if(middle < high-1 && target < pixel_data[middle+1])
                {
                    return get_nearest_char(pixel_data[middle], pixel_data[middle+1], target);
                }

                low = middle + 1;
            }
        }

        return pixel_data[middle];
    }
	
	float calculate_black_percentage(unsigned char** pixel_data, const int &image_width, const int &image_height)
	{
		float percentage = 0.0f;
		unsigned int pixel_count = static_cast<unsigned int>(image_width * image_height);
		unsigned int black_pixels = 0;

		for(int y = image_height-1; y >= 0; y--)
		{
			for(int x = 0; x < image_width; x++)
			{
				if(pixel_data[y][x] == 0)
				{
					black_pixels += 1;
				}
			}
		}

		percentage = std::roundf((static_cast<float>(black_pixels) / static_cast<float>(pixel_count)) * 100) / 100.0;
		return percentage;
	}

}

#endif
