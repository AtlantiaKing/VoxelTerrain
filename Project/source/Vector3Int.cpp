#include "pch.h"

#include "Vector3Int.h"

#include <cassert>

#include "Vector2Int.h"

namespace dae {
	const Vector3Int Vector3Int::UnitX = Vector3Int{ 1, 0, 0 };
	const Vector3Int Vector3Int::UnitY = Vector3Int{ 0, 1, 0 };
	const Vector3Int Vector3Int::UnitZ = Vector3Int{ 0, 0, 1 };
	const Vector3Int Vector3Int::Zero = Vector3Int{ 0, 0, 0 };

	Vector3Int::Vector3Int(int _x, int _y, int _z) : x(_x), y(_y), z(_z){}

	Vector3Int::Vector3Int(const Vector3Int& from, const Vector3Int& to) : x(to.x - from.x), y(to.y - from.y), z(to.z - from.z){}

	Vector3Int::Vector3Int(const Vector3& v) : x(static_cast<int>(v.x)), y(static_cast<int>(v.y)), z(static_cast<int>(v.z)) {}

	Vector2Int Vector3Int::GetXY() const
	{
		return { x, y };
	}

#pragma region Operator Overloads
	Vector3Int Vector3Int::operator*(int scale) const
	{
		return { x * scale, y * scale, z * scale };
	}

	Vector3Int Vector3Int::operator/(int scale) const
	{
		return { x / scale, y / scale, z / scale };
	}

	Vector3Int Vector3Int::operator+(const Vector3Int& v) const
	{
		return { x + v.x, y + v.y, z + v.z };
	}

	Vector3Int Vector3Int::operator-(const Vector3Int& v) const
	{
		return { x - v.x, y - v.y, z - v.z };
	}

	Vector3Int Vector3Int::operator-() const
	{
		return { -x ,-y,-z };
	}

	Vector3Int& Vector3Int::operator*=(int scale)
	{
		x *= scale;
		y *= scale;
		z *= scale;
		return *this;
	}

	Vector3Int& Vector3Int::operator/=(int scale)
	{
		x /= scale;
		y /= scale;
		z /= scale;
		return *this;
	}

	Vector3Int& Vector3Int::operator-=(const Vector3Int& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	Vector3Int& Vector3Int::operator+=(const Vector3Int& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	int& Vector3Int::operator[](int index)
	{
		assert(index <= 2 && index >= 0);

		if (index == 0) return x;
		if (index == 1) return y;
		return z;
	}

	int Vector3Int::operator[](int index) const
	{
		assert(index <= 2 && index >= 0);

		if (index == 0) return x;
		if (index == 1) return y;
		return z;
	}
#pragma endregion
}
