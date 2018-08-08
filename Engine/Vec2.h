#pragma once

template <typename T>
class Vec2_
{
public:
	Vec2_() = default;
	Vec2_(T x_in, T y_in)
		:
		x(x_in),
		y(y_in)
	{
	}
	template <typename S>
	Vec2_(const Vec2_<S>& rhs )
		:
		x((T)rhs.x),
		y((T)rhs.y)
	{}
	Vec2_ operator+(const Vec2_& rhs) const
	{
		return Vec2_(x + rhs.x, y + rhs.y);
	}

	Vec2_& operator+=(const Vec2_& rhs)
	{
		return *this = *this + rhs;
	}
	Vec2_ operator+(const T& rhs) const
	{
		return Vec2_(x + T, y + T);
	}

	Vec2_& operator+=(const T& rhs)
	{
		return *this = *this + rhs;
	}

	Vec2_ operator*(T rhs) const
	{
		return Vec2_(x * rhs, y * rhs);
	}

	Vec2_& operator*=(T rhs)
	{
		return *this = *this * rhs;
	}

	Vec2_ operator-(const Vec2_& rhs) const
	{
		return Vec2_(x - rhs.x, y - rhs.y);
	}

	Vec2_& operator-=(const Vec2_& rhs)
	{
		return *this = *this - rhs;
	}

	T GetLength() const
	{
		return std::sqrt(GetLengthSq());
	}

	T GetLengthSq() const
	{
		return x * x + y * y;
	}

	Vec2_& Normalize()
	{
		return *this = GetNormalized();
	}

	Vec2_ GetNormalized() const
	{
		const T len = GetLength();
		if (len != (T)0)
		{
			return *this * ((T)1 / len);
		}
		return *this;
	}

	Vec2_ InterpolateTo(const Vec2_& dest, T alpha) const
	{
		return *this + (dest - *this) * alpha;
	}

	Vec2_&	operator/=(const T &rhs)
	{
		x /= rhs;
		y /= rhs;
		return *this;
	}
	Vec2_	operator/(const T &rhs) const
	{
		return Vec2_(*this) /= rhs;
	}

	bool operator==(const Vec2_& rhs) const
	{
		return x == rhs.x && y == rhs.y;
	}
	bool operator!=(const Vec2_& rhs) const
	{
		return !(*this == rhs);
	}

public:
	T x;
	T y;
};
typedef Vec2_<int> VecI;
typedef Vec2_<float> VecF;