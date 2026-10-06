#include "./OPCodesHENew.h"

#include <htb_lib/script/ScriptArgType.h>

namespace htb::script
{
	//======================================================================================
	// ArgInfo
	//======================================================================================
	ArgInfo::ArgInfo(size_t a_iSize, EScriptArgType a_eArgumentType) :
		m_iSize(a_iSize),
		m_eArgumentType(a_eArgumentType)
	{}

	//======================================================================================
	// Bytecode
	//======================================================================================
	Bytecode::~Bytecode()
	{}

	//======================================================================================
	Bytecode::Bytecode(const std::string& a_sName) :
		m_sName(a_sName)
	{}

	//======================================================================================
	Bytecode::Bytecode(const std::string& a_sName, std::function<std::vector<ArgInfo>(uint8_t, const unsigned char*)> a_fnSize) :
		m_sName(a_sName),
		m_fnSize(a_fnSize)
	{}

	//======================================================================================
	const std::string& Bytecode::GetName() const
	{
		return m_sName;
	}

	//======================================================================================
	// Public functions
	//======================================================================================
	std::function<std::vector<ArgInfo>(uint8_t, const unsigned char*)> Bytecode::GetSizeFn() const
	{
		return m_fnSize;
	}

	//======================================================================================
	std::vector<ArgInfo> default_func(uint8_t a_iByte, const unsigned char* a_pData)
	{
		return {};
	}

	//======================================================================================
	std::vector<ArgInfo> extended_b_op(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<ArgInfo> args;
		args.emplace_back(sizeof(uint8_t), EScriptArgType::BYTE);
		return args;
	}

	//======================================================================================
	std::vector<ArgInfo> extended_w_op(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<ArgInfo> args;
		args.emplace_back(sizeof(uint16_t), EScriptArgType::INT16);
		return args;
	}

	//======================================================================================
	std::vector<ArgInfo> extended_ww_op(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<ArgInfo> args;
		args.emplace_back(sizeof(uint16_t), EScriptArgType::INT16);
		args.emplace_back(sizeof(uint16_t), EScriptArgType::INT16);
		return args;
	}

	//======================================================================================
	std::vector<ArgInfo> extended_dw_op(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<ArgInfo> args;
		args.emplace_back(sizeof(uint32_t), EScriptArgType::INT32);
		return args;
	}

	//======================================================================================
	std::vector<ArgInfo> extended_ddw_op(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<ArgInfo> args;
		args.emplace_back(sizeof(uint32_t), EScriptArgType::INT32);
		args.emplace_back(sizeof(uint32_t), EScriptArgType::INT32);
		return args;
	}

	//======================================================================================
	std::vector<ArgInfo> extended_bw_op(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<ArgInfo> args;
		args.emplace_back(sizeof(uint8_t), EScriptArgType::BYTE);
		args.emplace_back(sizeof(uint16_t), EScriptArgType::INT16);
		return args;
	}

	//======================================================================================
	std::vector<ArgInfo> extended_bdw_op(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<ArgInfo> args;
		args.emplace_back(sizeof(uint8_t), EScriptArgType::BYTE);
		args.emplace_back(sizeof(uint32_t), EScriptArgType::INT32);
		return args;
	}

	//======================================================================================
	std::vector<ArgInfo> jump_cmd(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<ArgInfo> args;
		args.emplace_back(sizeof(int16_t), EScriptArgType::REF);
		return args;
	}

	//======================================================================================
	std::vector<ArgInfo> djump_cmd(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<ArgInfo> args;
		args.emplace_back(sizeof(int32_t), EScriptArgType::REF);
		return args;
	}

	//======================================================================================
	std::vector<ArgInfo> msg_cmd(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<ArgInfo> args;

		uint8_t cmd = a_pData[0];

		args.emplace_back(sizeof(cmd), EScriptArgType::BYTE);

		size_t pos = 1;

		if (cmd == 75 || cmd == 194)
		{
			while (a_pData[pos] != 0)
			{
				pos++;
			}

			pos++;

			args.emplace_back(pos - 1,
				EScriptArgType::STRING
			);
		}

		return args;
	}

