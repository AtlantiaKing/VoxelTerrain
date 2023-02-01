#pragma once

namespace dae
{
	struct Vector2Int;

	struct Vector3Int
	{
		int x{};
		int y{};
		int z{};

		Vector3Int() = default;
		Vector3Int(int _x, int _y, int _z);
		Vector3Int(const Vector3Int& from, const Vector3Int& to);
		Vector3Int(const Vector3& v);

		Vector2Int GetXY() const;

		//Member Operators
		Vector3Int operator*(int scale) const;
		Vector3Int operator/(int scale) const;
		Vector3Int operator+(const Vector3Int& v) const;
		Vector3Int operator-(const Vector3Int& v) const;
		Vector3Int operator-() const;
		//Vector3Int& operator-();
		Vector3Int& operator+=(const Vector3Int& v);
		Vector3Int& operator-=(const Vector3Int& v);
		Vector3Int& operator/=(int scale);
		Vector3Int& operator*=(int scale);
		int& operator[](int index);
		int operator[](int index) const;

		static const Vector3Int UnitX;
		static const Vector3Int UnitY;
		static const Vector3Int UnitZ;
		static const Vector3Int Zero;
	};

	//Global Operators
	inline Vector3Int operator*(int scale, const Vector3Int& v)
	{
		return { v.x * scale, v.y * scale, v.z * scale };
	}
}
