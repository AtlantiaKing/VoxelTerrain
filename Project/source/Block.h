#pragma once
#include "Matrix.h"

namespace dae
{
	class Face;

	class Block final
	{
	public:
		Block(const Vector3 position);

		void Render(ID3D11DeviceContext* pDeviceContext, const Matrix& viewProjection, Face* pFace) const;
	private:
		Matrix m_TranslationMatrix{ Vector3::UnitX, Vector3::UnitY, Vector3::UnitZ, Vector3::Zero };
	};
}

