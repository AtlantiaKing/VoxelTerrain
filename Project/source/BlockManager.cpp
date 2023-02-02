#include "pch.h"
#include "BlockManager.h"
#include "TextureManager.h"
#include <cassert>

dae::BlockManager* dae::BlockManager::m_pInstance{};

dae::BlockManager::BlockManager()
{
    TextureManager* pTextureManager{ TextureManager::GetInstance() };

    for (int i{}; i <= static_cast<int>(BlockType::SAND); ++i)
    {
        const BlockType type{ static_cast<BlockType>(i) };

        const std::vector<Texture*>& pTextures{ pTextureManager->GetTextures(type) };
        Texture* pSideTexture{ pTextures[0] };
        Texture* pTopTexture{};
        Texture* pBottomTexture{};
        if (pTextures.size() > 1)
        {
            pTopTexture = pTextures[1];
            pBottomTexture = pTextures[2];
        }

        m_pBlocks[type] = new Block{ pSideTexture, pTopTexture, pBottomTexture };
    }
}

dae::BlockManager::~BlockManager()
{
    for (auto& pair : m_pBlocks)
    {
        delete pair.second;
    }
}

dae::BlockManager* dae::BlockManager::GetInstance()
{
    if (!m_pInstance) m_pInstance = new BlockManager();
    return m_pInstance;
}

dae::Block* dae::BlockManager::GetBlock(BlockType type) const
{
    auto it = m_pBlocks.find(type);
    assert(it != m_pBlocks.end());
    return it->second;
}
