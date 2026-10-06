#include "./Image.h"

#include <vector>

#include <htb_lib/core/DataStream.h>
#include <htb_lib/core/Memory.h>
#include <htb_lib/parsing/Chunk.h>
#include <htb_lib/parsing/ChunkIDs.h>
#include <htb_lib/parsing/chunks/image/APAL_Chunk.h>
#include <htb_lib/parsing/chunks/image/BMAP_Chunk.h>
#include <htb_lib/parsing/chunks/image/IMHD_Chunk.h>
#include <htb_lib/parsing/chunks/image/RMHD_Chunk.h>
#include <htb_lib/parsing/chunks/image/TRNS_Chunk.h>

#include <htb_lib_win32/dx11/DX11System.h>

namespace htb::patch
{
	//======================================================================================
	static core::Data CreateBitstream(const core::Data a_Data)
	{
		core::DataStream outBits = core::DataStream(a_Data.size() * 8);

		for (size_t i = 0; i < a_Data.size(); ++i)
		{
			const char c = a_Data[i];
			for (int j = 0; j < 8; j++)
			{
				uint8_t bit = (c >> j) & 1;
				outBits.Write(&bit, sizeof(bit));
			}
		}
		return outBits;
	}

	//======================================================================================
	static uint8_t CollectBits(int& a_Pos, const core::Data& a_Bitstream, int a_Count)
	{
		int result = 0;
		for (int i = 0; i < a_Count; i++)
		{
			result |= a_Bitstream[a_Pos++] << i;
		}

		return result;
	}

	//======================================================================================
	static bool DecodeHE(uint8_t a_FillColor, const core::Data& a_BMAPData, uint16_t a_iWidth, uint16_t a_iHeight, int a_iPalen, core::Data& a_OutData, bool a_bTransparent)
	{
		std::vector<uint8_t> out;

		const size_t num_pixels = a_iWidth * a_iHeight;
		out.reserve(num_pixels);

		if (a_BMAPData.size() == 0)
		{
			for (size_t i = 0; i < num_pixels; i++)
			{
				out.push_back(a_FillColor % 256);
			}
		}
		else
		{
			std::vector<int> delta_color = { -4, -3, -2, -1, 1, 2, 3, 4 };

			core::Data bits = CreateBitstream(a_BMAPData);

			out.push_back(a_FillColor % 256);

			int pos = 0;
			while (out.size() < num_pixels)
			{
				if (bits[pos++] == 1)
				{
					if (bits[pos++] == 1)
					{
						const uint8_t bitc = CollectBits(pos, bits, 3);
						a_FillColor += delta_color[bitc];
					}
					else
					{
						a_FillColor = CollectBits(pos, bits, a_iPalen);
					}
				}
				out.push_back(a_FillColor % 256);
			};
		}

		a_OutData = core::Data(out.data(), out.size());
		return true;
	}

	//======================================================================================
	static bool DecodeMajmin(uint8_t a_FillColor, const core::Data& a_BMAPData, uint16_t a_iWidth, uint16_t a_iHeight, int a_iPalen, core::Data& a_OutData, bool a_bTransparent)
	{
		std::vector<uint8_t> out;

		const size_t numPixels = a_iWidth * a_iHeight;
		out.reserve(numPixels);

		if (a_BMAPData.size() == 0)
		{
			for (size_t i = 0; i < numPixels; i++)
			{
				out.push_back(a_FillColor % 256);
			}
		}
		else
		{
			core::Data bits = CreateBitstream(a_BMAPData);

			out.push_back(a_FillColor % 256);

			const size_t num_pixels = a_iWidth * a_iHeight;
			out.reserve(num_pixels);

			int pos = 0;
			while (out.size() < num_pixels)
			{
				if (bits[pos++] == 1)
				{
					if (bits[pos++] == 1)
					{
						const uint8_t shift = CollectBits(pos, bits, 3) - 4;
						if (shift != 0)
						{
							a_FillColor += shift;
						}
						else
						{
							uint8_t ln = CollectBits(pos, bits, 8) - 1;
							for (size_t i = 0; i < ln; i++)
							{
								out.push_back((a_FillColor % 256));
							}
						}
					}
					else
					{
						a_FillColor = CollectBits(pos, bits, a_iPalen);
					}
				}
				out.push_back(a_FillColor % 256);
			};
		}

		a_OutData = core::Data(out.data(), out.size());
		return true;
	}

