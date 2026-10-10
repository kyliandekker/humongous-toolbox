#include "./Script.h"

#include <cassert>

#include <htb_lib/building/ScriptBuilder.h>
#include <htb_lib/core/DataStream.h>
#include <htb_lib/core/Log.h>
#include <htb_lib/parsing/Chunk.h>
#include <htb_lib/parsing/ChunkIDs.h>
#include <htb_lib/script/ScriptArgType.h>

namespace htb::building
{
	//======================================================================================
	// ScriptArg
	//======================================================================================
	ScriptArg::ScriptArg(const core::Data& a_Data, script::EScriptArgType a_eArgumentType, ScriptInstruction* a_pOwnerInstruction) :
		m_Data(a_Data),
		m_eArgumentType(a_eArgumentType),
		m_pOwnerInstruction(a_pOwnerInstruction)
	{}

	//======================================================================================
	const std::string ScriptArg::GetString() const
	{
		return std::string(m_Data.dataAs<const char>(), m_Data.size());
	}

	//======================================================================================
	int32_t ScriptArg::GetRefJump() const
	{
		if (m_Data.size() == sizeof(int32_t))
		{
			const int32_t len =
				static_cast<int32_t>(m_Data[0]) |
				(static_cast<int32_t>(m_Data[1]) << 8) |
				(static_cast<int32_t>(m_Data[2]) << 16) |
				(static_cast<int32_t>(m_Data[3]) << 24);

			return static_cast<int32_t>(len);
		}
		else if (m_Data.size() == sizeof(int16_t))
		{
			const int16_t len =
				static_cast<int16_t>(m_Data[0]) |
				(static_cast<int16_t>(m_Data[1]) << 8);

			return static_cast<int32_t>(len);
		}

		return 0;
	}

	//======================================================================================
	void ScriptArg::SetData(const core::Data& a_Data)
	{
		m_Data = a_Data;
	}

	//======================================================================================
	void ScriptArg::SetByte(uint8_t a_iValue)
	{
		assert(m_Data.size() == sizeof(int8_t));
		assert(m_eArgumentType == script::EScriptArgType::BYTE);
		m_Data = core::Data(&a_iValue, sizeof(uint8_t));
	}

	//======================================================================================
	void ScriptArg::SetInt16(int16_t a_iValue)
	{
		assert(m_Data.size() == sizeof(int16_t));
		assert(m_eArgumentType == script::EScriptArgType::INT16 || m_eArgumentType == script::EScriptArgType::REF);
		m_Data = core::Data(&a_iValue, sizeof(int16_t));
	}

	//======================================================================================
	void ScriptArg::SetInt32(int32_t a_iValue)
	{
		assert(m_Data.size() == sizeof(int32_t));
		assert(m_eArgumentType == script::EScriptArgType::INT32 || m_eArgumentType == script::EScriptArgType::REF);
		m_Data = core::Data(&a_iValue, sizeof(int32_t));
	}

	//======================================================================================
	int32_t ScriptArg::SetString(const std::string& a_sString)
	{
		int32_t oldSize = m_Data.size();
		assert(m_eArgumentType == script::EScriptArgType::STRING);
		m_Data = core::Data(a_sString.c_str(), a_sString.size() + 1);
		return static_cast<int32_t>(oldSize) - static_cast<int32_t>(m_Data.size());
	}

	//======================================================================================
	void ScriptArg::SetRefJump(int32_t a_iValue)
	{
		if (m_Data.size() == sizeof(int32_t))
		{
			m_Data = core::Data(&a_iValue, sizeof(int32_t));
		}
		else if (m_Data.size() == sizeof(int16_t))
		{
			int16_t val = static_cast<int16_t>(a_iValue);
			m_Data = core::Data(&val, sizeof(int16_t));
		}
	}

	//======================================================================================
	size_t ScriptArg::GetOffsetFromInstruction() const
	{
		size_t offset = sizeof(m_pOwnerInstruction->GetCode());
		for (const ScriptArg& arg : m_pOwnerInstruction->GetArgs())
		{
			if (&arg == this)
			{
				break;
			}
			offset += arg.m_Data.size();
		}
		return offset;
	}

