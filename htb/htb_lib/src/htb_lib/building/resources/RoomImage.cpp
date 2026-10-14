#include "./RoomImage.h"

#include <htb_lib/core/DataStream.h>
#include <htb_lib/core/Log.h>
#include <htb_lib/parsing/Chunk.h>
#include <htb_lib/parsing/ChunkIDs.h>
#include <htb_lib/parsing/chunks/image/IMHD_Chunk.h>

namespace htb::building
{
	constexpr uint8_t STRIP_WIDTH = 8;

#pragma pack(push, 1)
	//======================================================================================
	// PaletteColor
	//======================================================================================
	/// <summary>
	/// Used for the APAL chunk data.
	/// </summary>
	// The APAL chunk data consists of an array of PaletteColors. Usually it is 256 colors, but it is not a given.
	struct PaletteColor
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
	};

	//======================================================================================
	// TRNSData
	//======================================================================================
	/// <summary>
	/// The data for the TRNS chunk.
	/// </summary>
	struct TRNSData
	{
		uint8_t trnsVal = 0; 
	};

	//======================================================================================
	// IMHDData
	//======================================================================================
	/// <summary>
	/// This chunk contains information about the room images such as width and height and where on the screen they should appear.
	/// It is usually found in an OBIM chunk.
	/// It is used for multiple images.
	/// </summary>
	struct IMHDData
	{
		uint16_t id = 0;
		uint16_t num_imnn = 0;
		uint16_t nums_zpnn = 0;
		uint8_t flags = 0;
		uint8_t unknown = 0;
		uint16_t x = 0;
		uint16_t y = 0;
		uint16_t width = 0;
		uint16_t height = 0;
	};
