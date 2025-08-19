#pragma once
#include <vector>
#include <stdexcept>
#include <fstream>

struct Color {
	short r;
	short g;
	short b;
};

void write_n_bytes(std::vector<std::byte>& vec, int n, int data) {
	// Increasing offset due to little-endian ordering (LSB first)
	int offset = 0;
	for (int i = 0; i < n; i++) {
		vec.push_back(std::byte((data >> offset) & 255));
		offset += 8;
	}
}

class Image {
private:
	std::vector<Color> data;
	int width;
	int height;

	void validate_dimensions(int x, int y) const {
		if (x >= height) throw std::invalid_argument("Height exceeded");
		if (y >= width) throw std::invalid_argument("Width exceeded");
	}

public:
	Image(int _width, int _height) :
		width(_width), height(_height) {
		for (int i = 0; i < _width * _height; i++) {
			data.push_back(Color(0, 0, 0));
		}
	}
	int get_width() const { return width; }
	int get_height() const { return height; }
	Color get_color(int x, int y) const {
		validate_dimensions(x, y);
		return data[x * width + y];
	}
	void draw(int x, int y, Color color) {
		validate_dimensions(x, y);
		data[x * width + y] = color;
	}
	void generateBmp(std::string file_name) {
		std::vector<std::byte> image_bytes;

		// File Header
		image_bytes.push_back(std::byte('B'));
		image_bytes.push_back(std::byte('M'));

		const int file_size = 14 + 40 + 3 * width * height;
		write_n_bytes(image_bytes, 4, file_size);

		const int reserved_field = 0;
		write_n_bytes(image_bytes, 4, reserved_field);

		const int pixel_data_offset = 14 + 40;
		write_n_bytes(image_bytes, 4, pixel_data_offset);

		// Bitmap header
		const int bitmap_header_size = 40;
		write_n_bytes(image_bytes, 4, bitmap_header_size);

		write_n_bytes(image_bytes, 4, width);
		write_n_bytes(image_bytes, 4, height);

		const int bmp_reserved_field = 1;
		write_n_bytes(image_bytes, 2, bmp_reserved_field);

		const int bits_per_pixel = 24;
		write_n_bytes(image_bytes, 2, bits_per_pixel);

		const int compression = 0;
		write_n_bytes(image_bytes, 4, compression);

		const int size_of_pixel_data = 3 * width * height;
		write_n_bytes(image_bytes, 4, size_of_pixel_data);

		const int horizontal_resolution = 2835;
		write_n_bytes(image_bytes, 4, horizontal_resolution);

		const int vertical_resolution = 2835;
		write_n_bytes(image_bytes, 4, vertical_resolution);

		const int color_palette_info = 0;
		write_n_bytes(image_bytes, 4, color_palette_info);

		const int no_important_colors = 0;
		write_n_bytes(image_bytes, 4, no_important_colors);

		for (int r = height - 1; r >= 0; r--) {

			for (int c = 0; c < width; c++) {

				int i = r * width + c;

				image_bytes.push_back(static_cast<std::byte>(data[i].b));
				image_bytes.push_back(static_cast<std::byte>(data[i].g));
				image_bytes.push_back(static_cast<std::byte>(data[i].r));
			}
			int bytes_so_far = width * 3;
			while (bytes_so_far % 4 != 0) {
				image_bytes.push_back(std::byte(0));
				bytes_so_far++;
			}
		}

		// std::ios::binary needed for binary data instead of text data on Windows
		std::ofstream image_file(file_name, std::ios::binary);

		image_file.write(reinterpret_cast<const char*>(image_bytes.data()), image_bytes.size());
		image_file.close();
	}
};