	//======================================================================================
	const core::Data& ScriptArg::GetData() const
	{
		return m_Data;
	}

	//======================================================================================
	script::EScriptArgType ScriptArg::GetArgumentType() const
	{
		return m_eArgumentType;
	}

	//======================================================================================
	ScriptInstruction* ScriptArg::GetOwnerInstruction() const
	{
		return m_pOwnerInstruction;
	}

	//======================================================================================
	parsing::Chunk* ScriptArg::GetTALKChunk() const
	{
		return m_pTALKChunk;
	}

	ScriptInstruction* ScriptArg::GetJumpTo() const
	{
		return m_pJumpTo;
	}

	//======================================================================================
	void ScriptArg::SetJumpTo(ScriptInstruction* a_pJumpTo)
	{
		m_pJumpTo = a_pJumpTo;
	}

	//======================================================================================
	void ScriptArg::SetTALKChunk(parsing::Chunk* a_pTALKChunk)
	{
		m_pTALKChunk = a_pTALKChunk;
	}

	//======================================================================================
	// ScriptInstruction
	//======================================================================================
	ScriptInstruction::ScriptInstruction(core::DataStream& a_Data, const script::OPCodeMap& a_mOPCodeMap)
	{
		a_Data.Read(&m_iCode, sizeof(m_iCode), 1);

		auto it = a_mOPCodeMap.find(m_iCode);

		assert(it != a_mOPCodeMap.end());
		if (it == a_mOPCodeMap.end())
		{
			core::Log(core::ELogLevel::_ERROR, "Could not bind scripts: Script encountered invalid byte code.");
			return;
		}

		const unsigned char* pureDat = a_Data.dataAs<unsigned char>() + a_Data.Tell();
		const std::vector<script::ArgInfo> args = it->second.GetSizeFn()(m_iCode, pureDat);

		for (const script::ArgInfo& argInfo : args)
		{
			core::Data argData(argInfo.m_iSize);
			a_Data.Read(argData.data(), argData.size(), 1);

			m_aArgs.emplace_back(argData, argInfo.m_eArgumentType, this);
		}
	}

	//======================================================================================
	size_t ScriptInstruction::GetDataSize() const
	{
		size_t size = 0;
		for (const ScriptArg& argument : m_aArgs)
		{
			size += argument.GetData().size();
		}
		return size;
	}

	//======================================================================================
	size_t ScriptInstruction::GetSize() const
	{
		return GetDataSize() + sizeof(m_iCode);
	}

	//======================================================================================
	size_t ScriptInstruction::GetOffsetFromFirstInstruction() const
	{
		size_t offset = 0;
		const ScriptInstruction* current = m_pPrevious;
		while (current != nullptr)
		{
			offset += current->GetSize();
			current = current->m_pPrevious;
		}
		return offset;
	}

	//======================================================================================
	void ScriptInstruction::ToData(core::DataStream& a_Data) const
	{
		a_Data.Write(&m_iCode, sizeof(m_iCode));
		for (const ScriptArg& argument : m_aArgs)
		{
			a_Data.Write(argument.GetData().data(), argument.GetData().size());
		}
	}

	//======================================================================================
	uint8_t ScriptInstruction::GetCode() const
	{
		return m_iCode;
	}

	//======================================================================================
	ScriptInstruction* ScriptInstruction::GetPrevious() const
	{
		return m_pPrevious;
	}

	//======================================================================================
	ScriptInstruction* ScriptInstruction::GetNext() const
	{
		return m_pNext;
	}

	//======================================================================================
	const std::vector<ScriptArg>& ScriptInstruction::GetArgs() const
	{
		return m_aArgs;
	}

	//======================================================================================
	std::vector<ScriptArg>& ScriptInstruction::GetArgs()
	{
		return m_aArgs;
	}

	//======================================================================================
	void ScriptInstruction::SetPreviousInstruction(ScriptInstruction* a_pPrevious)
	{
		m_pPrevious = a_pPrevious;
	}

	//======================================================================================
	void ScriptInstruction::SetNextInstruction(ScriptInstruction * a_pNext)
	{
		m_pNext = a_pNext;
	}

