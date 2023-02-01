#include "pch.h"
#include "TextureManager.h"
#include "Texture.h"

dae::TextureManager* dae::TextureManager::m_pInstance{};

dae::TextureManager::TextureManager(ID3D11Device* pDevice)
{
	m_pTextures[TextureType::DIRT] = Texture::LoadFromFile(pDevice, "Resources/Dirt.png");
	m_pTextures[TextureType::WATER] = Texture::LoadFromFile(pDevice, "Resources/Water.png");
	m_pTextures[TextureType::GRASS] = Texture::LoadFromFile(pDevice, "Resources/Grass.png");
}

dae::TextureManager* dae::TextureManager::GetInstance()
{
	return m_pInstance;
}

dae::TextureManager::~TextureManager()
{
	for (auto& pair : m_pTextures)
	{
		delete pair.second;
	}
}

dae::Texture* dae::TextureManager::GetTexture(TextureType type)
{
	return m_pTextures[type];
}

void dae::TextureManager::CreateInstance(ID3D11Device* pDevice)
{
	m_pInstance = new TextureManager{ pDevice };
}
