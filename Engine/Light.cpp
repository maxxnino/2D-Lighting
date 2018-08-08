#include "Light.h"

void Light::Draw(Graphics & gfx)
{
	for (int y = 0; y < radian * 2; y++)
	{
		for (int x = 0; x < radian * 2; x++)
		{
			float colorStreng = GetLightMap(x, y);
			if (colorStreng > 0.0f)
			{
				const VecI worldPos = VecI((int)pos.x - radian + x, (int)pos.y - radian + y);
				if (worldPos.x >= 0 && worldPos.y >= 0)
				{
					const Color dest = gfx.GetPixel(worldPos.x, worldPos.y);
					const Color blend = {
						unsigned char(std::min((lightColor.GetR() * colorStreng + dest.GetR()) ,255.0f)),
						unsigned char(std::min((lightColor.GetG() * colorStreng + dest.GetG()) ,255.0f)),
						unsigned char(std::min((lightColor.GetB() * colorStreng + dest.GetB()) ,255.0f))
					};
					gfx.PutPixel(worldPos.x, worldPos.y, blend);
				}
			}
		}
	}
}

void Light::Update(const Wall& walls, const VecF& newpos, float dt, std::mt19937& rng)
{
	ResetLightMap();
	if (isMouse)
	{
		pos = newpos;
		screenAndWallRects.clear();
		rectLight.left = std::max(0.0f, pos.x - (float)radian);
		rectLight.top = std::max(0.0f, pos.y - (float)radian);
		rectLight.right = std::min((float)(Graphics::ScreenWidth - 1), pos.x + (float)radian);
		rectLight.bottom = std::min((float)(Graphics::ScreenHeight - 1), pos.y + (float)radian);
		screenAndWallRects.emplace_back(rectLight);
		for (const auto& wallrect : walls.getRectWalls())
		{
			if (rectLight.isOverLap(wallrect))
			{
				screenAndWallRects.emplace_back(wallrect);
			}
		}
	}
	else
	{
		UpdatePos(dt, walls, rng);
	}
	BuildScreenAndWallPoints();
	BuildVertexShadow();
	SortByAngle();
	BuildSpecialCaseVertexShadow();
	UpdateLightMap();
}

void Light::UpdatePos(float dt, const Wall& walls, std::mt19937& rng)
{
	std::uniform_real_distribution<float> change(-0.5f, 0.5f);
	std::uniform_int_distribution<int> changeDir(1, 10);
	const int chamgeRandom = changeDir(rng);
	if (chamgeRandom >= 5 )
	{
		vel.x += 50.0f * change(rng);
	}
	else
	{
		vel.y += 50.0f * change(rng);
	}
	pos += vel * dt;


	//do collision
	RectF rect = { pos.x - 5.0f,pos.y - 5.0f,pos.x + 5.0f,pos.y + 5.0f };
	for (const auto& w : walls.getRectWalls())
	{
		if (w.isOverLap(rect))
		{
			if (vel.x > 0)
			{
				float px = w.right - w.left;
				if (vel.y > 0)
				{
					float py = rect.bottom - w.top;
					//px/vel.x > py/vel.y
					//top collision
					if (abs(vel.y) * px > abs(vel.x) * py)
					{
						pos.y -= py;
						vel.y = -vel.y;
					}
					//left collision
					else
					{
						pos.x -= px;
						vel.x = -vel.x;
					}
				}
				else
				{
					float py = w.bottom - rect.top;
					//px/vel.x > py/vel.y
					//bottom collision
					if (abs(vel.y) * px > abs(vel.x) * py)
					{
						pos.y += py;
						vel.y = -vel.y;
					}
					//left collision
					else
					{
						pos.x -= px;
						vel.x = -vel.x;
					}
				}
			}
			else
			{
				float px = w.right - rect.left;
				if (vel.y > 0)
				{
					float py = rect.bottom - w.top;
					//px/vel.x > py/vel.y
					//top collision
					if (abs(vel.y) * px > abs(vel.x) * py)
					{
						pos.y -= py;
						vel.y = -vel.y;
					}
					//right collision
					else
					{
						pos.x += px;
						vel.x = -vel.x;
					}
				}
				else
				{
					float py = w.bottom - rect.top;
					//px/vel.x > py/vel.y
					//bottom collision
					if (abs(vel.y) * px > abs(vel.x) * py)
					{
						pos.y += py;
						vel.y = -vel.y;
					}
					//right collision
					else
					{
						pos.x += px;
						vel.x = -vel.x;
					}
				}
			}
		}
	}


	//screen collision
	if (pos.x < 5.0f)
	{
		pos.x = 5.0f;
		vel.x = -vel.x + 50.0f * change(rng);
	}
	else if (pos.x >= float(Graphics::ScreenWidth) - 5.0f)
	{
		pos.x = float(Graphics::ScreenWidth - 6.0f);
		vel.x = -vel.x * (float)change(rng) / 5.0f;
	}

	if (pos.y < 5.0f)
	{
		pos.y = 5.0f;
		vel.y = -vel.y + 50.0f * change(rng);
	}
	else if (pos.y >= float(Graphics::ScreenHeight) - 5.0f)
	{
		pos.y = float(Graphics::ScreenHeight - 6.0f);
		vel.y = -vel.y + 50.0f * change(rng);
	}

	//after collision
	screenAndWallRects.clear();
	rectLight.left = std::max(0.0f, pos.x - (float)radian);
	rectLight.top = std::max(0.0f, pos.y - (float)radian);
	rectLight.right = std::min((float)(Graphics::ScreenWidth - 1), pos.x + (float)radian);
	rectLight.bottom = std::min((float)(Graphics::ScreenHeight - 1), pos.y + (float)radian);
	screenAndWallRects.emplace_back(rectLight);
	for (const auto& wallrect : walls.getRectWalls())
	{
		if (rectLight.isOverLap(wallrect))
		{
			screenAndWallRects.emplace_back(wallrect);
		}
	}
	screenAndWallRects.emplace_back(rectLight);
}