	//======================================================================================
	// Script
	//======================================================================================
	Script::Script() : GameResource()
	{}

	//======================================================================================
	Script::Script(parsing::Chunk& a_Chunk, const script::OPCodeMap& a_mOPCodeMap)
	{
		if (a_Chunk.GetTag() != parsing::SCRP_CHUNK_ID &&
			a_Chunk.GetTag() != parsing::ENCD_CHUNK_ID &&
			a_Chunk.GetTag() != parsing::EXCD_CHUNK_ID &&
			a_Chunk.GetTag() != parsing::LSCR_CHUNK_ID &&
			a_Chunk.GetTag() != parsing::LSC2_CHUNK_ID &&
			a_Chunk.GetTag() != parsing::VERB_CHUNK_ID)
		{
			return;
		}

		m_pChunk = &a_Chunk;

		const size_t startTell = GetScriptStartingPoint();
		const core::Data& data = m_pChunk->GetData();

		if (data.size() > 1)
		{
			core::DataStream scriptData = core::DataStream(data.dataAs<char>() + startTell, data.size() - startTell);

			for (size_t tell = 0; tell < scriptData.size();)
			{
				std::unique_ptr<ScriptInstruction> instruction = std::make_unique<ScriptInstruction>(scriptData, a_mOPCodeMap);

				if (m_aInstructions.size() > 0)
				{
					ScriptInstruction* prevInstruction = m_aInstructions[m_aInstructions.size() - 1].get();
					instruction->SetPreviousInstruction(prevInstruction);
					prevInstruction->SetNextInstruction(instruction.get());
				}

				size_t size = instruction->GetSize();
				m_aInstructions.push_back(std::move(instruction));

				// Go to next arg.
				tell = scriptData.Tell();
			}
		}

		std::unordered_map<std::size_t, ScriptInstruction*> instructionsTable;
		for (std::unique_ptr<ScriptInstruction>& instruction : m_aInstructions)
		{
			instructionsTable[instruction->GetOffsetFromFirstInstruction()] = instruction.get();
		}

		for (std::unique_ptr<ScriptInstruction>& instruction : m_aInstructions)
		{
			for (ScriptArg& arg : instruction->GetArgs())
			{
				if (arg.GetArgumentType() == script::EScriptArgType::REF)
				{
					const int32_t jumpSize = arg.GetRefJump();

					const int32_t endOfArgumentPos = arg.GetOffsetFromInstruction() + instruction->GetOffsetFromFirstInstruction() + arg.GetData().size();
					const int32_t jumpTo = endOfArgumentPos + jumpSize;

					auto instructionIt = instructionsTable.find(static_cast<size_t>(jumpTo));

					assert(instructionIt != instructionsTable.end());
					if (instructionIt == instructionsTable.end())
					{
						core::Log(core::ELogLevel::_ERROR, "Could not bind scripts: Script jumped to unknown bytecode.");
						return;
					}

					arg.SetJumpTo(instructionIt->second);
				}
			}
		}

		m_bValid = true;
	}

	//======================================================================================
	struct TalkRef
	{
		size_t pos;
		size_t size;
		size_t strSize;

		size_t offsetInStr;
	};

	//======================================================================================
	std::vector<TalkRef> GetTalkRefs(const htb::core::Data& a_Data)
	{
		std::vector<TalkRef> out;
		out.reserve(3);
		for (std::size_t i = 0; i + 1 < a_Data.size(); )
		{
			if (a_Data[i] == 0x7F && a_Data[i + 1] == 0x54)
			{
				const std::size_t a = i + 2;
				std::size_t b = a;
				while (b < a_Data.size() && a_Data[b] != 0x7F)
				{
					b += 1;
				}
				std::size_t comma = a;
				bool found = false;
				for (std::size_t k = a; k < b; k += 1)
				{
					if (a_Data[k] == ',')
					{
						comma = k;
						found = true;
					}
				}
				if (found == true)
				{
					const std::string posStr = std::string(a_Data.dataAs<const char>() + a, comma - a);
					const std::string sizeStr = std::string(a_Data.dataAs<const char>() + comma + 1, b - comma - 1);
					TalkRef ref;
					ref.pos = std::stoul(posStr);
					ref.size = std::stoul(sizeStr);
					ref.strSize = posStr.size() + sizeof(',') + sizeStr.size();
					ref.offsetInStr = a;
					out.push_back(ref);
				}
				i = b + 1;
			}
			else
			{
				i += 1;
			}
		}
		return out;
	}

