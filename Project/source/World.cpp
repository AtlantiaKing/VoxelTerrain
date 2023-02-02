#include "pch.h"
#include "World.h"
#include "Vector3Int.h"
#include "Block.h"
#include "TextureManager.h"
#include "Utils.h"
#include "Vector2Int.h"

dae::World::World()
{
	m_IsBlockPredicate = [&](const Vector3Int& position) -> bool
	{
		const Vector2Int chunkPos{ position.x / m_MapSize, position.z / m_MapSize };

		auto it = m_Chunks.find(chunkPos);
		if (it == m_Chunks.end()) return false;
		
		Chunk* chunk{ (*it).second };

		const Vector3Int lookUpPos{ position.x - chunkPos.x * m_MapSize, position.y, position.z - chunkPos.y * m_MapSize };

		return lookUpPos.x >= 0 && lookUpPos.z >= 0 &&
			lookUpPos.x < m_MapSize&& lookUpPos.z < m_MapSize&&
			lookUpPos.y < m_MapHeight&& lookUpPos.y >= 0 &&
			chunk->pBlocks[lookUpPos.x + lookUpPos.z * m_MapSize + lookUpPos.y * m_MapSize * m_MapSize];
	};
}

dae::World::~World()
{
	for (auto chunkPair : m_Chunks)
	{
		delete chunkPair.second;
	}
}

void dae::World::Update(const Vector3& camPosition)
{
	const Vector2Int camChunk{ static_cast<int>(camPosition.x / m_MapSize), static_cast<int>(camPosition.z / m_MapSize) };

	constexpr int destroyRange{ 5 };
	constexpr int createRange{ 4 };
	constexpr int renderRange{ 3 };

	for (int x{ -destroyRange }; x <= destroyRange; ++x)
	{
		for (int z{ -destroyRange }; z <= destroyRange; ++z)
		{
			if (abs(x) <= createRange && abs(z) <= createRange)
			{
				const Vector2Int chunkPos{ camChunk + Vector2Int{ x,z } };

				auto it = m_Chunks.find(chunkPos);
				if (it != m_Chunks.end())
				{
					if (abs(x) <= renderRange && abs(z) <= renderRange)
					{
						it->second->canRender = true;
					}
					else
					{
						it->second->canRender = false;
					}
				}
				else
				{
					LoadChunk(chunkPos.x, chunkPos.y);
				}
			}
			else
			{
				auto it = m_Chunks.find(camChunk + Vector2Int{ x,z });
				if (it == m_Chunks.end()) continue;

				Chunk* chunkToErase{ (*it).second };
				m_Chunks.erase(it);
				delete chunkToErase;
			}
		}
	}

	for (int x{ -renderRange }; x <= renderRange; ++x)
	{
		for (int z{ -renderRange }; z <= renderRange; ++z)
		{
			LoadTrees(camChunk.x + x, camChunk.y + z);
		}
	}
}

void dae::World::Render(ID3D11DeviceContext* pDeviceContext, const Matrix& viewProjection, Face* pFace) const
{
	for (auto& chunkPair : m_Chunks)
	{
		Chunk* chunk{ chunkPair.second };

		if (!chunk->canRender) continue;

		for (int x{}; x < m_MapSize; ++x)
		{
			for (int z{}; z < m_MapSize; ++z)
			{
				for (int y{}; y < m_MapHeight; ++y)
				{
					Block* pBlock{ chunk->pBlocks[x + z * m_MapSize + y * m_MapSize * m_MapSize] };

					if (!pBlock) continue;

					const Vector3Int position{ chunkPair.first.x * m_MapSize + x, y, chunkPair.first.y * m_MapSize + z };

					if (m_IsBlockPredicate(position + Vector3Int::UnitX) && m_IsBlockPredicate(position - Vector3Int::UnitX) &&
						m_IsBlockPredicate(position + Vector3Int::UnitY) && m_IsBlockPredicate(position - Vector3Int::UnitY) &&
						m_IsBlockPredicate(position + Vector3Int::UnitZ) && m_IsBlockPredicate(position - Vector3Int::UnitZ))
						continue;

					pBlock->Render(pDeviceContext, m_IsBlockPredicate, viewProjection, pFace);

					int i{};
				}
			}
		}
	}
}