void Light::BuildScreenAndWallPoints()
{
	screenAndWallPoints.clear();
	screenAndWallSegments.clear();
	
	for (const auto& w : screenAndWallRects)
	{
		screenAndWallPoints.emplace_back(std::move(VecF(w.left, w.top)));
		screenAndWallPoints.emplace_back(std::move(VecF(w.left, w.bottom)));
		screenAndWallPoints.emplace_back(std::move(VecF(w.right, w.bottom)));
		screenAndWallPoints.emplace_back(std::move(VecF(w.right, w.top)));
	}
	for (size_t i = 0; i < screenAndWallPoints.size(); i += 4)
	{
		screenAndWallSegments.emplace_back(screenAndWallPoints[i], screenAndWallPoints[i + 1]);
		screenAndWallSegments.emplace_back(screenAndWallPoints[i + 1], screenAndWallPoints[i + 2]);
		screenAndWallSegments.emplace_back(screenAndWallPoints[i + 2], screenAndWallPoints[i + 3]);
		screenAndWallSegments.emplace_back(screenAndWallPoints[i + 3], screenAndWallPoints[i]);
	}
}

void Light::BuildVertexShadow()
{
	vertexShadow.clear();
	for (const auto& p : screenAndWallPoints)
	{
		FindClosestIntersect(p, screenAndWallSegments);
	}
}