	//======================================================================================
	bool Script::BindTALKies(std::unordered_map<std::size_t, parsing::Chunk*>& a_mTalkies)
	{
		if (!m_bValid)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not cache TALKies because the script was not valid.");
			return false;
		}

		bool hasTalkies = false;
		for (std::unique_ptr<ScriptInstruction>& instruction : m_aInstructions)
		{
			for (ScriptArg& arg : instruction->GetArgs())
			{
				if (arg.GetArgumentType() == script::EScriptArgType::STRING)
				{
					// So contrary to popular belief, talkActor is not the only code for calling TALKies.
					// These are known to call TALKs as of right now:
					//	* o72_getScriptString: By far the most calls.
					//	* o72_talkActor: Second-most calls.
					//	* o72_talkEgo: Third-most calls.
					//	* o6_printLine: Really only occasionally, but still a significant amount.
					//	* o6_printActor: Weirdly few calls.
					std::vector<TalkRef> talks = GetTalkRefs(arg.GetData());
					hasTalkies |= !talks.empty();

					assert(talks.size() == 1 || talks.empty());
					for (const TalkRef& talkRef : talks)
					{
						auto talkIt = a_mTalkies.find(talkRef.pos);

						assert(talkIt != a_mTalkies.end());
						if (talkIt == a_mTalkies.end())
						{
							core::Log(core::ELogLevel::_ERROR, "Could not bind TALKies: Script referenced invalid TALK chunk.");
							return false;
						}

						parsing::Chunk* referencedTALK = talkIt->second;

						assert(referencedTALK->WholeChunkSize() == talkRef.size);
						if (referencedTALK->WholeChunkSize() != talkRef.size)
						{
							core::Log(core::ELogLevel::_ERROR, "Could not bind TALKies: Referenced TALK chunk in script was not the same size.");
							return false;
						}

						arg.SetTALKChunk(referencedTALK);
					}
				}
			}
		}

