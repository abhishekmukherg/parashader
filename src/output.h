#ifndef OUTPUT_H_
#define OUTPUT_H_

#include <string>
#include <ostream>


class Output {
private:
	struct RGB {
		unsigned char r, g, b;
	};
	size_t width, height;
	RGB **data;

	Output(const Output &);
	const Output &operator=(const Output &);
public:
	Output(size_t w, size_t h);
	~Output();

	void set_pixel(size_t x, size_t y, double r, double g, double b);
	void set_pixel(size_t x, size_t y,
			unsigned char r, unsigned char g, unsigned char b);

	void to_ppm(std::ostream &file);

};

#endif
