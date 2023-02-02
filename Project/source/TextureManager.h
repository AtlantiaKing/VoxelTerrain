#pragma once

#include <unordered_map>
#include <vector>
#include "BlockType.h"

namespace dae
{
	class Texture;

	class TextureManager final
	{
	private:
		static TextureManager* m_pInstance;
		TextureManager(ID3D11Device* pDevice);
	public:
		~TextureManager();

		static void CreateInstance(ID3D11Device* pDevice);
		static TextureManager* GetInstance();

		const std::vector<Texture*>& GetTextures(BlockType type) const;
	private:

		std::unordered_map<BlockType, std::vector<Texture*>> m_pTextures{};
	};
}
