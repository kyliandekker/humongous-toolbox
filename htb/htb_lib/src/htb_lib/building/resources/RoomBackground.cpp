#include "./RoomBackground.h"

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
	// RoomBackground
	//======================================================================================
	RoomBackground::RoomBackground() : GameResource()
	{}

	//======================================================================================
	RoomBackground::RoomBackground(parsing::Chunk& a_RMIMChunk)
	{
		if (a_RMIMChunk.GetTag() != parsing::RMIM_CHUNK_ID)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not construct room background because the provided chunk was not a RMIM chunk.");
			return;
		}
		m_pChunk = &a_RMIMChunk;

		m_pRMIHChunk = m_pChunk->TryFindChild(parsing::RMIH_CHUNK_ID);
		if (!m_pRMIHChunk)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not construct room background because the RMIH chunk could not be found.");
			return;
		}

		m_pBSMAPChunk = m_pChunk->TryFindChild({ parsing::BMAP_CHUNK_ID, parsing::SMAP_CHUNK_ID });
		if (!m_pBSMAPChunk)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not construct room background because there were no BMAPs or SMAPs.");
			return;
		}

		m_bValid = true;
	}

	//======================================================================================
	void RoomBackground::ReplaceBackground(RoomBackground& a_RoomBackground)
	{
		m_pChunk->SetData(a_RoomBackground.m_pChunk->GetData());
		m_pRMIHChunk->SetData(a_RoomBackground.m_pRMIHChunk->GetData());
		m_pBSMAPChunk->SetData(a_RoomBackground.m_pBSMAPChunk->GetData());
		//m_pAPALChunk->SetData(a_RoomBackground.m_pAPALChunk->GetData());
		//m_pTRNSChunk->SetData(a_RoomBackground.m_pTRNSChunk->GetData());
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
