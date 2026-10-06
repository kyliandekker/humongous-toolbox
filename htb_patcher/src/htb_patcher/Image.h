#pragma once

#include <cstdint>
#include <d3d11.h>

#include <htb_lib/core/Data.h>

namespace htb::parsing
{
	class Chunk;
}
namespace htb::patch
{
	//======================================================================================
	// Image
	//======================================================================================
	/// <summary>
	/// Image resource contains all image data to display.
	/// </summary>
	class Image
	{
	public:
		Image(parsing::Chunk* a_pChunk);
		const core::Data& GetImageData() const;

		uint16_t GetWidth() const;
		uint16_t GetHeight() const;

		ID3D11ShaderResourceView* GetSRV();

		bool Load();
	protected:
		void ToGrayscale();

		parsing::Chunk* m_pDataChunk = nullptr;
		core::Data m_ImageData;
		uint16_t m_iWidth = 0;
		uint16_t m_iHeight = 0;
		ID3D11ShaderResourceView* m_pSRV = nullptr;
	};
}