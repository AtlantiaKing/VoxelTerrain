#pragma once

#include <unordered_map>
#include <vector>

namespace dae
{
	class Texture;

	class TextureManager final
	{
	private:
		static TextureManager* m_pInstance;
		TextureManager(ID3D11Device* pDevice);
	public:
		enum class TextureType : BYTE
		{
			DIRT,
			WATER,
			GRASS,
			SNOW,
			WOOD,
			LEAVES
		};

		~TextureManager();

		static void CreateInstance(ID3D11Device* pDevice);
		static TextureManager* GetInstance();

		const std::vector<Texture*>& GetTextures(TextureType type) const;
	private:

		std::unordered_map<TextureType, std::vector<Texture*>> m_pTextures{};
	};
}
