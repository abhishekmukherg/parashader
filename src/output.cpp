#include "output.h"

#include <limits>
#include <iostream>
#include <cassert>

const char *MAGIC_NUMBER = "P3";

Output::Output(size_t w, size_t h)
	: width(w), height(h), data(NULL)
{
	assert(width > 0);
	assert(height > 0);
	data = new RGB*[height];
	for (size_t i = 0; i < height; ++i)
		data[i] = new RGB[width];

}

Output::~Output()
{
	for (size_t i = 0; i < height; ++i)
		delete []data[i];
	delete[] data;
}

void Output::set_pixel(size_t x, size_t y, double r, double g, double b)
{
	set_pixel(x, y, static_cast<unsigned char>(r),
			static_cast<unsigned char>(g),
			static_cast<unsigned char>(b));
}

void Output::set_pixel(size_t x, size_t y,
		unsigned char r, unsigned char g, unsigned char b)
{
	assert(y < height);
	assert(x < width);
	data[y][x].r = r;
	data[y][x].g = g;
	data[y][x].b = b;
}

void Output::to_ppm(std::ostream &file)
{
	file << MAGIC_NUMBER << std::endl;
	file << width << std::endl;
	file << height << std::endl;
	file << static_cast<int>(std::numeric_limits<unsigned char>::max()) << std::endl;
	
	for (size_t j = 0; j < height; ++j) {
		for (size_t i = 0; i < width; ++i) {
			const RGB &b = data[j][i];

			file << static_cast<int>(b.r) << " "
				<< static_cast<int>(b.g) << " "
				<< static_cast<int>(b.b) << std::endl;
		}
	}

}
