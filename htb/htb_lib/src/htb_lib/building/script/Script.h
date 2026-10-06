#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <htb_lib/core/Data.h>
#include <htb_lib/script/ScriptArgType.h>

namespace htb::core
{
	class DataStream;
}
namespace htb::parsing
{
	class Chunk;
}
namespace htb::building
{
	class ScriptInstruction;

	//======================================================================================
	// ScriptArg
	//======================================================================================
	/// <summary>
	/// Script argument info for scripts. Gives optional TALK and JUMP info.
	/// </summary>
	struct ScriptArg
	{
		const std::string GetString() const;
		int32_t GetRefJump() const;

		void SetData(const core::Data& a_Data);

		// Public setters and getters.
		void SetByte(uint8_t a_iValue);
		void SetInt16(int16_t a_iValue);
		void SetInt32(int32_t a_iValue);
		int32_t SetString(const std::string& a_sString);
		void SetRefJump(int32_t a_iValue);

		size_t GetOffsetFromInstruction() const;

		core::Data m_Data;
		script::EScriptArgType m_eArgumentType;
		ScriptInstruction* m_pOwnerInstruction = nullptr;

		parsing::Chunk* m_pTALKChunk = nullptr; // Optional.
		ScriptInstruction* m_pJumpTo = nullptr; // Optional.
	};

	//======================================================================================
	// ScriptInstruction
	//======================================================================================
	/// <summary>
	/// Linked list approach for script instructions with size info and data.
	/// </summary>
	struct ScriptInstruction
	{
		uint8_t m_iCode = 0;
		ScriptInstruction* m_pPrevious = nullptr;
		ScriptInstruction* m_pNext = nullptr;
		std::vector<ScriptArg> m_aArgs;

		size_t GetSize() const;
		size_t GetDataSize() const;
		size_t GetOffsetFromFirstInstruction() const;
		void ToData(core::DataStream& a_Data) const;
	};

	//======================================================================================
	// Script
	//======================================================================================
	/// <summary>
	/// Contains a bunch of instructions with arguments. Can be easily converted to data.
	/// </summary>
	struct Script
	{
		parsing::Chunk* m_pChunk = nullptr;
		std::vector<std::unique_ptr<ScriptInstruction>> m_aInstructions;

		ScriptInstruction* GetInstructionAtOffset(size_t a_iStartingPoint);
		size_t GetSize() const;
		void ToData(core::DataStream& a_Data) const;
	};
}