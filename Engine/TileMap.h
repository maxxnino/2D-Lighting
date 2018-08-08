#pragma once

#include "Graphics.h"
#include "Surface.h"
#include "SpriteEffect.h"
#include "Codex.h"
#include <vector>

class TileMap
{
public:
	TileMap()
		:
		pTilesetSurface(Codex<Surface>::Retrieve(L"Images\\bg1.jpg")),
		size(200),
		row(size / sizeSection)
	{
		for (int y = 0; y < row; y++)
		{
			for (int x = 0; x < row; x++)
			{
				rects.emplace_back(x * sizeSection, y * sizeSection, (x + 1) * sizeSection, (y + 1) * sizeSection);
			}
		}

		if (Graphics::ScreenHeight / sizeSection > 0)
		{
			gridHeight = 1 + Graphics::ScreenHeight / sizeSection;
		}
		else
		{
			gridHeight = Graphics::ScreenHeight / sizeSection;

		}

		if (Graphics::ScreenWidth / sizeSection > 0)
		{
			gridWidth = 1 + Graphics::ScreenWidth / sizeSection;
		}
		else
		{
			gridWidth = Graphics::ScreenWidth / sizeSection;

		}
	}
	void Draw(Graphics& gfx) const
	{
		for (int y = 0; y < gridHeight; y++)
		{
			for (int x = 0; x < gridWidth; x++)
			{
				gfx.DrawSprite(x * sizeSection, y * sizeSection, GetRect(x, y), *pTilesetSurface, SpriteEffect::InShadow{ 0.3f });
			}
		}
		
	}
private:
	const RectI& GetRect(int x, int y) const
	{
		return rects[(y % row) * row + (x % row)];
	}
private:
	// tileset image for background
	static constexpr int sizeSection = 200;
	const Surface* pTilesetSurface;
	std::vector<RectI> rects;
	int size;
	int row;
	int gridWidth;
	int gridHeight;
};