#include "Surface.h"
#include <cassert>

void Surface::PutPixel(int x, int y, Color c)
{
	assert(x >= 0);
	assert(x < width);
	assert(y >= 0);
	assert(y < height);
	pixels.data()[y * width + x] = c;
}

Color Surface::GetPixel(int x, int y) const
{
	assert(x >= 0);
	assert(x < width);
	assert(y >= 0);
	assert(y < height);
	return pixels.data()[y * width + x];
}

void Surface::Fill(Color c)
{
	std::fill(pixels.begin(), pixels.begin() + height * width, c);
}

void Surface::BakeAlpha()
{
	const int nPixels = GetWidth() * GetHeight();
	for (int i = 0; i < nPixels; i++)
	{
		auto pix = pixels[i];
		const int alpha = pix.GetA();
		// premulitply alpha time each channel
		pix.SetR((pix.GetR() * alpha) / 256);
		pix.SetG((pix.GetG() * alpha) / 256);
		pix.SetB((pix.GetB() * alpha) / 256);
		// write back to surface
		pixels[i] = pix;
	}
}