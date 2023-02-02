#pragma once

#include <unordered_map>
#include <vector>
#include "Block.h"
#include "BlockType.h"

namespace dae
{
	class BlockManager final
	{
	private:
		static BlockManager* m_pInstance;
		BlockManager();
	public:

		~BlockManager();

		static BlockManager* GetInstance();

		Block* GetBlock(BlockType type) const;
	private:
		std::unordered_map<BlockType, Block*> m_pBlocks{};
	};
}
