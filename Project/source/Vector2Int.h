#pragma once

namespace dae
{
	struct Vector2Int
	{
		int x{};
		int y{};

		Vector2Int() = default;
		Vector2Int(int _x, int _y);
		Vector2Int(const Vector2Int& from, const Vector2Int& to);

		//Member Operators
		Vector2Int operator*(int scale) const;
		Vector2Int operator/(int scale) const;
		Vector2Int operator+(const Vector2Int& v) const;
		Vector2Int operator-(const Vector2Int& v) const;
		Vector2Int operator-() const;
		//Vector2Int& operator-();
		Vector2Int& operator+=(const Vector2Int& v);
		Vector2Int& operator-=(const Vector2Int& v);
		Vector2Int& operator/=(int scale);
		Vector2Int& operator*=(int scale);
		int& operator[](int index);
		int operator[](int index) const;

		static const Vector2Int UnitX;
		static const Vector2Int UnitY;
		static const Vector2Int Zero;
	};

	//Global Operators
	inline Vector2Int operator*(int scale, const Vector2Int& v)
	{
		return { v.x * scale, v.y * scale };
	}

	inline bool operator==(const Vector2Int& v0, const Vector2Int& v1)
	{
		return { v0.x == v1.x && v0.y == v1.y };
	}
}
