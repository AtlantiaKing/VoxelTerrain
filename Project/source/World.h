#pragma once

#include <functional>

namespace dae
{
	class Block;
	class Face;
	struct Vector2Int;

	class World final
	{
	public:
		World();
		~World();

		World(const World&) = delete;
		World(World&&) noexcept = delete;
		World& operator=(const World&) = delete;
		World& operator=(World&&) noexcept = delete;

		void Render(ID3D11DeviceContext* pDeviceContext, const Matrix& viewProjection, Face* pFace) const;
	private:
		const static int m_MapSize{ 64 };
		const static int m_MapHeight{ 64 };
		Block* m_pBlocks[m_MapSize * m_MapSize * m_MapHeight]{};
		std::function<bool(const Vector3Int& position)> m_IsBlockPredicate{};

		const int m_NrOctaves{ 5 };
		const float m_MapZoom{ 1.0f };
		const float m_MapOffset{ 493.0f };

		bool HasTree(const Vector2Int& position);
		void CreateTree(const Vector3Int& position);
	};
}

