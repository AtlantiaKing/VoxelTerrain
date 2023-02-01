#include "pch.h"

#include "Vector2Int.h"
#include <cassert>

namespace dae {
	const Vector2Int Vector2Int::UnitX = Vector2Int{ 1, 0 };
	const Vector2Int Vector2Int::UnitY = Vector2Int{ 0, 1 };
	const Vector2Int Vector2Int::Zero = Vector2Int{ 0, 0 };

	Vector2Int::Vector2Int(int _x, int _y) : x(_x), y(_y) {}


	Vector2Int::Vector2Int(const Vector2Int& from, const Vector2Int& to) : x(to.x - from.x), y(to.y - from.y) {}

#pragma region Operator Overloads
	Vector2Int Vector2Int::operator*(int scale) const
	{
		return { x * scale, y * scale };
	}

	Vector2Int Vector2Int::operator/(int scale) const
	{
		return { x / scale, y / scale };
	}

	Vector2Int Vector2Int::operator+(const Vector2Int& v) const
	{
		return { x + v.x, y + v.y };
	}

	Vector2Int Vector2Int::operator-(const Vector2Int& v) const
	{
		return { x - v.x, y - v.y };
	}

	Vector2Int Vector2Int::operator-() const
	{
		return { -x ,-y };
	}

	Vector2Int& Vector2Int::operator*=(int scale)
	{
		x *= scale;
		y *= scale;
		return *this;
	}

	Vector2Int& Vector2Int::operator/=(int scale)
	{
		x /= scale;
		y /= scale;
		return *this;
	}

	Vector2Int& Vector2Int::operator-=(const Vector2Int& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	Vector2Int& Vector2Int::operator+=(const Vector2Int& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	int& Vector2Int::operator[](int index)
	{
		assert(index <= 1 && index >= 0);
		return index == 0 ? x : y;
	}

	int Vector2Int::operator[](int index) const
	{
		assert(index <= 1 && index >= 0);
		return index == 0 ? x : y;
	}
#pragma endregion
}