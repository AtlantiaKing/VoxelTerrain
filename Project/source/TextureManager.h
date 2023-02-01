#pragma once

#include <unordered_map>

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
			WATER
		};

		~TextureManager();

		static void CreateInstance(ID3D11Device* pDevice);
		static TextureManager* GetInstance();

		Texture* GetTexture(TextureType type);
	private:

		std::unordered_map<TextureType, Texture*> m_pTextures{};
	};
}
