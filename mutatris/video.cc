/* libcmx generic video */
#include "cmx_video.h"
#include "cmx_error.h"
#include<cstdio>
#include<cstdlib>
#include<cassert>
#include<fstream>
#include<sstream>
#include<string.h>
#ifdef HAVE_CONFIG_H
#include<config.h>
#endif
#ifdef __APPLE__
#include<sys/mman.h>
#include<unistd.h>
#include<fcntl.h>
#endif



namespace cmx {
	namespace video {
		Surface *newSurface(int w, int h) {
			Surface *temp = new Surface();
			temp->createSurface(w,h, 4);
			return temp;
		}
		void *s = 0;
		int offset = 0;

        /*
        
        void userReadData(png_structp pngPtr, png_bytep data, png_size_t length) {
			png_voidp a = png_get_io_ptr(pngPtr);
			memcpy((void*)data,((const char*)a)+offset, length);
			offset += length;
		} */
 
		int Surface::loadImage(const void *buf, unsigned int size) { /*
            offset = 0;
            int x, y;
            int width, height;
            png_byte color_type;
            png_byte bit_depth;
            png_structp png_ptr;
            png_infop info_ptr;
            int number_of_passes;
            png_bytep * row_pointers;
            png_byte header[8];
            memcpy((char*)header, (const void*)buf, 8);
            unsigned char *buf_c = (unsigned char*)buf;
            buf_c += 8;
            
            if(png_sig_cmp(header, 0, 8) != 0) {
                std::cerr <<  "Not a valid png file\n";
                return -9;
            }
            png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
            if (!png_ptr)
                return -3;
            
            info_ptr = png_create_info_struct(png_ptr);
            
            if (!info_ptr)
                return -4;
            
            if(setjmp(png_jmpbuf(png_ptr))) {
                
                return -5;
            }
            png_set_read_fn(png_ptr,(void*)buf_c,userReadData);
            
            png_set_sig_bytes(png_ptr, 8);
            png_read_info(png_ptr, info_ptr);
            
            width = png_get_image_width(png_ptr, info_ptr);
            height = png_get_image_height(png_ptr, info_ptr);
            w = width;
            h = height;
            color_type = png_get_color_type(png_ptr, info_ptr);
            bit_depth = png_get_bit_depth(png_ptr, info_ptr);
            
            
            number_of_passes = png_set_interlace_handling(png_ptr);
            png_read_update_info(png_ptr, info_ptr);
            
            if (setjmp(png_jmpbuf(png_ptr)))
                return -6;
            
            assert(height > 0);
            row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
            assert(row_pointers != 0);
            for (y=0; y<height; y++) {
                row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr, info_ptr));
                assert(row_pointers[y] != 0);
            }
            png_read_image(png_ptr, row_pointers);
            w=width, h=height;
            int pixel_space = 3;
            if(color_type == PNG_COLOR_TYPE_RGB)
                pixel_space = 3;
            
            if(color_type == PNG_COLOR_TYPE_RGBA)
                pixel_space = 4;
            
            createSurface(w,h, 4);
            unsigned int *buffer = (unsigned int*)this->buffer;
            for(y=0;y<height;y++)  {
                png_byte *row = row_pointers[y];
                for(x=0; x<width;x++)
                {
                    png_byte *ptr = &(row[x*pixel_space]);
                    unsigned int *uptr = &buffer[x+y*width];
                    unsigned int value = _RGB(ptr[0], ptr[1], ptr[2]);
                    *uptr = value;
                }
            }
            for (y=0; y<height; y++)
                free(row_pointers[y]);
            free(row_pointers);
            png_destroy_info_struct (png_ptr, &info_ptr);
            png_destroy_read_struct (&png_ptr, 0, 0); */
            return 1;
        }
	}
}