void Light::BuildSpecialCaseVertexShadow()
{
	setCCW.clear();
	setCW.clear();
	std::vector<VecF> CCWqueue;
	std::vector<VecF> CWqueue;
	for (const auto& p : vertexShadow)
	{
		for (int i = 0; i < screenAndWallPoints.size(); i++)
		{
			if (p == screenAndWallPoints[i])
			{
				const VecF rotatePos = p - pos;
				const int curBlock = (i / 4) * 4;
				//CCW
				const VecF pCCW = { rotatePos.x * cosCCW - rotatePos.y * sinCCW + pos.x, rotatePos.x * sinCCW + rotatePos.y * cosCCW + pos.y };
				bool isAddCCW = true;
				for (int i = 0; i < 4; i++)
				{
					if (PointIntersect(pCCW, screenAndWallSegments[curBlock + i].pointA, screenAndWallSegments[curBlock + i].pointB) != nullvector)
					{
						isAddCCW = false;
						break;
					}
				}
				if (isAddCCW)
				{
					if (std::any_of(CCWqueue.begin(), CCWqueue.end(), [p](const VecF& lhs) {return p == lhs; }))
					{
						isAddCCW = false;
					}
					if (isAddCCW)
					{
						CCWqueue.emplace_back(p);
					}
				}
				//CW
				const VecF pCW = { rotatePos.x * cosCW - rotatePos.y * sinCW + pos.x, rotatePos.x * sinCW + rotatePos.y * cosCW + pos.y };
				bool isAddCW = true;
				for (int i = 0; i < 4; i++)
				{
					if (PointIntersect(pCW, screenAndWallSegments[curBlock + i].pointA, screenAndWallSegments[curBlock + i].pointB) != nullvector)
					{
						isAddCW = false;
						break;
					}
				}
				if (isAddCW)
				{
					if (std::any_of(CWqueue.begin(), CWqueue.end(), [p](const VecF& lhs) {return p == lhs; }))
					{
						isAddCW = false;
					}
					if (isAddCW)
					{
						CWqueue.emplace_back(p);
					}
				}
				break;
			}
		}
	}
	for (const auto& pointCCW : CCWqueue)
	{
		VecF closestIntersect = nullvector;
		for (int i = 0; i < screenAndWallSegments.size(); i++)
		{
			const VecF intersect = PointIntersect(pointCCW, screenAndWallSegments[i].pointA, screenAndWallSegments[i].pointB);
			if (intersect == nullvector) continue;
			bool isContinute = false;
			const int curBlock = (i / 4) * 4;
			if (curBlock < screenAndWallSegments.size())
			{
				for (int step = 0; step < 4; step++)
				{
					if (screenAndWallPoints[curBlock + step] == intersect)
					{
						isContinute = true;
						break;
					}
				}
				if (isContinute) continue;
			}
			if (closestIntersect == nullvector)
			{
				closestIntersect = intersect;
			}
			else
			{
				const float len1 = (pos - intersect).GetLengthSq();
				const float len2 = (pos - closestIntersect).GetLengthSq();
				if (len1 <= len2)
				{
					closestIntersect = intersect;
				}
			}
		}
		if (closestIntersect != nullvector)
		{
			setCCW.emplace_back(pointCCW);
			setCCW.emplace_back(closestIntersect);

		}
	}
	for (const auto& pointCW : CWqueue)
	{
		VecF closestIntersect = nullvector;
		for (int i = 0; i < screenAndWallSegments.size(); i++)
		{
			const VecF intersect = PointIntersect(pointCW, screenAndWallSegments[i].pointA, screenAndWallSegments[i].pointB);
			if (intersect == nullvector) continue;
			bool isContinute = false;
			const int curBlock = (i / 4) * 4;
			if (curBlock < screenAndWallPoints.size())
			{
				for (int step = 0; step < 4; step++)
				{
					if (screenAndWallPoints[curBlock + step] == intersect)
					{
						isContinute = true;
						break;
					}
				}
				if (isContinute) continue;
			}
			if (closestIntersect == nullvector)
			{
				closestIntersect = intersect;
			}
			else
			{
				const float len1 = (pos - intersect).GetLengthSq();
				const float len2 = (pos - closestIntersect).GetLengthSq();
				if (len1 <= len2)
				{
					closestIntersect = intersect;
				}
			}
		}
		if (closestIntersect != nullvector)
		{
			setCW.emplace_back(pointCW);
			setCW.emplace_back(closestIntersect);
		}
	}
}

void Light::FindClosestIntersect(const VecF & point, const std::vector<Wall::Segment>& segments)
{
	VecF closestIntersect = nullvector;
	for (int i = 0; i < segments.size(); i++)
	{
		const VecF intersect = PointIntersect(point, segments[i].pointA, segments[i].pointB);
		if (intersect == nullvector) continue;
		if (closestIntersect == nullvector)
		{
			closestIntersect = intersect;
			continue;
		}
		else
		{
			const float len1 = (pos - intersect).GetLengthSq();
			const float len2 = (pos - closestIntersect).GetLengthSq();
			if (len1 < len2)
			{
				closestIntersect = intersect;
			}
		}
	}
	if (closestIntersect == nullvector)
	{
		vertexShadow.emplace_back(point);
	}
	else
	{
		vertexShadow.emplace_back(closestIntersect);
	}
}