		return hasTalkies;
	}

	//======================================================================================
	ScriptInstruction* Script::GetInstructionAtOffset(size_t a_iStartingPoint)
	{
		size_t tell = a_iStartingPoint;
		for (std::unique_ptr<ScriptInstruction>& instruction : m_aInstructions)
		{
			if (tell == a_iStartingPoint)
			{
				return instruction.get();
			}
			tell += instruction->GetSize();
		}
		return nullptr;
	}

	//======================================================================================
	ScriptInstruction* Script::GetInstructionAtIndex(size_t a_iIndex)
	{
		if (a_iIndex >= m_aInstructions.size())
		{
			return nullptr;
		}

		return m_aInstructions[a_iIndex].get();
	}

	//======================================================================================
	void Script::Update()
	{
		if (!m_bValid)
		{
			core::Log(core::ELogLevel::_ERROR, "Could not update script because the script was not valid.");
			return;
		}

		// Update the TALK instructions.
		for (std::unique_ptr<ScriptInstruction>& instruction : m_aInstructions)
		{
			for (ScriptArg& arg : instruction->GetArgs())
			{
				if (arg.GetArgumentType() == script::EScriptArgType::STRING)
				{
					std::vector<TalkRef> talks = GetTalkRefs(arg.GetData());
					for (auto it = talks.rbegin(); it != talks.rend(); ++it)
					{
						const TalkRef& talkRef = *it;

						parsing::Chunk* talkChunk = arg.GetTALKChunk();
						const std::string newTalkRef = std::to_string(talkChunk->GetOffsetFromRoot()) + "," + std::to_string(talkChunk->WholeChunkSize());

						size_t restOfData = arg.GetData().size() - (talkRef.offsetInStr + talkRef.strSize);
						core::DataStream newData(talkRef.offsetInStr + newTalkRef.size() + restOfData);
						newData.Write(arg.GetData().data(), talkRef.offsetInStr);
						newData.Write(newTalkRef.data(), newTalkRef.size());
						newData.Write(arg.GetData().dataAs<unsigned char>() + talkRef.offsetInStr + talkRef.strSize, restOfData);

						arg.SetData(newData);
					}
				}
			}
		}

		// Cache jump positions.
		std::unordered_map<std::size_t, ScriptInstruction*> instructionsTable;
		size_t tell = 0;
		for (std::unique_ptr<ScriptInstruction>& instruction : m_aInstructions)
		{
			instructionsTable[tell] = instruction.get();
			tell += instruction->GetSize();
		}

		// Update all jump refs.
		for (std::unique_ptr<ScriptInstruction>& instruction : m_aInstructions)
		{
			for (ScriptArg& arg : instruction->GetArgs())
			{
				if (arg.GetArgumentType() == script::EScriptArgType::REF)
				{
					const int32_t jumpSize = arg.GetRefJump();

					const int32_t endOfArgumentPos = arg.GetOffsetFromInstruction() + instruction->GetOffsetFromFirstInstruction() + arg.GetData().size();

					const int32_t jumpTo = endOfArgumentPos + jumpSize;
					const size_t actualOffset = arg.GetJumpTo()->GetOffsetFromFirstInstruction();
					if (jumpTo != actualOffset)
					{
						const int32_t newJumpSize = actualOffset - endOfArgumentPos;
						arg.SetRefJump(newJumpSize);
					}
				}
			}
		}

		// Write the data to the actual chunk.
		const size_t sizeInstructions = GetInstructionsSize();
		const size_t startTell = GetScriptStartingPoint();

		core::DataStream newData(sizeInstructions + startTell);
		if (startTell > 0)
		{
			newData.Write(m_pChunk->GetData().data(), startTell);
		}

		for (const std::unique_ptr<ScriptInstruction>& instruction : m_aInstructions)
		{
			core::DataStream instructionData(instruction->GetSize());
			instruction->ToData(instructionData);

			newData.Write(instructionData.data(), instructionData.size());
		}

		m_pChunk->SetData(newData);
	}

	//======================================================================================
	size_t Script::GetInstructionsSize() const
	{
		size_t size = 0;
		for (const std::unique_ptr<ScriptInstruction>& instruction : m_aInstructions)
		{
			size += instruction->GetSize();
		}
		return size;
	}

	//======================================================================================
	size_t Script::NumInstructions() const
	{
		return m_aInstructions.size();
	}

	//======================================================================================
	const std::vector<std::unique_ptr<ScriptInstruction>>& Script::GetInstructions() const
	{
		return m_aInstructions;
	}

	//======================================================================================
	size_t Script::GetScriptStartingPoint() const
	{
		const core::Data& data = m_pChunk->GetData();
		if (
			m_pChunk->GetTag() == parsing::SCRP_CHUNK_ID ||
			m_pChunk->GetTag() == parsing::ENCD_CHUNK_ID ||
			m_pChunk->GetTag() == parsing::EXCD_CHUNK_ID
		)
		{
			return 0;
		}
		else if (m_pChunk->GetTag() == parsing::LSCR_CHUNK_ID) // Skip ID (8bit unsigned int)
		{
			return sizeof(uint8_t);
		}
		else if (m_pChunk->GetTag() == parsing::LSC2_CHUNK_ID) // Skip ID (32bit unsigned int)
		{
			return sizeof(uint32_t);
		}
		else if (m_pChunk->GetTag() == parsing::VERB_CHUNK_ID)
		{
			size_t tell = 0;
			size_t dataSize = data.size();
			while (tell < dataSize)
			{
				const uint8_t key = data[tell];
				tell += 1; // key byte
				if (key == 0x00)
				{
					break;
				}
				tell += 2; // 2-byte offset
			}

			return tell;
		}

		assert(false);
		core::Log(core::ELogLevel::_ERROR, "Could not bind scripts: Could not recognize script type.");

		return 0;
	}
}