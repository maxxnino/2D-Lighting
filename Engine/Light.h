#pragma once
#include "Graphics.h"
#include <vector>
#include "wall.h"
#include <queue>
#include <memory>
#include <random>

class Light
{
public:
	Light(VecF pos, Color lightColor, bool isMouse = false, int radian = 200, VecF vel = { 200.0f, 200.0f })
		:
		pos(pos),
		lightColor(lightColor),
		isMouse(isMouse),
		radian(radian),
		vel(vel)
	{
		lightMap.resize(radian * radian * 4);
		std::fill(lightMap.begin(), lightMap.end(), 0.0f);
	}
	void Draw(Graphics& gfx);
	void Update(const Wall& walls, const VecF& mousePos, float dt, std::mt19937& rng);
	const float& GetLightMap(int x, int y) const;
private:
	template <typename T>
	inline T DotProduct(const T& x1, const T& y1, const T& x2, const T& y2) const
	{
		return x1 * x2 + y1 * y2;
	}
	template <typename T>
	inline T CrossProduct(const T& x1, const T& y1, const T& x2, const T& y2) const
	{
		return x1 * y2 - x2 * y1;
	}
	void BuildScreenAndWallPoints();
	void BuildVertexShadow();
	void BuildSpecialCaseVertexShadow();
	VecF PointIntersect(const VecF& wallPos, const VecF& segmentA, const VecF& segmentB) const;
	void FindClosestIntersect(const VecF & point, const std::vector<Wall::Segment>& segments);
	void SortByAngle();
	void UpdateLightMap();
	void PutLightMap(int x, int y, float c);
	void UpdatePos(float dt, const Wall& walls, std::mt19937& rng);
	void DrawTriangle(const VecF& v0, const VecF& v1, const VecF& v2);
	void DrawFlatTopTriangle(const VecF& v0, const VecF& v1, const VecF& v2);
	void DrawFlatBottomTriangle(const VecF& v0, const VecF& v1, const VecF& v2);
	void ResetLightMap();
private:
	VecF pos;
	//new
	RectF rectLight;
	std::vector<RectF> screenAndWallRects;
	std::vector<VecF> screenAndWallPoints;
	std::vector<Wall::Segment> screenAndWallSegments;
	//old
	std::vector<VecF> vertexShadow;
	std::vector<VecF> setCCW;
	std::vector<VecF> setCW;
	//color
	/*Color darkRed = Colors::MakeRGB(0u, 108u, 22u);
	Color orange = Colors::MakeRGB(19u, 131u, 7u);
	Color mediumRed = Colors::MakeRGB(57u, 121u, 6u);
	Color lightOrange = Colors::MakeRGB(7u, 131u, 67u);*/
	Color lightColor;

	Color darkRed = Colors::MakeRGB(0u, 108u, 22u);
	Color orange = Colors::MakeRGB(0u, 108u, 22u);
	Color mediumRed = Colors::MakeRGB(0u, 108u, 22u);
	Color lightOrange = Colors::MakeRGB(0u, 108u, 22u);
	std::vector<float> lightMap;
	VecF vel = {200.0f, 200.0f};
	bool isMouse = false;
	int radian = 200;

	float cosCCW = cosf(-0.0000174533);
	float sinCCW = sinf(-0.0000174533);
	float cosCW = cosf(0.0000174533);
	float sinCW = sinf(0.0000174533);
	VecF nullvector = { -1.0f,-1.0f };
};