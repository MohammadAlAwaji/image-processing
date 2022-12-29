#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "image_bright.c"
#include "image_dark.c"
#include "image_colortosepia.c"
#include "image_simulate_cvd.c"
#include "image_correct_cvd.c"
#include "image_rgbtogray.c"
#include "black_white.c"
#include "image_blur_color.c"
#include "image_blur_gray.c"
#include "image_rotate.c"
#include "negative_image.c"

#ifdef RELEASE
#define printer
#else
#define printer printf
#endif

int colored(char imageFileName[])
{
	char ImageFilePath[150];
	sprintf(ImageFilePath, "images/%s.bmp", imageFileName);

	printer("******** This code is executing the colored image processing applications ***** \n");
	printer(" ==  %s \n", ImageFilePath);
	FILE *fIn = fopen(ImageFilePath, "r");	 // Input File name
	FILE *fIn3D = fopen(ImageFilePath, "r"); // Input File name
	unsigned char header[54];
	unsigned char colorTable[1024];
	int i, j;

	if (fIn == NULL) // check if the input file has not been opened succesfully.
	{
		printf("File does not exist.\n");
	}

	for (i = 0; i < 54; i++) // read the 54 byte header from fIn
	{
		{
			header[i] = getc(fIn);
			getc(fIn3D);
		}
	}

	int height = *(int *)&header[18];
	int width = *(int *)&header[22];
	int bitDepth = *(int *)&header[28];
	if (bitDepth <= 8) // if ColorTable present, extract it.
	{
		fread(colorTable, sizeof(unsigned char), 1024, fIn);
	}

	int size = height * width;				  // calculate image size
	unsigned char D3buffer[width][height][3]; // to store the image data

	for (int i = 0; i < width; i++)
	{
		for (j = 0; j < height; j++)
		{
			D3buffer[i][j][2] = getc(fIn3D); // blue
			D3buffer[i][j][1] = getc(fIn3D); // green
			D3buffer[i][j][0] = getc(fIn3D); // red
		}
	}
	unsigned char buffer[size][3]; // to store the image data

	for (i = 0; i < size; i++)
	{
		{
			buffer[i][2] = getc(fIn); // blue
			buffer[i][1] = getc(fIn); // green
			buffer[i][0] = getc(fIn); // red
		}
	}

	printer("height: %d\n", height);
	printer("width: %d\n", width);
	printer("size: %d\n", size);

	image_colortosepia(imageFileName, header, size, buffer, bitDepth, colorTable);

	simulate_cvd_protanopia(imageFileName, header, size, buffer, bitDepth, colorTable);

	simulate_cvd_deuteranopia(imageFileName, header, size, buffer, bitDepth, colorTable);

	simulate_cvd_tritanopia(imageFileName, header, size, buffer, bitDepth, colorTable);

	correct_cvd_protanopia(imageFileName, header, size, buffer, bitDepth, colorTable);

	correct_cvd_deuteranopia(imageFileName, header, size, buffer, bitDepth, colorTable);

	correct_cvd_tritanopia(imageFileName, header, size, buffer, bitDepth, colorTable);

	black_and_white(imageFileName, header, size, buffer, bitDepth, colorTable);

	image_bluring_color(imageFileName, header, size, height, width, buffer, bitDepth, colorTable);

	image_rgb_rotate_right(imageFileName, header, height, width, D3buffer, colorTable);

	image_rgb_rotate_left(imageFileName, header, height, width, D3buffer, colorTable);

	image_rgb_rotate_180(imageFileName, header, height, width, D3buffer, colorTable);

	image_negative(imageFileName, header, height, width, D3buffer, colorTable);

	image_rgbtogray(imageFileName, header, height, width, D3buffer, colorTable);

	fclose(fIn);
	return 0;
}

int nonColored(char imageFileName[])
{
	printer("******** This code is executing the non-colored image processing applications ****** \n");

	// FILE *fIn = fopen(imageFileName, "r"); // Input File name
	// FILE *fIn = fopen(imageFileName, "r"); // Input File name
	char ImageFilePath[150];
	sprintf(ImageFilePath, "images/%s.bmp", imageFileName);
	printer(" ==  %s \n", ImageFilePath);
	FILE *fIn = fopen(ImageFilePath, "r"); // Input File name

	unsigned char header[54];
	unsigned char colorTable[1024];
	int i;

	if (fIn == NULL) // check if the input file has not been opened succesfully.
	{
		printf("File does not exist.\n");
	}

	for (i = 0; i < 54; i++) // read the 54 byte header from fIn
	{
		{
			header[i] = getc(fIn);
		}
	}

	int height = *(int *)&header[18];
	int width = *(int *)&header[22];
	int bitDepth = *(int *)&header[28];

	if (bitDepth <= 8) // if ColorTable present, extract it.
	{
		fread(colorTable, sizeof(unsigned char), 1024, fIn);
	}

	int size = height * width;	// calculate image size
	unsigned char buffer[size]; // to store the image data

	for (i = 0; i < size; i++)
	{
		buffer[i] = getc(fIn);
	}

	printer("height: %d\n", height);
	printer("width: %d\n", width);
	printer("size: %d\n", size);

	image_bluring_gray(imageFileName, header, size, height, width, buffer, bitDepth, colorTable); // lena512.bmp

	image_dark(imageFileName, header, colorTable, size, buffer);

	image_bright(imageFileName, header, colorTable, size, buffer);
	fclose(fIn);
	return 0;
}

int C = 1;

int coloredImagesDriver()
{
	char coloredImages[24][100] = {
		"airplane",
		"baboon",
		"barbara",
		"BoatsColor",
		"goldhill",
		"lena_color",
		"airplane",
		"baboon",
		"barbara",
		"BoatsColor",
		"goldhill",
		"lena_color",
		"airplane",
		"baboon",
		"barbara",
		"BoatsColor",
		"goldhill",
		"lena_color",
		"airplane",
		"baboon",
		"barbara",
		"BoatsColor",
		"goldhill",
		"lena_color",
	};

	for (int i = 0; i < 24; i++)
	{
		// Process image
		colored(coloredImages[i]);
	}

	return 0;
}

int nonColoredImagesDriver()
{
	char nonColoredImages[24][100] = {
		"boats",
		"boats",
		"boats",
		"boats",
		"boats",
		"boats",
		"boats",
		"boats",
		"boats",
		"boats",
		"boats",
		"boats",
		"boats",
		"boats",
		"boats",
		"boats",
		"boats",
		"boats",
		"boats",
		"boats",
		"boats",
		"boats",
		"boats",
		"boats",
	};

	for (int i = 0; i < 24; i++)
	{
		nonColored(nonColoredImages[i]);
	}

	return 0;
}

int main(int argc, char *argv[])
{
	clock_t CStart, CStop, NCStart, NCStop;
	double cpu_time_used;

	CStart = clock();
	coloredImagesDriver();
	CStop = clock();

	NCStart = clock();
	nonColoredImagesDriver();
	NCStop = clock();

	double cc_time = ((double)(CStop - CStart)) / CLOCKS_PER_SEC;
	double nc_time = ((double)(NCStop - NCStart)) / CLOCKS_PER_SEC;
	printf("colored excution Time: %lf ms\n", cc_time);
	printf("non-colored excution Time: %lf ms\n", nc_time);
	printf("overall excution Time: %lf ms\n", (cc_time + nc_time));
}