	//======================================================================================
	std::vector<ArgInfo> msg_cmd_v8(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<ArgInfo> args;

		uint8_t cmd = a_pData[0];

		args.emplace_back(sizeof(cmd), EScriptArgType::BYTE);

		size_t pos = 1;

		if (cmd == 209)
		{
			uint32_t len =
				static_cast<uint32_t>(a_pData[pos]) |
				(static_cast<uint32_t>(a_pData[pos + 1]) << 8) |
				(static_cast<uint32_t>(a_pData[pos + 2]) << 16) |
				(static_cast<uint32_t>(a_pData[pos + 3]) << 24);

			args.emplace_back(sizeof(uint32_t),
				EScriptArgType::INT32
			);

			pos += 4;

			args.emplace_back(len,
				EScriptArgType::STRING
			);
		}

		return args;
	}

	//======================================================================================
	std::vector<ArgInfo> msg_cmd_he100(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<ArgInfo> args;

		uint8_t cmd = a_pData[0];

		args.emplace_back(sizeof(cmd), EScriptArgType::BYTE);

		size_t pos = 1;

		if (cmd == 35 || cmd == 79)
		{
			while (a_pData[pos] != 0)
			{
				pos++;
			}

			pos++;

			args.emplace_back(pos - 1,
				EScriptArgType::STRING
			);
		}

		return args;
	}

	//======================================================================================
	std::vector<ArgInfo> msg_op(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<ArgInfo> args;

		size_t pos = 0;

		while (a_pData[pos] != 0)
		{
			pos++;
		}

		pos++;

		args.emplace_back(pos,
			EScriptArgType::STRING
		);

		return args;
	}

	//======================================================================================
	std::vector<ArgInfo> msg_op_v8(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<ArgInfo> args;

		uint32_t len =
			static_cast<uint32_t>(a_pData[0]) |
			(static_cast<uint32_t>(a_pData[1]) << 8) |
			(static_cast<uint32_t>(a_pData[2]) << 16) |
			(static_cast<uint32_t>(a_pData[3]) << 24);

		args.emplace_back(sizeof(uint32_t),
			EScriptArgType::INT32
		);

		args.emplace_back(len,
			EScriptArgType::STRING
		);

		return args;
	}

	//======================================================================================
	std::vector<ArgInfo> actor_ops_v6(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<ArgInfo> args;

		uint8_t cmd = a_pData[0];

		args.emplace_back(sizeof(cmd), EScriptArgType::BYTE);

		size_t pos = 1;

		if (cmd == 0x58)
		{
			while (a_pData[pos] != 0)
			{
				pos++;
			}

			pos++;

			args.emplace_back(pos - 1,
				EScriptArgType::STRING
			);
		}

		return args;
	}

	//======================================================================================
	std::vector<ArgInfo> actor_ops_v8(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<ArgInfo> args;

		uint8_t cmd = a_pData[0];

		args.emplace_back(sizeof(cmd), EScriptArgType::BYTE);

		size_t pos = 1;

		if (cmd == 0x71)
		{
			uint32_t len =
				static_cast<uint32_t>(a_pData[pos]) |
				(static_cast<uint32_t>(a_pData[pos + 1]) << 8) |
				(static_cast<uint32_t>(a_pData[pos + 2]) << 16) |
				(static_cast<uint32_t>(a_pData[pos + 3]) << 24);

			args.emplace_back(sizeof(uint32_t),
				EScriptArgType::INT32
			);

			pos += 4;

			args.emplace_back(len,
				EScriptArgType::STRING
			);
		}

		return args;
	}

	//======================================================================================
	std::vector<ArgInfo> actor_ops_he60(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<ArgInfo> args;

		uint8_t cmd = a_pData[0];

		args.emplace_back(sizeof(cmd), EScriptArgType::BYTE);

		size_t pos = 1;

		if (cmd == 225)
		{
			while (a_pData[pos] != 0)
			{
				pos++;
			}

			pos++;

			args.emplace_back(pos - 1,
				EScriptArgType::STRING
			);
		}

		return args;
	}

	//======================================================================================
	std::vector<ArgInfo> verb_ops_v6(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<ArgInfo> args;

		uint8_t cmd = a_pData[0];

		args.emplace_back(sizeof(cmd), EScriptArgType::BYTE);

		size_t pos = 1;

		if (cmd == 0x7D)
		{
			while (a_pData[pos] != 0)
			{
				pos++;
			}

			pos++;

			args.emplace_back(pos - 1,
				EScriptArgType::STRING
			);
		}

		return args;
	}

