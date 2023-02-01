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
		Block(const Vector3Int& position, Texture* pSideTexture, Texture* pTopTexture = nullptr, Texture* pBottomTexture = nullptr);

		void Render(ID3D11DeviceContext* pDeviceContext, std::function<bool(const Vector3Int&)> airPredicate, const Matrix& viewProjection, Face* pFace) const;
	private:
		Texture* m_pSideTexture{};
		Texture* m_pTopTexture{};
		Texture* m_pBottomTexture{};
		Matrix m_TranslationMatrix{ Vector3::UnitX, Vector3::UnitY, Vector3::UnitZ, Vector3::Zero };
		Matrix m_FaceTransformations[6]{};
		static bool m_Initialized;
		static Vector3Int m_NeighbouringBlocks[6];
	};
}

