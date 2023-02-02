#pragma once

#include <functional>
#include <unordered_map>
#include "Vector2Int.h"
#include "Block.h"
#include "BlockType.h"

namespace std
{
	template<>
	struct hash<dae::Vector2Int>
	{
		std::size_t operator()(const dae::Vector2Int& v) const
		{
			return std::hash<int>()(v.x) ^ std::hash<int>()(v.y);
		}
	};
}

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

		void Update(const Vector3& camPosition);
		void Render(ID3D11DeviceContext* pDeviceContext, const Matrix& viewProjection, Face* pFace) const;
	private:
		const static int m_MapSize{ 16 };
		const static int m_MapHeight{ 64 };

		struct Chunk
		{
			bool canRender{};
			bool hasTrees{};
			int heightMap[m_MapSize * m_MapSize]{};
			Block* pBlocks[m_MapSize * m_MapSize * m_MapHeight]{};
		};

		std::unordered_map<Vector2Int, Chunk*> m_Chunks{};

		std::function<bool(const Vector3Int& position)> m_IsBlockPredicate{};

		const int m_NrOctaves{ 5 };
		const float m_MapZoom{ 10.0f };
		const float m_MapOffset{ 493.0f };
		const int m_SeaLevel{ 30 };
		const int m_SnowLevel{ 43 };
		const int m_BeachSize{ 2 };
		const int m_RenderDistance{ 3 };

		void LoadChunk(const Vector2Int& chunkPos);
		BlockType GetBiomeBlock(int x, int z);
		void LoadTrees(int chunkX, int chunkY);
		bool HasTree(const Vector2Int& position);
		void CreateTree(const Vector3Int& position, Chunk* pChunk);
		void CreateCactus(const Vector3Int& position, Chunk* pChunk);
		void AddBlock(int x, int y, int z,  Block* pBlock);
	};
}

