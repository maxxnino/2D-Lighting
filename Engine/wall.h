#pragma once
#include "Graphics.h"
#include <vector>
class Wall
{
public:
	struct Segment
	{
		Segment(VecF pointA, VecF pointB)
			:
			pointA(pointA),
			pointB(pointB)
		{}
		VecF pointA;
		VecF pointB;
	};
	Wall()
	{
		for (const auto& w : walls)
		{
			wallPoints.emplace_back(std::move(VecF(w.left, w.top)));
			wallPoints.emplace_back(std::move(VecF(w.left, w.bottom)));
			wallPoints.emplace_back(std::move(VecF(w.right, w.bottom)));
			wallPoints.emplace_back(std::move(VecF(w.right, w.top)));
		}
		for (size_t i = 0; i < wallPoints.size(); i += 4)
		{
			segments.emplace_back(wallPoints[i], wallPoints[i + 1]);
			segments.emplace_back(wallPoints[i + 1], wallPoints[i + 2]);
			segments.emplace_back(wallPoints[i + 2], wallPoints[i + 3]);
			segments.emplace_back(wallPoints[i + 3], wallPoints[i]);
		}
		segments.emplace_back(VecF(0.0f, Graphics::ScreenHeight - 1), VecF(0.0f, 0.0f));
		segments.emplace_back(VecF(Graphics::ScreenWidth - 1, Graphics::ScreenHeight - 1), VecF(0.0f, Graphics::ScreenHeight - 1));
		segments.emplace_back(VecF(Graphics::ScreenWidth - 1, 0.0f), VecF(Graphics::ScreenWidth - 1, Graphics::ScreenHeight - 1));
		segments.emplace_back(VecF(0.0f, 0.0f), VecF(Graphics::ScreenWidth - 1, 0.0f));
	}
	
	void Draw(Graphics& gfx) const;
	const std::vector<RectF>& getRectWalls() const 
	{
		return walls;
	}
	const std::vector<Segment>& getSegments() const
	{
		return segments;
	}
	const std::vector<VecF>& getWallPoints() const
	{
		return wallPoints;
	}
private:
	static constexpr float width = 100.0f;
	static constexpr float height = 25.0f;
	std::vector<RectF> walls = {
		RectF(VecF(100.0f,100.0f), width, height),
		RectF(VecF(150.0f,250.0f), width, height),
		RectF(VecF(100.0f,400.0f), width, height),
		RectF(VecF(100.0f,500.0f), width, height),

		RectF(VecF(300.0f,100.0f), width, height),
		RectF(VecF(350.0f,200.0f), width, height),
		RectF(VecF(250.0f,300.0f), width, height),
		RectF(VecF(330.0f,400.0f), width, height),

		RectF(VecF(500.0f,100.0f), width, height),
		RectF(VecF(450.0f,250.0f), width, height),
		RectF(VecF(500.0f,400.0f), width, height),
		RectF(VecF(520.0f,500.0f), width, height),

		RectF(VecF(700.0f,100.0f), height, width),
		RectF(VecF(750.0f,250.0f), height, width),
		RectF(VecF(700.0f,400.0f), height, width),

	};
	std::vector<VecF> wallPoints;
	std::vector<Segment> segments;
};