	//======================================================================================
	void Image::ToGrayscale()
	{
		const size_t expected = static_cast<size_t>(m_iWidth) * static_cast<size_t>(m_iHeight) * 4;
		if (m_ImageData.size() < expected)
		{
			return;
		}

		unsigned char* data = m_ImageData.dataAs<unsigned char>();
		const size_t numPixels = m_iWidth * m_iHeight;
		for (size_t i = 0; i < numPixels; i++)
		{
			uint8_t r = data[i * 4];
			uint8_t g = data[i * 4 + 1];
			uint8_t b = data[i * 4 + 2];
			uint8_t gray = static_cast<uint8_t>(0.299f * r + 0.587f * g + 0.114f * b);
			data[i * 4] = gray;
			data[i * 4 + 1] = gray;
			data[i * 4 + 2] = gray;
		}

		m_pSRV = nullptr;
	}
	
	//======================================================================================
	static bool DecodeBasic(uint8_t a_FillColor, const core::Data& a_BMAPData, uint16_t a_iWidth, uint16_t a_iHeight, int a_iPalen, core::Data& a_OutData, bool a_bTransparent)
	{
		std::vector<uint8_t> out;

		const size_t numPixels = a_iWidth * a_iHeight;
		out.reserve(numPixels);

		if (a_BMAPData.size() == 0)
		{
			for (size_t i = 0; i < numPixels; i++)
			{
				out.push_back(a_FillColor % 256);
			}
		}
		else
		{
			core::Data bits = CreateBitstream(a_BMAPData);

			out.push_back(a_FillColor % 256);

			const size_t num_pixels = a_iWidth * a_iHeight;
			out.reserve(num_pixels);

			int sub = 1;
			int pos = 0;
			while (out.size() < num_pixels)
			{
				if (bits[pos++] == 1)
				{
					if (bits[pos++] == 1)
					{
						if (bits[pos++] == 1)
						{
							sub = -sub;
						}
						a_FillColor -= sub;
					}
					else
					{
						a_FillColor = CollectBits(pos, bits, a_iPalen);
						sub = 1;
					}
				}
				out.push_back(a_FillColor % 256);
			};
		}

		a_OutData = core::Data(out.data(), out.size());
		return true;
	}

	//======================================================================================
	static bool DecodeRaw(const core::Data& a_BMAPData, core::Data& a_OutData)
	{
		a_OutData = a_BMAPData;
		return true;
	}

	//======================================================================================
	Image::Image(parsing::Chunk* a_pChunk)
	{
		m_pDataChunk = a_pChunk;
	}

	//======================================================================================
	const core::Data& Image::GetImageData() const
	{
		return m_ImageData;
	}

	//======================================================================================
	uint16_t Image::GetWidth() const
	{
		return m_iWidth;
	}

	//======================================================================================
	uint16_t Image::GetHeight() const
	{
		return m_iHeight;
	}

	//======================================================================================
	ID3D11ShaderResourceView* Image::GetSRV()
	{
		if (m_pSRV || m_ImageData.empty())
		{
			return m_pSRV;
		}

		// Validate that ImageData is RGBA (4 bytes per pixel). If it's still
		// 1 byte per pixel (palette indices), CreateTexture2D would read
		// past the allocation (width*4 pitch vs width*1 buffer) and AV in
		// atidxx64.dll - the crash reported at DX11System.cpp:126.
		const size_t expected = static_cast<size_t>(m_iWidth) * static_cast<size_t>(m_iHeight) * 4;
		if (m_ImageData.size() < expected)
		{
			return nullptr;
		}

		m_pSRV = dx11::GetDX11System().CreateTexture(
			m_ImageData.data(),
			m_iWidth,
			m_iHeight
		);

		return m_pSRV;
	}