#pragma pack(pop)

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
	// RoomImage
	//======================================================================================
	RoomImage::RoomImage() : GameResource()
	{}

	//======================================================================================
	RoomImage::RoomImage(parsing::Chunk& a_OBIMChunk)
	{
		if (a_OBIMChunk.GetTag() != parsing::OBIM_CHUNK_ID)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not construct room image because the provided chunk was not an OBIM chunk.");
			return;
		}
		m_pChunk = &a_OBIMChunk;

		parsing::Chunk* IMxxChunk = m_pChunk->TryFindChild(
			{
				parsing::IM00_CHUNK_ID,
				parsing::IM01_CHUNK_ID,
				parsing::IM02_CHUNK_ID,
				parsing::IM03_CHUNK_ID,
				parsing::IM04_CHUNK_ID,
				parsing::IM05_CHUNK_ID,
				parsing::IM06_CHUNK_ID,
				parsing::IM07_CHUNK_ID,
				parsing::IM08_CHUNK_ID,
				parsing::IM09_CHUNK_ID,
				parsing::IM10_CHUNK_ID,
				parsing::IM11_CHUNK_ID,
				parsing::IM12_CHUNK_ID,
				parsing::IM13_CHUNK_ID,
				parsing::IM14_CHUNK_ID,
				parsing::IM15_CHUNK_ID,
				parsing::IM16_CHUNK_ID,
				parsing::IM17_CHUNK_ID,
				parsing::IM0A_CHUNK_ID,
				parsing::IM0B_CHUNK_ID,
				parsing::IM0C_CHUNK_ID,
				parsing::IM0D_CHUNK_ID,
				parsing::IM0E_CHUNK_ID,
				parsing::IM0F_CHUNK_ID,
			}
		);
		if (!IMxxChunk)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not construct room image because the IMxx chunk could not be found.");
			return;
		}

		m_pIMHDChunk = m_pChunk->TryFindChild(parsing::IMHD_CHUNK_ID);
		if (!m_pIMHDChunk)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not construct room image because the IMHD chunk could not be found.");
			return;
		}

		const IMHDData* imhdData = m_pIMHDChunk->GetData().dataAs<IMHDData>();
		if (!imhdData)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not construct room image because the IMHD data was invalid.");
			return;
		}

		parsing::Chunk* RMDAChunk = m_pChunk->GetParent();
		if (RMDAChunk->GetTag() != parsing::RMDA_CHUNK_ID)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not construct room image because the RMDA chunk could not be found.");
			return;
		}

		m_pAPALChunk = RMDAChunk->TryFindChild(parsing::APAL_CHUNK_ID);
		if (!m_pAPALChunk)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not construct room image because the APAL chunk could not be found.");
			return;
		}

		m_pTRNSChunk = RMDAChunk->TryFindChild(parsing::TRNS_CHUNK_ID);
		if (!m_pTRNSChunk)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not construct room image because the TRNS chunk could not be found.");
			return;
		}

		if (!m_pChunk->TryFindChildren({
			parsing::BMAP_CHUNK_ID,
			parsing::SMAP_CHUNK_ID,
			}, m_aImages))
		{
			core::Log(core::ELogLevel::_ERROR, "Could not construct room image because no BMAP or SMAP chunk were found.");
			return;
		}

		const size_t num_strips = static_cast<size_t>(imhdData->width / STRIP_WIDTH);

		m_bValid = true;
	}

	//======================================================================================
	void RoomImage::ReplaceImage(size_t a_iIndex, const core::Data& a_ChunkData)
	{
		m_aImages[a_iIndex]->SetData(a_ChunkData);
	}

	//======================================================================================
	void RoomImage::ReplaceImage(RoomImage& a_RoomImage)
	{
		m_pChunk->SetData(a_RoomImage.m_pChunk->GetData());
		m_pIMHDChunk->SetData(a_RoomImage.m_pIMHDChunk->GetData());
		//m_pAPALChunk->SetData(a_RoomImage.m_pAPALChunk->GetData());
		//m_pTRNSChunk->SetData(a_RoomImage.m_pTRNSChunk->GetData());

		for (size_t i = 0; i < m_aImages.size(); i++)
		{
			if (a_RoomImage.m_aImages.size() <= i)
			{
				return;
			}
			m_aImages[i]->SetData(a_RoomImage.m_aImages[i]->GetData());
		}
	}

	// BMAP (single code byte at start of chunk, decodes to full width x height image)
	// width/height come from IMHD, NOT from the chunk itself (unlike BOMP)
	// palen = code % 10, same rule as SMAP
	// no strip table, no direction, no raw/basic/run codes - always fills the whole image
	//
	// val			Compression				dir				tr						palen / notes
	// 134			decode_he				n/a				no						palen 4  (16 colors)
	// 135			decode_he				n/a				no						palen 5  (32 colors)
	// 136			decode_he				n/a				no						palen 6  (64 colors)
	// 137			decode_he				n/a				no						palen 7  (128 colors)
	// 138			decode_he				n/a				no						palen 8  (256 colors)
	// 144			decode_he				n/a				intended				palen 4, transparency NOT implemented
	// 145			decode_he				n/a				intended				palen 5, transparency NOT implemented
	// 146			decode_he				n/a				intended				palen 6, transparency NOT implemented
	// 147			decode_he				n/a				intended				palen 7, transparency NOT implemented
	// 148			decode_he				n/a				intended				palen 8, transparency NOT implemented
	// 150			solid fill				n/a				no						no compression, next byte = one color for the whole image

	// SMAP strip code (first byte of every 8px strip). Rules: get_method_info
	// palen = code % 10 = bits used to write one color number (used by basic/run/HE, ignored by raw)
	// dir   = VERTICAL only for code 3-18 and 34-38, else HORIZONTAL
	// tr    = transparency, ON for code 34-48, 84-128 and >=143
	//
	// val			Compression				dir				transparent				palen / notes
	// 1			raw						horizontal		no						ignored (computes 1)
	// 14-18		decode_basic			vertical		no						= last digit (4-8)
	// 19-33		decode_basic			horizontal		no						= last digit
	// 34-38		decode_basic			vertical		YES						= last digit (4-8)
	// 39-48		decode_basic			horizontal		YES						= last digit (4-8)
	// 64-69		decode_run_majmin		horizontal		no						= last digit; run limit 255 (v5, e.g. 68)
	// 80-83		decode_run_majmin		horizontal		no						= last digit; run limit 255 (v5)
	// 84-89		decode_run_majmin		horizontal		YES						= last digit; run limit 255 (v5)
	// 100-109		decode_run_majmin		horizontal		YES						= last digit; run limit 12 (v6+, e.g. 108)
	// 120-128		decode_run_majmin		horizontal		YES						= last digit; run limit 12 (v6+)
	// 134			decode_he				horizontal		no						palen 4  (16 colors)
	// 135			decode_he				horizontal		no						palen 5  (32 colors)
	// 136			decode_he				horizontal		no						palen 6  (64 colors)
	// 137			decode_he				horizontal		no						palen 7  (128 colors)
	// 138			decode_he				horizontal		no						palen 8  (256 colors)
	// 139-143		decode_he				horizontal		143 only				palen 9,0,1,2,3 - unused in practice
	// 144			decode_he				horizontal		YES						palen 4, transparent
	// 145			decode_he				horizontal		YES						palen 5, transparent
	// 146			decode_he				horizontal		YES						palen 6, transparent
	// 147			decode_he				horizontal		YES						palen 7, transparent
	// 148			decode_he				horizontal		YES						palen 8, transparent
	// 149			raw						horizontal		(set, unused)			ignored (computes 9)
	// 150			ERROR
	// 70-79		ERROR
	// 90-99		ERROR
	// 110-119		ERROR
	// everything else is an error (unknown_decoder)
}
