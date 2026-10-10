#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <htb_lib/building/GameResource.h>
#include <htb_lib/core/Data.h>
#include <htb_lib/script/OPCodesHENew.h>

namespace htb::core
{
	class DataStream;
}
namespace htb::parsing
{
	class Chunk;
}
namespace htb::script
{
	enum class EScriptArgType;
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
	public:
		ScriptArg(const core::Data& a_Data, script::EScriptArgType a_eArgumentType, ScriptInstruction* a_pOwnerInstruction);

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

		const core::Data& GetData() const;
		script::EScriptArgType GetArgumentType() const;
		ScriptInstruction* GetOwnerInstruction() const;

		parsing::Chunk* GetTALKChunk() const;
		ScriptInstruction* GetJumpTo() const;

		void SetJumpTo(ScriptInstruction* a_pJumpTo);
		void SetTALKChunk(parsing::Chunk* a_pTALKChunk);
	private:
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
	public:
		/// <summary>
		/// Creates the script instruction and parses the arguments.
		/// </summary>
		/// <param name="a_Data">The data chunk.</param>
		/// <param name="a_mOPCodeMap">The script code map.</param>
		ScriptInstruction(core::DataStream& a_Data, const script::OPCodeMap& a_mOPCodeMap);

		size_t GetSize() const;
		size_t GetDataSize() const;
		size_t GetOffsetFromFirstInstruction() const;
		void ToData(core::DataStream& a_Data) const;

		uint8_t GetCode() const;
		ScriptInstruction* GetPrevious() const;
		ScriptInstruction* GetNext() const;
		const std::vector<ScriptArg>& GetArgs() const;
		std::vector<ScriptArg>& GetArgs();

		void SetPreviousInstruction(ScriptInstruction* a_pPrevious);
		void SetNextInstruction(ScriptInstruction* a_pNext);
	private:
		uint8_t m_iCode = 0;
		ScriptInstruction* m_pPrevious = nullptr;
		ScriptInstruction* m_pNext = nullptr;
		std::vector<ScriptArg> m_aArgs;
	};

	//======================================================================================
	// Script
	//======================================================================================
	/// <summary>
	/// Contains the info for a script and can rebuild.
	/// </summary>
	class Script : public GameResource
	{
	public:
		/// <summary>
		/// Constructs an empty script. Not valid.
		/// </summary>
		Script();

		/// <summary>
		/// Constructs a script. Provided chunk MUST be SCRP, LSCR, LSC2, ENCD, EXCD and VERB.
		/// </summary>
		/// <param name="a_Chunk">The script chunk.</param>
		/// <param name="a_mOPCodeMap">The script code map.</param>
		Script(parsing::Chunk& a_Chunk, const script::OPCodeMap& a_mOPCodeMap);

		/// <summary>
		/// Caches all the TALKies in the script arguments (when applicable).
		/// </summary>s
		/// <param name="a_mTalkies">The map of talkies of the other archive.</param>
		/// <returns>True if the script contained talk instructions, false otherwise.</returns>
		bool BindTALKies(std::unordered_map<std::size_t, parsing::Chunk*>& a_mTalkies);

		/// <summary>
		/// Retrieves an instruction at a given offset.
		/// </summary>
		/// <param name="a_iStartingPoint">The offset.</param>
		/// <returns>Instruction at the offset, nullptr if not found.</returns>
		ScriptInstruction* GetInstructionAtOffset(size_t a_iStartingPoint);

		/// <summary>
		/// Retrieves an instruction at a given index.
		/// </summary>
		/// <param name="a_iStartingPoint">The offset.</param>
		/// <returns>Instruction at index, nullptr if not found.</returns>
		ScriptInstruction* GetInstructionAtIndex(size_t a_iIndex);

		/// <summary>
		/// Updates the data to the latest working version.
		/// </summary>
		void Update();

		/// <summary>
		/// Retrieves the total size of the instructions.
		/// </summary>
		/// <returns>Returns the total size of the instructions.</returns>
		size_t GetInstructionsSize() const;

		/// <summary>
		/// Returns the number of instructions.
		/// </summary>
		/// <returns>Number of instructions.</returns>
		size_t NumInstructions() const;

		/// <summary>
		/// Retrieves all instructions.
		/// </summary>
		/// <returns>Get the instructions.</returns>
		const std::vector<std::unique_ptr<ScriptInstruction>>& GetInstructions() const;
	private:
		/// <summary>
		/// Retrieves the start point of the script data (depends on the chunk ID).
		/// </summary>
		/// <returns>The starting point in the data container where the first instruction is.</returns>
		size_t GetScriptStartingPoint() const;

		parsing::Chunk* m_pChunk = nullptr;
		std::vector<std::unique_ptr<ScriptInstruction>> m_aInstructions;
	};
}