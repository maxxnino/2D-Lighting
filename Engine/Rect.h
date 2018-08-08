#pragma once
#include "Vec2.h"

template <typename T>
class Rect_
{
public:
	Rect_<T>() = default;
	Rect_<T>(T left, T top, T right, T bottom)
		:
		left(left),
		top(top),
		right(right),
		bottom(bottom)
	{}

	Rect_<T>(const Vec2_<T> & topleft, const Vec2_<T> & bottomright)
		:
		Rect_<T>(topleft.x, topleft.y, bottomright.x, bottomright.y)
	{}

	Rect_<T>(const Vec2_<T> & topleft, T width, T height)
		:
		Rect_<T>(topleft, topleft + Vec2_<T>(width, height))
	{}
	template <typename S>
	Rect_(const Rect_<S>& rhs)
		:
		left((T)rhs.left),
		top((T)rhs.top),
		right((T)rhs.right),
		bottom((T)rhs.bottom)
	{}

	Rect_<T> GetRectCenter(const Vec2_<T> & center, T halfwidth, T halfheight)
	{
		Vec2_<T> half = Vec2_<T>(halfwidth, halfheight);
		return Rect_<T>(center - half, center + half);
	}

	Vec2_<T> GetCenter() const
	{
		return Vec2_<T>((left + right) / (T)2, (top + bottom) / (T)2);
	}

	bool isContaint(Vec2_<T> pos)
	{
		return pos.x >= left && pos.x <= right && pos.y >= top && pos.y <= bottom;
	}
	bool isOverLap(const Rect_<T> & rect) const
	{
		return left < rect.right
			&& right > rect.left
			&& bottom > rect.top
			&& top < rect.bottom;
	}
	T GetWidth() const
	{
		return right - left;
	}
	T GetHeight() const
	{
		return bottom - top;
	}
	Vec2_<T> TopLeft() const
	{
		return{ left,top };
	}
	Vec2_<T> BottomRight() const
	{
		return{ right,bottom };
	}

public:
	T left;
	T right;
	T top;
	T bottom;
};
typedef Rect_<int> RectI;
typedef Rect_<float> RectF;
