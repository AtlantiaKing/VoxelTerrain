#pragma once
#include "Matrix.h"
#include "Vector3Int.h"
#include <functional>

namespace dae
{
	class Face;
	class Texture;

	class Block final
	{
	public:
		Block() = default;
		Block(const Vector3Int& position, Texture* pTexture);

		void Render(ID3D11DeviceContext* pDeviceContext, std::function<bool(const Vector3Int&)> airPredicate, const Matrix& viewProjection, Face* pFace) const;
	private:
		Texture* m_pTexture{};
		Matrix m_TranslationMatrix{ Vector3::UnitX, Vector3::UnitY, Vector3::UnitZ, Vector3::Zero };
		Matrix m_FaceTransformations[6]{};
		static bool m_Initialized;
		static Vector3Int m_NeighbouringBlocks[6];
	};
}

