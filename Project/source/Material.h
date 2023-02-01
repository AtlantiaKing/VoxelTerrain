#pragma once

namespace dae
{
	class Texture;

	class Material
	{
	public:
		Material(ID3D11Device* pDevice, const std::wstring& assetFile);
		virtual ~Material();

		Material(const Material&)				= delete;
		Material(Material&&)					= delete;
		Material& operator=(const Material&)	= delete;
		Material& operator=(Material&&)			= delete;

		virtual void SetMatrix(const Matrix& matrix);
		virtual void SetTexture(Texture* pTexture) = 0;
		ID3DX11Effect* GetEffect() const;
		ID3DX11EffectTechnique* GetTechnique() const;

		ID3D11InputLayout* LoadInputLayout(ID3D11Device* pDevice);
	protected:
		ID3DX11Effect* m_pEffect{};
		ID3DX11EffectTechnique* m_pTechnique{};
		ID3DX11EffectMatrixVariable* m_pMatWorldViewProjVariable{};
		ID3DX11EffectSamplerVariable* m_pSamplerStateVariable{};

		static ID3DX11Effect* LoadEffect(ID3D11Device* pDevice, const std::wstring& assetFile);
	};
}