	//======================================================================================
	std::vector<ArgInfo> verb_ops_v8(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<ArgInfo> args;

		uint8_t cmd = a_pData[0];

		args.emplace_back(sizeof(cmd), EScriptArgType::BYTE);

		size_t pos = 1;

		if (cmd == 0x99 || cmd == 0xA4)
		{
			uint32_t len =
				static_cast<uint32_t>(a_pData[pos]) |
				(static_cast<uint32_t>(a_pData[pos + 1]) << 8) |
				(static_cast<uint32_t>(a_pData[pos + 2]) << 16) |
				(static_cast<uint32_t>(a_pData[pos + 3]) << 24);

			args.emplace_back(sizeof(uint32_t),
				EScriptArgType::INT32
			);

			pos += 4;

			args.emplace_back(len,
				EScriptArgType::STRING
			);
		}

		return args;
	}

	//======================================================================================
	std::vector<ArgInfo> array_ops_v6(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<ArgInfo> args;

		uint8_t cmd = a_pData[0];

		args.emplace_back(sizeof(cmd), EScriptArgType::BYTE);

		args.emplace_back(sizeof(uint16_t),
			EScriptArgType::INT16
		);

		size_t pos = 1 + sizeof(uint16_t);

		if (cmd == 205)
		{
			while (a_pData[pos] != 0)
			{
				pos++;
			}

			pos++;

			args.emplace_back(pos - (1 + sizeof(uint16_t)),
				EScriptArgType::STRING
			);
		}

		return args;
	}

	//======================================================================================
	std::vector<ArgInfo> array_ops(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<ArgInfo> args;

		uint8_t cmd = a_pData[0];

		args.emplace_back(sizeof(cmd), EScriptArgType::BYTE);

		args.emplace_back(sizeof(uint16_t),
			EScriptArgType::INT16
		);

		size_t pos = 1 + sizeof(uint16_t);

		if (cmd == 127)
		{
			args.emplace_back(sizeof(uint16_t),
				EScriptArgType::INT16
			);
		}
		else if (cmd == 138)
		{
			args.emplace_back(sizeof(uint16_t),
				EScriptArgType::INT16
			);

			args.emplace_back(sizeof(uint16_t),
				EScriptArgType::INT16
			);
		}

		return args;
	}

	//======================================================================================
	std::vector<ArgInfo> array_ops_v8(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<ArgInfo> args;

		uint8_t cmd = a_pData[0];

		args.emplace_back(sizeof(cmd), EScriptArgType::BYTE);

		args.emplace_back(sizeof(uint32_t),
			EScriptArgType::INT32
		);

		size_t pos = 1 + sizeof(uint32_t);

		if (cmd == 0x14)
		{
			uint32_t len =
				static_cast<uint32_t>(a_pData[pos]) |
				(static_cast<uint32_t>(a_pData[pos + 1]) << 8) |
				(static_cast<uint32_t>(a_pData[pos + 2]) << 16) |
				(static_cast<uint32_t>(a_pData[pos + 3]) << 24);

			args.emplace_back(sizeof(uint32_t),
				EScriptArgType::INT32
			);

			pos += 4;

			args.emplace_back(len,
				EScriptArgType::STRING
			);
		}

		return args;
	}

	//======================================================================================
	std::vector<ArgInfo> array_ops_he100(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<ArgInfo> args;

		uint8_t cmd = a_pData[0];

		args.emplace_back(sizeof(cmd), EScriptArgType::BYTE);

		args.emplace_back(sizeof(uint16_t),
			EScriptArgType::INT16
		);

		size_t pos = 1 + sizeof(uint16_t);

		if (cmd == 131)
		{
			args.emplace_back(sizeof(uint16_t),
				EScriptArgType::INT16
			);
		}
		else if (cmd == 132)
		{
			args.emplace_back(sizeof(uint16_t),
				EScriptArgType::INT16
			);

			args.emplace_back(sizeof(uint16_t),
				EScriptArgType::INT16
			);
		}

		return args;
	}

