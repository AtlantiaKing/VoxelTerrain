#pragma once
#include <cassert>
#include <SDL_keyboard.h>
#include <SDL_mouse.h>
#include <algorithm>

#include "Math.h"
#include "Timer.h"

namespace dae
{
	class Camera final
	{
	public:
		Camera() = default;

		Camera(const Vector3& _origin, float _fovAngle);

		void Initialize(float _fovAngle = 90.f, Vector3 _origin = { 0.f,0.f,0.f }, float _aspectRatio = 1.0f);
		void Update(const Timer* pTimer);
		void ChangeFOV(float newFov);

		Matrix& GetViewMatrix() { return m_ViewMatrix; };
		Matrix& GetInverseViewMatrix() { return m_InvViewMatrix; };
		Matrix& GetProjectionMatrix() { return m_ProjectionMatrix; };
	private:
		Vector3 m_Origin{};
		float m_FovAngle{90.f};
		float m_Fov{ tanf((m_FovAngle * TO_RADIANS) / 2.f) };

		Vector3 m_Forward{ Vector3::UnitZ };
		Vector3 m_Up{ Vector3::UnitY };
		Vector3 m_Right{ Vector3::UnitX };

		float m_TotalPitch{};
		float m_TotalYaw{};

		float m_NearPlane{ 0.1f };
		float m_FarPlane{ 100.0f };

		float m_AspectRatio{ 1.0f };

		Matrix m_InvViewMatrix{};
		Matrix m_ViewMatrix{};
		Matrix m_ProjectionMatrix{};

		void CalculateViewMatrix();
		void CalculateProjectionMatrix();
	};
}
