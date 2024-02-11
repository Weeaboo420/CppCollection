#ifndef FILTERS_GRADIENT_HPP
#define FILTERS_GRADIENT_HPP

namespace filters
{
    void gradient(unsigned char** pixel_data, const int &image_width, const int &image_height)
    {
        for(int x = 0; x < image_width; x++)
        {
            //First loop along the y-axis: calculate terrain height for
            //the current column (x).
            int height = 0;
            for(int y = image_height-1; y >= 0; y--)
            {
                if(pixel_data[y][x] == 255)
                {
                    height += 1;
                }
            }

            if(height >= 2)
            {
                //Calculate the current brightness with the help
                //of a brightness step which will reduce the overall
                //brightness as the gradient goes on.
                float current_brightness = 1.0f;
                float brightness_step = 1.0f/static_cast<float>(height);

                //Second loop. Fill each pixel inside the column according
                //to the gradient going from 255 to 0.
                for(int y = image_height-1; y >= 0; y--)
                {
                    if(y <= height)
                    {
                        float new_brightness = current_brightness * 255.0f;
                        if(new_brightness > 255.0f) { new_brightness = 255.0f; }
                        else if(new_brightness < 0.0f) { new_brightness = 0.0f; }  

                        pixel_data[y][x] = static_cast<unsigned char>(new_brightness);

                        current_brightness -= brightness_step;
                        if(current_brightness < 0.0f) { current_brightness = 0.0f; }
                    }
                }
            }
        }
    } //void end
}

#endif