void Light::UpdateLightMap()
{
	const int sizePoint = (int)vertexShadow.size();
	for (int i = 0; i < sizePoint; i++)
	{
		bool drawyellow = true;
		for (int ccw = 0; ccw < setCCW.size(); ccw += 2)
		{
			if (vertexShadow[i] == setCCW[ccw])
			{
				int prev = i - 1;

				if (prev < 0)
				{
					prev = sizePoint - 1;
				}
				bool isDraw = true;
				for (int cw = 0; cw < setCW.size(); cw += 2)
				{
					if (vertexShadow[prev] == setCW[cw])
					{
						DrawTriangle(pos, setCCW[ccw + 1], setCW[cw + 1]);
						isDraw = false;
						break;
					}
				}
				if (isDraw)
				{
					DrawTriangle(pos, vertexShadow[prev], setCCW[ccw + 1]);
					break;
				}
			}
		}
		for (int cw = 0; cw < setCW.size(); cw += 2)
		{
			if (vertexShadow[i] == setCW[cw])
			{
				int front = i + 1;
				if (front >= sizePoint)
				{
					front = 0;
				}
				bool isDraw = true;
				for (int ccw = 0; ccw < setCCW.size(); ccw += 2)
				{
					if (vertexShadow[front] == setCCW[ccw])
					{
						isDraw = false;
						drawyellow = false;
						break;
					}
				}
				if (isDraw)
				{
					DrawTriangle(pos, vertexShadow[front], setCW[cw + 1]);
					break;
				}
			}
		}
		if (drawyellow)
		{
			if (i >= sizePoint - 1)
			{
				DrawTriangle(pos, vertexShadow.back(), vertexShadow.front());
			}
			else
			{
				DrawTriangle(pos, vertexShadow[i], vertexShadow[i + 1]);
			}
		}
	}
}

const float & Light::GetLightMap(int x, int y) const
{
	return lightMap[radian * 2 * y + x];
}

void Light::PutLightMap(int x, int y, float c)
{
	lightMap[radian * 2 * y + x] = c;
}

void Light::ResetLightMap()
{
	std::fill(lightMap.begin(), lightMap.end(), 0.0f);
}

VecF Light::PointIntersect(const VecF & wallPos, const VecF & segmentA, const VecF & segmentB) const
{
	const VecF v1 = pos - segmentA;
	const VecF v2 = segmentB - segmentA;
	const VecF dir = wallPos - pos;
	VecF v3 = { -dir.y ,dir.x };

	/*|v2xv1|
	int a = abs(CrossProduct(v2.x, v2.y, v1.x, v1.y));
	v2.v3
	int b = DotProduct(v2.x, v2.y, v3.x, v3.y);
	v1.v3
	int c = DotProduct(v1.x, v1.y, v3.x, v3.y);
	v2.v3
	int d = DotProduct(v2.x, v2.y, v3.x, v3.y);*/

	const float a = CrossProduct(v2.x, v2.y, v1.x, v1.y);
	const float b = DotProduct(v2.x, v2.y, v3.x, v3.y);

	if (a > 0.0f)
	{
		v3 = { dir.y , -dir.x };
	}

	// t1 = a / b && t1 >= 0
	//no intersect
	if (b == 0.0f) return nullvector;

	const float t1 = a / b;
	if (t1 >= 0.0f)
	{
		//t2 = c / d  && 0 <= t2 <= 1
		const float d = DotProduct(v2.x, v2.y, v3.x, v3.y);

		//no intersect
		if (d == 0.0f) return nullvector;

		const float c = DotProduct(v1.x, v1.y, v3.x, v3.y);
		const float t2 = c / d;
		if (t2 >= 0.0f && t2 <= 1.0f)
		{
			//intersect
			VecF point = { pos.x + t1 * dir.x, pos.y + t1 * dir.y };
			point.x = std::max(0.0f, point.x);
			point.x = std::min(point.x, (float)(Graphics::ScreenWidth - 1));
			point.y = std::max(0.0f, point.y);
			point.y = std::min(point.y, (float)(Graphics::ScreenHeight - 1));
			return point;
		}
	}
	//no intersect
	return nullvector;
}

void Light::SortByAngle()
{
	auto pred = [this](const VecF& lhs, const VecF& rhs)
	{
		const VecF v1 = lhs - pos;
		const VecF v2 = rhs - pos;
		if (v1.x >= 0 && v2.x < 0) return true;
		if (v1.x < 0 && v2.x >= 0) return false;
		if (v1.x == 0 && v2.x == 0)
		{
			if (v1.y >= 0 || v2.y >= 0) return lhs.y > rhs.y;
			return rhs.y > lhs.y;
		}

		// compute the cross product of vectors (center -> a) x (center -> b)
		const float det = v1.x * v2.y - v2.x * v1.y;
		if (det < 0) return true;
		if (det > 0) return false;

		// points a and b are on the same line from the center
		// check which point is closer to the center
		float d1 = v1.x * v1.x + v1.y * v1.y;
		float d2 = v2.x * v2.x + v2.y * v2.y;
		return d1 > d2;
	};
	std::priority_queue<VecF, std::vector<VecF>, decltype(pred)> queue(pred);
	for (const auto& vs : vertexShadow)
	{
		queue.push(vs);
	}

	vertexShadow.clear();
	while (!queue.empty())
	{
		vertexShadow.emplace_back(queue.top());
		queue.pop();
	}
}