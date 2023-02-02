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
		Block(Texture* pSideTexture, Texture* pTopTexture = nullptr, Texture* pBottomTexture = nullptr);
		~Block() = default;

		void Render(const std::function<bool(const Vector3Int&)>& airPredicate, const Vector3Int& position, const Matrix& viewProjection, Face* pFace) const;
	private:
		Texture* m_pSideTexture{};
		Texture* m_pTopTexture{};
		Texture* m_pBottomTexture{};
		static Matrix m_TranslationMatrix;
		static bool m_Initialized;
		static Vector3Int m_NeighbouringBlocks[6];
	};
}