	//======================================================================================
	std::vector<ArgInfo> wait_ops(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<ArgInfo> args;

		uint8_t cmd = a_pData[0];

		args.emplace_back(sizeof(cmd), EScriptArgType::BYTE);

		if (cmd == 168 || cmd == 226 || cmd == 232)
		{
			args.emplace_back(sizeof(int16_t),
				EScriptArgType::REF
			);
		}

		return args;
	}

	//======================================================================================
	std::vector<ArgInfo> wait_ops_v8(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<ArgInfo> args;

		uint8_t cmd = a_pData[0];

		args.emplace_back(sizeof(cmd), EScriptArgType::BYTE);

		if (cmd == 30 || cmd == 34 || cmd == 35)
		{
			args.emplace_back(sizeof(int32_t),
				EScriptArgType::REF
			);
		}

		return args;
	}

	//======================================================================================
	std::vector<ArgInfo> wait_ops_he100(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<ArgInfo> args;

		uint8_t cmd = a_pData[0];

		args.emplace_back(sizeof(cmd), EScriptArgType::BYTE);

		if (cmd == 128)
		{
			args.emplace_back(sizeof(int16_t),
				EScriptArgType::REF
			);
		}

		return args;
	}

	//======================================================================================
	std::vector<ArgInfo> room_ops_he60(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<ArgInfo> args;

		uint8_t cmd = a_pData[0];

		args.emplace_back(sizeof(cmd), EScriptArgType::BYTE);

		size_t pos = 1;

		if (cmd == 221)
		{
			while (a_pData[pos] != 0)
			{
				pos++;
			}

			pos++;

			args.emplace_back(pos - 1,
				EScriptArgType::STRING
			);
		}

		return args;
	}

	//======================================================================================
	std::vector<ArgInfo> dmsg_op(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<ArgInfo> args;

		size_t pos = 0;

		while (a_pData[pos] != 0)
		{
			pos++;
		}

		pos++;

		args.emplace_back(pos,
			EScriptArgType::STRING
		);

		size_t start = pos;

		while (a_pData[pos] != 0)
		{
			pos++;
		}

		pos++;

		args.emplace_back(pos - start,
			EScriptArgType::STRING
		);

		return args;
	}

	//======================================================================================
	std::vector<ArgInfo> sys_msg(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<ArgInfo> args;

		args.emplace_back(sizeof(uint8_t),
			EScriptArgType::BYTE
		);
		size_t pos = sizeof(uint8_t);

		while (a_pData[pos] != 0)
		{
			pos++;
		}

		pos++;

		args.emplace_back(pos - 1,
			EScriptArgType::STRING
		);

		return args;
	}

	//======================================================================================
	std::vector<ArgInfo> ini_op_v71(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<ArgInfo> args;

		uint8_t type = a_pData[-3];
		size_t pos = 0;

		if (type == 1)
		{
			while (a_pData[pos] != 0)
			{
				pos++;
			}

			pos++;

			args.emplace_back(pos,
				EScriptArgType::STRING
			);
		}
		else if (type == 2)
		{
			while (a_pData[pos] != 0)
			{
				pos++;
			}

			pos++;

			args.emplace_back(pos,
				EScriptArgType::STRING
			);

			size_t start = pos;

			while (a_pData[pos] != 0)
			{
				pos++;
			}

			pos++;

			args.emplace_back(pos - start,
				EScriptArgType::STRING
			);
		}

		return args;
	}

	//======================================================================================
	std::vector<ArgInfo> file_op(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<ArgInfo> args;

		uint8_t cmd = a_pData[0];

		args.emplace_back(sizeof(cmd), EScriptArgType::BYTE);

		if (cmd == 8)
		{
			args.emplace_back(sizeof(uint8_t),
				EScriptArgType::BYTE
			);
		}

		return args;
	}

	//======================================================================================
	std::vector<ArgInfo> file_op_he100(uint8_t a_iByte, const unsigned char* a_pData)
	{
		std::vector<ArgInfo> args;

		uint8_t cmd = a_pData[0];

		args.emplace_back(sizeof(cmd), EScriptArgType::BYTE);

		if (cmd == 5)
		{
			args.emplace_back(sizeof(uint8_t),
				EScriptArgType::BYTE
			);
		}

		return args;
	}
}