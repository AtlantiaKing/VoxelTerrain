#pragma once
#include <fstream>
#include "Math.h"
#include <vector>

namespace dae
{
	namespace Utils
	{
		Vector2 RandomGradient(int ix, int iy)
		{
			// No precomputed gradients mean this works for any number of grid coordinates
			const unsigned w = 8 * sizeof(unsigned);
			const unsigned s = w / 2; // rotation width
			unsigned a = ix, b = iy;
			a *= 3284157443; b ^= a << s | a >> w - s;
			b *= 1911520717; a ^= b << s | b >> w - s;
			a *= 2048419325;
			float random = a * (3.14159265 / ~(~0u >> 1)); // in [0, 2*Pi]
			Vector2 v;
			v.x = cos(random); v.y = sin(random);
			return v;
		}

		float Lerp(float x, float y, float a)
		{
			return x + a * (y - x);
		}

		float PerlinFunction(float x, float y, int octave, float zoom, float offset)
		{
			x += offset;
			y += offset;

			x *= powf(2.0f, static_cast<float>(octave - 1));
			y *= powf(2.0f, static_cast<float>(octave - 1));

			x /= zoom;
			y /= zoom;

			const int gridX0{ static_cast<int>(x) };
			const int gridX1{ gridX0 + 1 };
			const int gridY0{ static_cast<int>(y) };
			const int gridY1{ gridY0 + 1 };

			const float gridPosX{ x - gridX0 };
			const float gridPosY{ y - gridY0 };

			const float dotGradient0{ Vector2::Dot(RandomGradient(gridX0, gridY0), Vector2{ gridPosX, gridPosY }) };
			const float dotGradient1{ Vector2::Dot(RandomGradient(gridX1, gridY0), Vector2{ gridPosX - 1.0f, gridPosY }) };
			const float dotGradient2{ Vector2::Dot(RandomGradient(gridX0, gridY1), Vector2{ gridPosX, gridPosY - 1.0f }) };
			const float dotGradient3{ Vector2::Dot(RandomGradient(gridX1, gridY1), Vector2{ gridPosX - 1.0f, gridPosY - 1.0f }) };

			const float xEase{ 3.0f * powf(gridPosX, 2.0f) - 2.0f * powf(gridPosX, 3.0f) };
			const float yEase{ 3.0f * powf(gridPosY, 2.0f) - 2.0f * powf(gridPosY, 3.0f) };

			return Lerp(Lerp(dotGradient0, dotGradient1, xEase), Lerp(dotGradient2, dotGradient3, xEase), yEase);
		}
	}
}