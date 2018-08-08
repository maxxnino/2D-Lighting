#pragma once
#include "Light.h"


void Light::DrawTriangle(const VecF& v0, const VecF& v1, const VecF& v2)
{
	// using pointers so we can swap (for sorting purposes)
	const VecF* pv0 = &v0;
	const VecF* pv1 = &v1;
	const VecF* pv2 = &v2;

	// sorting vertices by y
	if (pv1->y < pv0->y) std::swap(pv0, pv1);
	if (pv2->y < pv1->y) std::swap(pv1, pv2);
	if (pv1->y < pv0->y) std::swap(pv0, pv1);

	if (pv0->y == pv1->y) // natural flat top
	{
		// sorting top vertices by x
		if (pv1->x < pv0->x) std::swap(pv0, pv1);
		DrawFlatTopTriangle(*pv0, *pv1, *pv2);
	}
	else if (pv1->y == pv2->y) // natural flat bottom
	{
		// sorting bottom vertices by x
		if (pv2->x < pv1->x) std::swap(pv1, pv2);
		DrawFlatBottomTriangle(*pv0, *pv1, *pv2);
	}
	else // general triangle
	{
		// find splitting vertex
		const float alphaSplit =
			(pv1->y - pv0->y) /
			(pv2->y - pv0->y);
		const VecF vi = (VecF)*pv0 + ((VecF)(*pv2 - *pv0)) * alphaSplit;

		if (pv1->x < vi.x) // major right
		{
			DrawFlatBottomTriangle((VecF)*pv0, (VecF)*pv1, vi);
			DrawFlatTopTriangle((VecF)*pv1, vi, (VecF)*pv2);
		}
		else // major left
		{
			DrawFlatBottomTriangle((VecF)*pv0, vi, (VecF)*pv1);
			DrawFlatTopTriangle(vi, (VecF)*pv1, (VecF)*pv2);
		}
	}
}

void Light::DrawFlatTopTriangle(const VecF& v0, const VecF& v1, const VecF& v2)
{
	// calulcate slopes in screen space
	float m0 = (v2.x - v0.x) / (v2.y - v0.y);
	float m1 = (v2.x - v1.x) / (v2.y - v1.y);

	// calculate start and end scanlines
	const int yStart = (int)ceilf(v0.y - 0.5f);
	const int yEnd = (int)ceilf(v2.y - 0.5f); // the scanline AFTER the last line drawn

	for (int y = yStart; y < yEnd; y++)
	{
		// caluclate start and end points (x-coords)
		// add 0.5 to y value because we're calculating based on pixel CENTERS
		const float px0 = m0 * (float(y) + 0.5f - v0.y) + v0.x;
		const float px1 = m1 * (float(y) + 0.5f - v1.y) + v1.x;

		// calculate start and end pixels
		const int xStart = (int)ceilf(px0 - 0.5f);
		const int xEnd = (int)ceilf(px1 - 0.5f); // the pixel AFTER the last pixel drawn

		for (int x = xStart; x < xEnd; x++)
		{
			float len = VecF(pos.x - x, pos.y - y).GetLength();
			if (len < radian)
			{
				const VecI lightmapPos = { std::min(x - (int)pos.x + radian,radian * 2 - 1), std::min(y - (int)pos.y + radian,radian * 2 - 1) };
				PutLightMap(lightmapPos.x, lightmapPos.y, 1.0f - len / radian);
			}
		}
	}
}

void Light::DrawFlatBottomTriangle(const VecF& v0, const VecF& v1, const VecF& v2)
{
	// calulcate slopes in screen space
	float m0 = (v1.x - v0.x) / (v1.y - v0.y);
	float m1 = (v2.x - v0.x) / (v2.y - v0.y);

	// calculate start and end scanlines
	const int yStart = (int)ceilf(v0.y - 0.5f);
	const int yEnd = (int)ceilf(v2.y - 0.5f); // the scanline AFTER the last line drawn

	for (int y = yStart; y < yEnd; y++)
	{
		// caluclate start and end points
		// add 0.5 to y value because we're calculating based on pixel CENTERS
		const float px0 = m0 * (float(y) + 0.5f - v0.y) + v0.x;
		const float px1 = m1 * (float(y) + 0.5f - v0.y) + v0.x;

		// calculate start and end pixels
		const int xStart = (int)ceilf(px0 - 0.5f);
		const int xEnd = (int)ceilf(px1 - 0.5f); // the pixel AFTER the last pixel drawn

		for (int x = xStart; x < xEnd; x++)
		{
			float len = VecF(pos.x - x, pos.y - y).GetLength();
			if (len < radian)
			{
				const VecI lightmapPos = { std::min(x - (int)pos.x + radian,radian * 2 - 1), std::min(y - (int)pos.y + radian,radian * 2 - 1) };
				PutLightMap(lightmapPos.x, lightmapPos.y, 1.0f - len / radian);
			}
		}
	}
}


