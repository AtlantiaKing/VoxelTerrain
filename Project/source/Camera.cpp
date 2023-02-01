#include "pch.h"
#include "Camera.h"

namespace dae
{
	Camera::Camera(const Vector3& _origin, float _fovAngle) :
		m_Origin{ _origin }
	{
		ChangeFOV(_fovAngle);
	}

	void Camera::Initialize(float _fovAngle, Vector3 _origin, float _aspectRatio)
	{
		m_Origin = _origin;

		m_AspectRatio = _aspectRatio;

		// Needs to be called last because ChangeFOV calls CalculateProjectionMatrix which needs the aspectRatio
		ChangeFOV(_fovAngle);
	}

	void Camera::CalculateViewMatrix()
	{
		m_Right = Vector3::Cross(Vector3::UnitY, m_Forward).Normalized();
		m_Up = Vector3::Cross(m_Forward, m_Right);

		m_InvViewMatrix = Matrix
		{
			m_Right,
			m_Up,
			m_Forward,
			m_Origin
		};

		m_ViewMatrix = Matrix::Inverse(m_InvViewMatrix);

		//ViewMatrix => Matrix::CreateLookAtLH(...) [not implemented yet]
		//DirectX Implementation => https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixlookatlh
	}

	void Camera::CalculateProjectionMatrix()
	{
		m_ProjectionMatrix = Matrix::CreatePerspectiveFovLH(m_Fov, m_AspectRatio, m_NearPlane, m_FarPlane);
		//DirectX Implementation => https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3dxmatrixperspectivefovlh
	}

	void Camera::Update(const Timer* pTimer)
	{
		//Camera Update Logic
		const float deltaTime = pTimer->GetElapsed();

		// Keyboard Input
		const uint8_t* pKeyboardState = SDL_GetKeyboardState(nullptr);

		// Mouse Input
		int mouseX{}, mouseY{};
		const uint32_t mouseState = SDL_GetRelativeMouseState(&mouseX, &mouseY);

		// Speed and limit constants
		const float keyboardMovementSpeed{ 10.0f };
		const float fovChangeSpeed{ 50.0f };
		const float minFov{ 30.0f };
		const float maxFov{ 170.0f };
		const float mouseMovementSpeed{ 2.0f };
		const float angularSpeed{ 360 * TO_RADIANS };

		// The total movement of this frame
		Vector3 direction{};

		// Calculate new position with keyboard inputs
		direction += (pKeyboardState[SDL_SCANCODE_W] || pKeyboardState[SDL_SCANCODE_Z]) * m_Forward * keyboardMovementSpeed * deltaTime;
		direction -= pKeyboardState[SDL_SCANCODE_S] * m_Forward * keyboardMovementSpeed * deltaTime;
		direction -= (pKeyboardState[SDL_SCANCODE_Q] || pKeyboardState[SDL_SCANCODE_A]) * m_Right * keyboardMovementSpeed * deltaTime;
		direction += pKeyboardState[SDL_SCANCODE_D] * m_Right * keyboardMovementSpeed * deltaTime;

		// Calculate new position and rotation with mouse inputs
		switch (mouseState)
		{
		case SDL_BUTTON_LMASK: // LEFT CLICK
			direction -= m_Forward * (mouseY * mouseMovementSpeed * deltaTime);
			m_TotalYaw += mouseX * angularSpeed * deltaTime;
			break;
		case SDL_BUTTON_RMASK: // RIGHT CLICK
			m_TotalYaw += mouseX * angularSpeed * deltaTime;
			m_TotalPitch -= mouseY * angularSpeed * deltaTime;
			break;
		case SDL_BUTTON_X2: // BOTH CLICK
			direction.y -= mouseY * mouseMovementSpeed * deltaTime;
			break;
		}
		m_TotalPitch = std::clamp(m_TotalPitch, -89.0f * TO_RADIANS, 89.0f * TO_RADIANS);

		// Speed up all movement when the shift button is pressed
		const float speedUpFactor{ 4.0f };
		direction *= 1.0f + pKeyboardState[SDL_SCANCODE_LSHIFT] * (speedUpFactor - 1.0f);

		// Apply the direction to the current position
		m_Origin += direction;

		// Calculate the rotation matrix with the new pitch and yaw
		Matrix rotationMatrix = Matrix::CreateRotationX(m_TotalPitch) * Matrix::CreateRotationY(m_TotalYaw);

		// Calculate the new forward vector with the new pitch and yaw
		m_Forward = rotationMatrix.TransformVector(Vector3::UnitZ);

		//Update Matrices
		CalculateViewMatrix();
	}

	void Camera::ChangeFOV(float newFov)
	{
		m_FovAngle = newFov;
		m_Fov = tanf((m_FovAngle * TO_RADIANS) / 2.f);

		CalculateProjectionMatrix();
	}
}