void dae::World::LoadChunk(int chunkX, int chunkZ)
{
	const Vector2Int chunkPos{ chunkX, chunkZ };

	Chunk* pChunk{ new Chunk{} };

	for (int x{}; x < m_MapSize; ++x)
	{
		for (int z{}; z < m_MapSize; ++z)
		{
			const int worldPosX{ chunkX * m_MapSize + x };
			const int worldPosZ{ chunkZ * m_MapSize + z };

			float worldLevel{};
			float maxValue{};
			for (int i{ 1 }; i <= m_NrOctaves; ++i)
			{
				maxValue += 1.0f / i;
				worldLevel += Utils::PerlinFunction(static_cast<float>(worldPosX) / m_MapSize, static_cast<float>(worldPosZ) / m_MapSize, i, m_MapZoom, m_MapOffset) / i;
			}
			worldLevel /= (maxValue / 2.0f);
			worldLevel /= 2.0f;
			worldLevel += 0.5f;
			worldLevel *= m_MapHeight;

			bool hasWater{};

			pChunk->heightMap[x + z * m_MapSize] = worldLevel > m_SeaLevel ? worldLevel : m_SeaLevel;

			for (int y{ m_MapHeight }; y >= 0; --y)
			{
				if (y > worldLevel && y > m_SeaLevel) continue;

				TextureManager::TextureType textureType{};

				/*if (y > snowLevel)
				{
					textureType = TextureManager::TextureType::SNOW;
				}
				else*/
				if (y == static_cast<int>(worldLevel))
				{
					if (hasWater)
					{
						textureType = TextureManager::TextureType::SAND;
					}
					else
					{
						if (y < m_SeaLevel + m_BeachSize)
						{
							textureType = TextureManager::TextureType::SAND;
						}
						else
						{
							textureType = TextureManager::TextureType::GRASS;
						}
					}

				}
				else if (y > worldLevel)
				{
					textureType = TextureManager::TextureType::WATER;

					hasWater = true;
				}
				else if(y < m_SeaLevel + m_BeachSize)
				{
					textureType = TextureManager::TextureType::SAND;
				}
				else
				{
					textureType = TextureManager::TextureType::DIRT;
				}

				const std::vector<Texture*>& pTextures{ TextureManager::GetInstance()->GetTextures(textureType) };

				Texture* pMainTexture{ pTextures[0] };
				Texture* pTopTexture{};
				Texture* pBottomTexture{};
				if (pTextures.size() > 1)
				{
					pTopTexture = pTextures[1];
					pBottomTexture = pTextures[2];
				}

				pChunk->pBlocks[x + z * m_MapSize + y * m_MapSize * m_MapSize] = new Block{ { worldPosX, y, worldPosZ }, pMainTexture, pTopTexture, pBottomTexture };
			}
		}
	}

	m_Chunks[chunkPos] = pChunk;
}

void dae::World::LoadTrees(int chunkX, int chunkY)
{
	Chunk* pChunk{ m_Chunks[Vector2Int{ chunkX, chunkY }] };

	if (pChunk->hasTrees) return;

	pChunk->hasTrees = true;

	for (int x{}; x < m_MapSize; ++x)
	{
		for (int z{}; z < m_MapSize; ++z)
		{
			if (!HasTree(Vector2Int{ x + chunkX * m_MapSize,  z + chunkY * m_MapSize })) continue;

			const int y{ pChunk->heightMap[x + z * m_MapSize] };

			if (y <= m_SeaLevel + m_BeachSize) continue;

			CreateTree(Vector3Int{ x + chunkX * m_MapSize, y + 1, z + chunkY * m_MapSize }, pChunk);

			break;
		}
	}
}

bool dae::World::HasTree(const Vector2Int& position)
{
	constexpr float treeZoom{ 0.01f };

	float worldLevel{};
	float maxValue{};
	for (int i{ 1 }; i <= m_NrOctaves; ++i)
	{
		maxValue += 1.0f / i;
		worldLevel += Utils::PerlinFunction(static_cast<float>(position.x) / m_MapSize, static_cast<float>(position.y) / m_MapSize, i, treeZoom, m_MapOffset) / i;
	}
	worldLevel /= (maxValue / 2.0f);

	return worldLevel / 2.0f + 0.5f > 0.7f;
}

void dae::World::CreateTree(const Vector3Int& position, Chunk* pChunk)
{
	Texture* woodTexture{ TextureManager::GetInstance()->GetTextures(TextureManager::TextureType::WOOD)[0] };
	Texture* leavesTexture{ TextureManager::GetInstance()->GetTextures(TextureManager::TextureType::LEAVES)[0] };

	Vector2Int chunkPos{ position.x / m_MapSize, position.z / m_MapSize };
	Vector3Int blockInChunkPos{ position.x - chunkPos.x * m_MapSize, position.y, position.z - chunkPos.y * m_MapSize };

	for (int i{}; i < 5; ++i)
	{
		if (blockInChunkPos.y + i < m_MapHeight)
			AddBlock(position.x, position.y + i, position.z, new Block{ { position.x, position.y + i, position.z }, woodTexture });
	}

	for (int x{ -2 }; x <= 2; ++x)
	{
		for (int z{ -2 }; z <= 2; ++z)
		{
			if (position.y + 3 < m_MapHeight)
				AddBlock(position.x + x, position.y + 3, position.z + z, new Block{ { position.x + x, position.y + 3, position.z + z }, leavesTexture });
		
			if (position.y + 4 < m_MapHeight)
				AddBlock(position.x + x, position.y + 4, position.z + z, new Block{ { position.x + x, position.y + 4, position.z + z }, leavesTexture });
		}
	}

	for (int x{ -1 }; x <= 1; ++x)
	{
		for (int z{ -1 }; z <= 1; ++z)
		{
			if(blockInChunkPos.y + 5 < m_MapHeight)
				AddBlock(position.x + x, position.y + 5, position.z + z, new Block{ { position.x + x, position.y + 5, position.z + z }, leavesTexture });

			if (abs(x) && abs(z)) continue;

			if (blockInChunkPos.y + 6 < m_MapHeight)
				AddBlock(position.x + x, position.y + 6, position.z + z, new Block{ { position.x + x, position.y + 6, position.z + z }, leavesTexture });
		}
	}
}

void dae::World::AddBlock(int x, int y, int z, Block* pBlock)
{
	const Vector2Int chunkPos{ x / m_MapSize, z / m_MapSize };
	const Vector3Int positionInChunk{ x - chunkPos.x * m_MapSize, y, z - chunkPos.y * m_MapSize };

	Chunk* pChunk{ m_Chunks[chunkPos] };

	if (!pChunk) return;

	const int blockIdx{ positionInChunk.x + positionInChunk.z * m_MapSize + y * m_MapSize * m_MapSize };

	if (!pChunk->pBlocks[blockIdx])
		pChunk->pBlocks[blockIdx] = pBlock;
}
