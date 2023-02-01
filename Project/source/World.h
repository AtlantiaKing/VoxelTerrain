#pragma once

#include <functional>

namespace dae
{
	class Block;
	class Face;

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
		const static int m_MapSize{ 16 };
		const static int m_MapHeight{ 256 };
		Block* m_pBlocks[m_MapSize * m_MapSize * m_MapHeight]{};
		std::function<bool(const Vector3Int& position)> m_IsBlockPredicate{};
	};
}

