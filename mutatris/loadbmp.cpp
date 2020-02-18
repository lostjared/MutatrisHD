#include "MX_interface.h"
#include<cstdio>
#include<cstdlib>


namespace MX_i {

void fix_endian_func(void* data, int size) {
	int endian_test = 1;
	unsigned char* endian_test_bytes = (unsigned char *)&endian_test;
	if (endian_test_bytes[0] == '\0')
	{
		 unsigned char* cdata = (unsigned char*)data;
		 int i;
		 for (i=0; i<size/2; i++) {
			 unsigned char temp = cdata[i];
			 cdata[i] = cdata[size-1 - i];
			 cdata[size-1 - i] = temp;
		}
	}
}

 #define fix_endian(x)  (fix_endian_func(&(x), sizeof(x)))

GLubyte *LoadBMP(const char *source, int *w, int *h) {
		   FILE *fptr = 0;
		   long offset, size;
		   short type, bitcount;
		   GLubyte *raw_data = 0;
		   int i = 0;
	  	   int x, y,q;

		   fptr = fopen(source,"rb");
		   if(!fptr) {
			   perror("fopen: ");
			   return 0;
		   }
		   fread(&type, sizeof(short), 1, fptr);
#ifdef __FOR_PS3__
		   fix_endian(type);
#endif
		   if(type != 0x4D42) {
			   printf("Not a bitmap\n");
				fclose(fptr);
				return 0;
		   }

		   int width, height;

		   fseek(fptr, 10, SEEK_SET);
		   fread(&offset, sizeof(int), 1, fptr);
		   fseek(fptr, 4, SEEK_CUR);
		   fread(&width, sizeof(int), 1, fptr);
		   fread(&height, sizeof(int), 1, fptr);
		   fseek(fptr, 2, SEEK_CUR);
		   fread(&bitcount, sizeof(short), 1, fptr);

#ifdef __FOR_PS3__
		   fix_endian(offset);
		   fix_endian(width);
		   fix_endian(height);
		   fix_endian(bitcount);
#endif


		   if(bitcount != 24) {
			   	 printf("Not 24 bit: %d\n", bitcount);
				fclose(fptr);
				return 0;
		   }


		   fseek(fptr, 4, SEEK_CUR);
		   fread(&size, sizeof(int), 1, fptr);
		   fseek(fptr,16,SEEK_CUR);

#ifdef __FOR_PS3__
		   fix_endian(size);
#endif

		   raw_data = (GLubyte*) malloc ( ((width) * (height)) * 4 );

		   GLubyte *ptr = raw_data;

		   if(!raw_data) {
				fclose(fptr);
				return 0;
		   }

		   for(y = height-1; y > -1; y--)
		   {
				for(x = 0; x < width; x++)
				{
					 q = x + y * (width);
					 GLubyte b,g,r,w;
					 b = fgetc(fptr);
					 g = fgetc(fptr);
					 r = fgetc(fptr);
					 w = 255;
					 *ptr = r;
					 ptr++;
					 *ptr = g;
					 ptr++;
					 *ptr = b;
					 ptr++;
					 *ptr = 255;
					 ptr++;
				}
				for(i = 0; i < (width) % 4; i++)
					 fgetc(fptr);
		   }
		   fclose(fptr);
		   *w = width;
		   *h = height;
		   return raw_data;
	}
}
