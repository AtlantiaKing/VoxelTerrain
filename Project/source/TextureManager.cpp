#include "pch.h"
#include "TextureManager.h"
#include "Texture.h"
#include <cassert>
#include "BlockType.h"

dae::TextureManager* dae::TextureManager::m_pInstance{};

dae::TextureManager::TextureManager(ID3D11Device* pDevice)
{
	m_pTextures[BlockType::DIRT].push_back(Texture::LoadFromFile(pDevice, "Resources/Dirt.png"));
	m_pTextures[BlockType::WATER].push_back(Texture::LoadFromFile(pDevice, "Resources/Water.png"));
	m_pTextures[BlockType::GRASS].push_back(Texture::LoadFromFile(pDevice, "Resources/GrassSide.png"));
	m_pTextures[BlockType::GRASS].push_back(Texture::LoadFromFile(pDevice, "Resources/Grass.png"));
	m_pTextures[BlockType::GRASS].push_back(Texture::LoadFromFile(pDevice, "Resources/Dirt.png"));
	m_pTextures[BlockType::SNOW].push_back(Texture::LoadFromFile(pDevice, "Resources/Snow.png"));
	m_pTextures[BlockType::WOOD].push_back(Texture::LoadFromFile(pDevice, "Resources/Wood.png"));
	m_pTextures[BlockType::LEAVES].push_back(Texture::LoadFromFile(pDevice, "Resources/Leaves.png"));
	m_pTextures[BlockType::SAND].push_back(Texture::LoadFromFile(pDevice, "Resources/Sand.png"));
}

dae::TextureManager* dae::TextureManager::GetInstance()
{
	return m_pInstance;
}

dae::TextureManager::~TextureManager()
{
	for (auto& pair : m_pTextures)
	{
		for (Texture* pTexture : pair.second)
		{
			delete pTexture;
		}
	}
}

const std::vector<dae::Texture*>& dae::TextureManager::GetTextures(BlockType type) const
{
	auto it = m_pTextures.find(type);
	assert(it != m_pTextures.end());
	return it->second;
}

void dae::TextureManager::CreateInstance(ID3D11Device* pDevice)
{
	m_pInstance = new TextureManager{ pDevice };
}
