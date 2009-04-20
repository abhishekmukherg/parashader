#include "output.h"

#include <fstream>
#include <iostream>

int main(int argc, char *argv[])
{
	Output o(255, 255);
	unsigned char z = 0;
	for (size_t j = 0; j < 255; ++j) {
		for (size_t i = 0; i < 255; ++i) {
			o.set_pixel(j, i, z, static_cast<int>(i), static_cast<int>(j));
		}
	}
	std::ofstream outfile("foo.ppm");
	o.to_ppm(outfile);
}