	//======================================================================================
	bool Image::Load()
	{
		// BMAP is the data chunk.
		parsing::Chunk* bmap = m_pDataChunk->TryFindChild(parsing::BMAP_CHUNK_ID);
		parsing::BMAP_Chunk* bmapData = bmap->GetData().dataAs<parsing::BMAP_Chunk>();

		const int palen = bmapData->encoding % 10;

		// Determine which version.
		const bool he = bmapData->encoding >= 0x86 && bmapData->encoding <= 0x8A;
		const bool he_transparent = bmapData->encoding >= 0x90 && bmapData->encoding <= 0x94;

		// Get the RMIH and RMHD chunks.
		parsing::Chunk* rmih = m_pDataChunk->GetParent()->GetParent()->TryFindChild(parsing::RMDA_CHUNK_ID);

		parsing::Chunk* rmhd = rmih->TryFindChild(parsing::RMHD_CHUNK_ID);
		if (!rmhd)
		{
			return false;
		}

		parsing::RMHD_Chunk* rmhdData = rmhd->GetData().dataAs<parsing::RMHD_Chunk>();
		if (!rmhdData)
		{
			return false;
		}

		m_iWidth = rmhdData->width;
		m_iHeight = rmhdData->height;

		parsing::Chunk* apal = rmih->TryFindChild(parsing::APAL_CHUNK_ID);
		if (!apal)
		{
			return false;
		}

		parsing::APAL_Chunk* apalData = apal->GetData().dataAs<parsing::APAL_Chunk>();
		if (!apalData)
		{
			return false;
		}

		parsing::Chunk* trns = rmih->TryFindChild(parsing::TRNS_CHUNK_ID);
		if (!trns)
		{
			return false;
		}

		parsing::TRNS_Chunk* trnsData = trns->GetData().dataAs<parsing::TRNS_Chunk>();
		if (!trnsData)
		{
			return false;
		}

		size_t bmapSize = 0;
		if (bmap->ChunkSize() >= sizeof(parsing::BMAP_Chunk))
		{
			bmapSize = bmap->ChunkSize() - sizeof(parsing::BMAP_Chunk);
		}
		if (bmapSize > 0)
		{
			core::Data bmapImageData = core::Data(core::add(bmap->GetData().data(), sizeof(parsing::BMAP_Chunk)), bmapSize);
			if (!DecodeHE(bmapData->fillColor, bmapImageData, m_iWidth, m_iHeight, palen, m_ImageData, he_transparent))
			{
				return false;
			}
		}
		else
		{
			// No BMAP data -> no room background (valid case). Leave m_ImageData empty
			// so GetSRV() returns nullptr instead of trying to create a 0-byte texture.
			m_ImageData = core::Data();
		}

		if (!m_ImageData.empty())
		{
			std::vector<uint8_t> newOut;
			newOut.reserve(m_ImageData.size() * 4);
			for (size_t i = 0; i < m_ImageData.size(); i++)
			{
				uint8_t idx = m_ImageData.dataAs<unsigned char>()[i];
				newOut.push_back(apalData->data[idx * 3]);
				newOut.push_back(apalData->data[idx * 3 + 1]);
				newOut.push_back(apalData->data[idx * 3 + 2]);
				if (m_ImageData[i] == bmapData->fillColor && he_transparent)
				{
					newOut.push_back(0);
				}
				else
				{
					newOut.push_back(255);
				}
			}

			m_ImageData = core::Data(newOut.data(), newOut.size());

			ToGrayscale();
		}

		return true;
	}
}