#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace htb::script
{
	enum class EScriptArgType;

	/// <summary>
	/// Information about the op command argument.
	/// </summary>
	struct ArgInfo
	{
		ArgInfo(size_t a_iSize, EScriptArgType a_eArgumentType);

		size_t m_iSize;
		EScriptArgType m_eArgumentType;
	};

	std::vector<ArgInfo> default_func(uint8_t a_iByte, const unsigned char* a_pData);

	/// <summary>
	/// Information about a byte code operation.
	/// </summary>
	class Bytecode
	{
	public:
		~Bytecode();
		Bytecode() = default;
		Bytecode(const std::string& a_sName);
		Bytecode(const std::string& a_sName, std::function<std::vector<ArgInfo>(uint8_t, const unsigned char*)> a_fnSize);

		const std::string& GetName() const;
		std::function<std::vector<ArgInfo>(uint8_t, const unsigned char*)> GetSizeFn() const;
	private:
		std::string m_sName;
		std::function<std::vector<ArgInfo>(uint8_t, const unsigned char*)> m_fnSize = default_func;
	};

	using OPCodeMap = std::unordered_map<uint8_t, Bytecode>;

	std::vector<ArgInfo> extended_b_op(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> extended_w_op(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> extended_ww_op(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> extended_dw_op(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> extended_ddw_op(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> extended_bw_op(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> extended_bdw_op(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> jump_cmd(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> djump_cmd(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> msg_cmd(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> msg_cmd_v8(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> msg_cmd_he100(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> msg_op(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> msg_op_v8(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> actor_ops_v6(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> actor_ops_v8(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> actor_ops_he60(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> verb_ops_v6(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> verb_ops_v8(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> array_ops_v6(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> array_ops(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> array_ops_v8(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> array_ops_he100(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> wait_ops(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> wait_ops_v8(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> wait_ops_he100(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> room_ops_he60(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> dmsg_op(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> sys_msg(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> ini_op_v71(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> file_op(uint8_t a_iByte, const unsigned char* a_pData);
	std::vector<ArgInfo> file_op_he100(uint8_t a_iByte, const unsigned char* a_pData);

	//======================================================================================
	inline void GetV6codes(OPCodeMap& a_mOPCodes)
	{
		a_mOPCodes[0x00] = Bytecode("o6_pushByte", extended_b_op );
		a_mOPCodes[0x01] = Bytecode("o6_pushWord", extended_w_op );
		a_mOPCodes[0x02] = Bytecode("o6_pushByteVar", extended_b_op );
		a_mOPCodes[0x03] = Bytecode("o6_pushWordVar", extended_w_op );
		// TODO: a_mOPCodes[0x06] = bytecode("o6_byteArrayRead"};
		a_mOPCodes[0x07] = Bytecode("o6_wordArrayRead", extended_w_op );
		a_mOPCodes[0x0A] = Bytecode("o6_byteArrayIndexedRead", extended_b_op );
		a_mOPCodes[0x0B] = Bytecode("o6_wordArrayIndexedRead", extended_w_op );
		a_mOPCodes[0x0C] = Bytecode("o6_dup");
		a_mOPCodes[0x0D] = Bytecode("o6_not");
		a_mOPCodes[0x0E] = Bytecode("o6_eq" );
		a_mOPCodes[0x0F] = Bytecode("o6_neq" );
		a_mOPCodes[0x10] = Bytecode("o6_gt" );
		a_mOPCodes[0x11] = Bytecode("o6_lt" );
		a_mOPCodes[0x12] = Bytecode("o6_le" );
		a_mOPCodes[0x13] = Bytecode("o6_ge" );
		a_mOPCodes[0x14] = Bytecode("o6_add" );
		a_mOPCodes[0x15] = Bytecode("o6_sub" );
		a_mOPCodes[0x16] = Bytecode("o6_mul" );
		a_mOPCodes[0x17] = Bytecode("o6_div" );
		a_mOPCodes[0x18] = Bytecode("o6_land" );  // logical and
		a_mOPCodes[0x19] = Bytecode("o6_lor" );  // logical or
		a_mOPCodes[0x1A] = Bytecode("o6_pop" );
		// TODO: a_mOPCodes[0x42] = bytecode("o6_writeByteVar"};
		a_mOPCodes[0x43] = Bytecode("o6_writeWordVar", extended_w_op );
		// TODO: a_mOPCodes[0x46] = bytecode("o6_byteArrayWrite"};
		a_mOPCodes[0x47] = Bytecode("o6_wordArrayWrite", extended_w_op );
		// TODO: a_mOPCodes[0x4a] = bytecode("o6_byteArrayIndexedWrite"};
		a_mOPCodes[0x4B] = Bytecode("o6_wordArrayIndexedWrite", extended_w_op );
		// TODO: a_mOPCodes[0x4e] = bytecode("o6_byteVarInc"};
		a_mOPCodes[0x4F] = Bytecode("o6_wordVarInc", extended_w_op );
		// TODO: a_mOPCodes[0x52] = bytecode("o6_byteArrayInc"};
		a_mOPCodes[0x53] = Bytecode("o6_wordArrayInc", extended_w_op );
		// TODO: a_mOPCodes[0x56] = bytecode("o6_byteVarDec"};
		a_mOPCodes[0x57] = Bytecode("o6_wordVarDec", extended_w_op );
		// TODO: a_mOPCodes[0x5a] = bytecode("o6_byteArrayDec"};
		a_mOPCodes[0x5B] = Bytecode("o6_wordArrayDec", extended_w_op );
		a_mOPCodes[0x5C] = Bytecode("o6_if", jump_cmd );  // jump if
		a_mOPCodes[0x5D] = Bytecode("o6_ifNot", jump_cmd );  // jump if not
		a_mOPCodes[0x5E] = Bytecode("o6_startScript" );
		a_mOPCodes[0x5F] = Bytecode("o6_startScriptQuick" );
		a_mOPCodes[0x60] = Bytecode("o6_startObject" );
		a_mOPCodes[0x61] = Bytecode("o6_drawObject" );
		a_mOPCodes[0x62] = Bytecode("o6_drawObjectAt" );
		a_mOPCodes[0x63] = Bytecode("o6_drawBlastObject" );
		a_mOPCodes[0x64] = Bytecode("o6_setBlastObjectWindow" );
		a_mOPCodes[0x65] = Bytecode("o6_stopObjectCodeObject" );  // o6_stopObjectCode
		a_mOPCodes[0x66] = Bytecode("o6_stopObjectCodeScript" );  // o6_stopObjectCode
		a_mOPCodes[0x67] = Bytecode("o6_endCutscene" );
		a_mOPCodes[0x68] = Bytecode("o6_cutscene" );
		// TODO: a_mOPCodes[0x69] = bytecode("o6_stopMusic"};
		a_mOPCodes[0x6A] = Bytecode("o6_freezeUnfreeze" );
		a_mOPCodes[0x6B] = Bytecode("o6_cursorCommand", extended_b_op );
		a_mOPCodes[0x6C] = Bytecode("o6_breakHere" );
		a_mOPCodes[0x6D] = Bytecode("o6_ifClassOfIs" );
		a_mOPCodes[0x6E] = Bytecode("o6_setClass" );
		a_mOPCodes[0x6F] = Bytecode("o6_getState" );
		a_mOPCodes[0x70] = Bytecode("o6_setState" );
		a_mOPCodes[0x71] = Bytecode("o6_setOwner" );
		a_mOPCodes[0x72] = Bytecode("o6_getOwner" );
		a_mOPCodes[0x73] = Bytecode("o6_jump", jump_cmd );
		a_mOPCodes[0x74] = Bytecode("o6_startSound" );
		a_mOPCodes[0x75] = Bytecode("o6_stopSound" );
		// TODO: a_mOPCodes[0x76] = bytecode("o6_startMusic"};
		a_mOPCodes[0x77] = Bytecode("o6_stopObjectScript" );
		a_mOPCodes[0x78] = Bytecode("o6_panCameraTo" );
		a_mOPCodes[0x79] = Bytecode("o6_actorFollowCamera" );
		a_mOPCodes[0x7A] = Bytecode("o6_setCameraAt" );
		a_mOPCodes[0x7B] = Bytecode("o6_loadRoom" );
		a_mOPCodes[0x7C] = Bytecode("o6_stopScript" );
		a_mOPCodes[0x7D] = Bytecode("o6_walkActorToObj" );
		a_mOPCodes[0x7E] = Bytecode("o6_walkActorTo" );
		a_mOPCodes[0x7F] = Bytecode("o6_putActorAtXY" );
		a_mOPCodes[0x80] = Bytecode("o6_putActorAtObject" );
		a_mOPCodes[0x81] = Bytecode("o6_faceActor" );
		a_mOPCodes[0x82] = Bytecode("o6_animateActor" );
		a_mOPCodes[0x83] = Bytecode("o6_doSentence" );
		a_mOPCodes[0x84] = Bytecode("o6_pickupObject" );
		a_mOPCodes[0x85] = Bytecode("o6_loadRoomWithEgo" );
		a_mOPCodes[0x87] = Bytecode("o6_getRandomNumber" );
		a_mOPCodes[0x88] = Bytecode("o6_getRandomNumberRange" );
		a_mOPCodes[0x8A] = Bytecode("o6_getActorMoving" );
		a_mOPCodes[0x8B] = Bytecode("o6_isScriptRunning" );
		a_mOPCodes[0x8C] = Bytecode("o6_getActorRoom" );
		a_mOPCodes[0x8D] = Bytecode("o6_getObjectX" );
		a_mOPCodes[0x8E] = Bytecode("o6_getObjectY" );
		a_mOPCodes[0x8F] = Bytecode("o6_getObjectOldDir" );
		a_mOPCodes[0x90] = Bytecode("o6_getActorWalkBox" );
		a_mOPCodes[0x91] = Bytecode("o6_getActorCostume" );
		a_mOPCodes[0x92] = Bytecode("o6_findInventory" );
		a_mOPCodes[0x93] = Bytecode("o6_getInventoryCount" );
		a_mOPCodes[0x94] = Bytecode("o6_getVerbFromXY" );
		a_mOPCodes[0x95] = Bytecode("o6_beginOverride" );
		a_mOPCodes[0x96] = Bytecode("o6_endOverride" );
		a_mOPCodes[0x97] = Bytecode("o6_setObjectName", msg_op );
		a_mOPCodes[0x98] = Bytecode("o6_isSoundRunning" );
		a_mOPCodes[0x99] = Bytecode("o6_setBoxFlags" );
		a_mOPCodes[0x9A] = Bytecode("o6_createBoxMatrix" );
		a_mOPCodes[0x9B] = Bytecode("o6_resourceRoutines", extended_b_op );
		a_mOPCodes[0x9C] = Bytecode("o6_roomOps", extended_b_op );
		a_mOPCodes[0x9D] = Bytecode("o6_actorOps", actor_ops_v6 );
		a_mOPCodes[0x9E] = Bytecode("o6_verbOps", verb_ops_v6 );
		a_mOPCodes[0x9F] = Bytecode("o6_getActorFromXY" );
		a_mOPCodes[0xA0] = Bytecode("o6_findObject" );
		a_mOPCodes[0xA1] = Bytecode("o6_pseudoRoom" );
		a_mOPCodes[0xA2] = Bytecode("o6_getActorElevation" );
		a_mOPCodes[0xA3] = Bytecode("o6_getVerbEntrypoint" );
		a_mOPCodes[0xA4] = Bytecode("o6_arrayOps", array_ops_v6 );
		a_mOPCodes[0xA5] = Bytecode("o6_saveRestoreVerbs", extended_b_op );
		a_mOPCodes[0xA6] = Bytecode("o6_drawBox" );
		a_mOPCodes[0xA7] = Bytecode("o6_pop" );
		a_mOPCodes[0xA8] = Bytecode("o6_getActorWidth" );
		a_mOPCodes[0xA9] = Bytecode("o6_wait", wait_ops );
		a_mOPCodes[0xAA] = Bytecode("o6_getActorScaleX" );
		a_mOPCodes[0xAB] = Bytecode("o6_getActorAnimCounter" );
		a_mOPCodes[0xAC] = Bytecode("o6_soundKludge" );
		a_mOPCodes[0xAD] = Bytecode("o6_isAnyOf" );
		a_mOPCodes[0xAE] = Bytecode("o6_systemOps", extended_b_op );
		a_mOPCodes[0xAF] = Bytecode("o6_isActorInBox" );
		a_mOPCodes[0xB0] = Bytecode("o6_delay" );
		a_mOPCodes[0xB1] = Bytecode("o6_delaySeconds" );
		a_mOPCodes[0xB2] = Bytecode("o6_delayMinutes" );
		a_mOPCodes[0xB3] = Bytecode("o6_stopSentence" );
		a_mOPCodes[0xB4] = Bytecode("o6_printLine", msg_cmd );
		a_mOPCodes[0xB5] = Bytecode("o6_printText", msg_cmd );
		a_mOPCodes[0xB6] = Bytecode("o6_printDebug", msg_cmd );
		a_mOPCodes[0xB7] = Bytecode("o6_printSystem", msg_cmd );
		a_mOPCodes[0xB8] = Bytecode("o6_printActor", msg_cmd );
		a_mOPCodes[0xB9] = Bytecode("o6_printEgo", msg_cmd );
		a_mOPCodes[0xBA] = Bytecode("o6_talkActor", msg_op );
		a_mOPCodes[0xBB] = Bytecode("o6_talkEgo", msg_op );
		a_mOPCodes[0xBC] = Bytecode("o6_dimArray", extended_bw_op );
		a_mOPCodes[0xBD] = Bytecode("o6_dummy" );
		a_mOPCodes[0xBE] = Bytecode("o6_startObjectQuick" );
		a_mOPCodes[0xBF] = Bytecode("o6_startScriptQuick2" );
		a_mOPCodes[0xC0] = Bytecode("o6_dim2dimArray", extended_bw_op );
		a_mOPCodes[0xC4] = Bytecode("o6_abs" );
		a_mOPCodes[0xC5] = Bytecode("o6_distObjectObject" );
		// TODO: a_mOPCodes[0xc6] = bytecode("o6_distObjectPt"};
		a_mOPCodes[0xC7] = Bytecode("o6_distPtPt" );
		a_mOPCodes[0xC8] = Bytecode("o6_kernelGetFunctions" );
		a_mOPCodes[0xC9] = Bytecode("o6_kernelSetFunctions" );
		a_mOPCodes[0xCA] = Bytecode("o6_delayFrames" );
		a_mOPCodes[0xCB] = Bytecode("o6_pickOneOf" );
		a_mOPCodes[0xCC] = Bytecode("o6_pickOneOfDefault" );
		a_mOPCodes[0xCD] = Bytecode("o6_stampObject" );
		a_mOPCodes[0xD0] = Bytecode("o6_getDateTime" );
		a_mOPCodes[0xD1] = Bytecode("o6_stopTalking" );
		a_mOPCodes[0xD2] = Bytecode("o6_getAnimateVariable" );
		a_mOPCodes[0xD4] = Bytecode("o6_shuffle", extended_w_op );
		a_mOPCodes[0xD5] = Bytecode("o6_jumpToScript" );
		a_mOPCodes[0xD6] = Bytecode("o6_band" );  // bitwise and
		a_mOPCodes[0xD7] = Bytecode("o6_bor" );  // bitwise or
		a_mOPCodes[0xD8] = Bytecode("o6_isRoomScriptRunning" );
		a_mOPCodes[0xDD] = Bytecode("o6_findAllObjects" );
		a_mOPCodes[0xE1] = Bytecode("o6_getPixel" );
		a_mOPCodes[0xE3] = Bytecode("o6_pickVarRandom", extended_w_op );
		a_mOPCodes[0xE4] = Bytecode("o6_setBoxSet", extended_b_op );
		a_mOPCodes[0xEC] = Bytecode("o6_getActorLayer" );
		a_mOPCodes[0xED] = Bytecode("o6_getObjectNewDir" );
	}

	//======================================================================================
	inline void GetHE60codes(OPCodeMap& a_mOPCodes)
	{
		GetV6codes(a_mOPCodes);

		a_mOPCodes[0x63] = Bytecode();
		a_mOPCodes[0x64] = Bytecode();
		a_mOPCodes[0x70] = Bytecode("o60_setState" );
		a_mOPCodes[0x9A] = Bytecode();
		a_mOPCodes[0x9C] = Bytecode("o60_roomOps", room_ops_he60 );
		a_mOPCodes[0x9D] = Bytecode("o60_actorOps", actor_ops_he60 );
		a_mOPCodes[0xAC] = Bytecode();
		a_mOPCodes[0xBD] = Bytecode("o6_stopObjectCodeReturn" );
		a_mOPCodes[0xC8] = Bytecode("o60_kernelGetFunctions" );
		a_mOPCodes[0xC9] = Bytecode("o60_kernelSetFunctions" );
		a_mOPCodes[0xD9] = Bytecode("o60_closeFile" );
		a_mOPCodes[0xDA] = Bytecode("o60_openFile", msg_op );
		a_mOPCodes[0xDB] = Bytecode("o60_readFile" );
		a_mOPCodes[0xDC] = Bytecode("o60_writeFile" );
		a_mOPCodes[0xDE] = Bytecode("o60_deleteFile", msg_op );
		a_mOPCodes[0xDF] = Bytecode("o60_rename", dmsg_op );
		a_mOPCodes[0xE0] = Bytecode("o60_soundOps", extended_b_op );
		a_mOPCodes[0xE2] = Bytecode("o60_localizeArrayToScript" );
		a_mOPCodes[0xE9] = Bytecode("o60_seekFilePos" );
		a_mOPCodes[0xEA] = Bytecode("o60_redimArray", extended_bw_op );
		a_mOPCodes[0xEB] = Bytecode("o60_readFilePos" );
		a_mOPCodes[0xEC] = Bytecode();
		a_mOPCodes[0xED] = Bytecode();
	}

	//======================================================================================
	inline void GetHE70codes(OPCodeMap& a_mOPCodes)
	{
		GetHE60codes(a_mOPCodes);

		a_mOPCodes[0x74] = Bytecode("o70_soundOps", extended_b_op );
		a_mOPCodes[0x84] = Bytecode("o70_pickupObject" );
		a_mOPCodes[0x8C] = Bytecode("o70_getActorRoom" );
		a_mOPCodes[0x9B] = Bytecode("o70_resourceRoutines", extended_b_op );
		a_mOPCodes[0xAE] = Bytecode("o70_systemOps", extended_b_op );
		a_mOPCodes[0xEE] = Bytecode("o70_getStringLen" );
		a_mOPCodes[0xF2] = Bytecode("o70_isResourceLoaded", extended_b_op );
		a_mOPCodes[0xF3] = Bytecode("o70_readINI", msg_op );
		a_mOPCodes[0xF4] = Bytecode("o70_writeINI", ini_op_v71 );
		a_mOPCodes[0xF9] = Bytecode("o70_createDirectory", msg_op );
		a_mOPCodes[0xFA] = Bytecode("o70_setSystemMessage", sys_msg );
	}

	//======================================================================================
	inline void GetHE71codes(OPCodeMap& a_mOPCodes)
	{
		GetHE70codes(a_mOPCodes);

		a_mOPCodes[0xC9] = Bytecode("o71_kernelSetFunctions" );
		a_mOPCodes[0xEC] = Bytecode("o71_copyString" );
		a_mOPCodes[0xED] = Bytecode("o71_getStringWidth" );
		a_mOPCodes[0xEF] = Bytecode("o71_appendString" );
		a_mOPCodes[0xF0] = Bytecode("o71_concatString" );
		a_mOPCodes[0xF1] = Bytecode("o71_compareString" );
		a_mOPCodes[0xF5] = Bytecode("o71_getStringLenForWidth" );
		a_mOPCodes[0xF6] = Bytecode("o71_getCharIndexInString" );
		a_mOPCodes[0xF7] = Bytecode("o71_findBox" );
		a_mOPCodes[0xFB] = Bytecode("o71_polygonOps", extended_b_op );
		a_mOPCodes[0xFC] = Bytecode("o71_polygonHit" );
	}

	//======================================================================================
	inline void GetHE72codes(OPCodeMap& a_mOPCodes)
	{
		GetHE71codes(a_mOPCodes);

		a_mOPCodes[0x02] = Bytecode("o72_pushDWord", extended_dw_op );
		a_mOPCodes[0x04] = Bytecode("o72_getScriptString", msg_op );
		a_mOPCodes[0x0A] = Bytecode();
		a_mOPCodes[0x1B] = Bytecode("o72_isAnyOf" );
		a_mOPCodes[0x42] = Bytecode();
		a_mOPCodes[0x46] = Bytecode();
		a_mOPCodes[0x4A] = Bytecode();
		a_mOPCodes[0x4E] = Bytecode();
		a_mOPCodes[0x50] = Bytecode("o72_resetCutscene" );
		a_mOPCodes[0x51] = Bytecode("o72_getHeap", extended_b_op );
		a_mOPCodes[0x52] = Bytecode("o72_findObjectWithClassOf" );
		a_mOPCodes[0x54] = Bytecode("o72_getObjectImageX" );
		a_mOPCodes[0x55] = Bytecode("o72_getObjectImageY" );
		a_mOPCodes[0x56] = Bytecode("o72_captureWizImage" );
		a_mOPCodes[0x58] = Bytecode("o72_getTimer", extended_b_op );
		a_mOPCodes[0x59] = Bytecode("o72_setTimer", extended_b_op );
		a_mOPCodes[0x5A] = Bytecode("o72_getSoundPosition" );
		a_mOPCodes[0x5E] = Bytecode("o72_startScript", extended_b_op );
		a_mOPCodes[0x60] = Bytecode("o72_startObject", extended_b_op );
		a_mOPCodes[0x61] = Bytecode("o72_drawObject", extended_b_op );
		a_mOPCodes[0x62] = Bytecode("o72_printWizImage" );
		a_mOPCodes[0x63] = Bytecode("o72_getArrayDimSize", extended_bw_op );
		a_mOPCodes[0x64] = Bytecode("o72_getNumFreeArrays" );
		a_mOPCodes[0x97] = Bytecode();
		a_mOPCodes[0x9C] = Bytecode("o72_roomOps", extended_b_op );
		a_mOPCodes[0x9D] = Bytecode("o72_actorOps", extended_b_op );
		a_mOPCodes[0x9E] = Bytecode("o72_verbOps", extended_b_op );
		// TODO: a_mOPCodes[0xa0] = bytecode("o72_findObject" );
		a_mOPCodes[0xA4] = Bytecode("o72_arrayOps", array_ops );
		a_mOPCodes[0xAE] = Bytecode("o72_systemOps", extended_b_op );
		a_mOPCodes[0xBA] = Bytecode("o72_talkActor", msg_op );
		a_mOPCodes[0xBB] = Bytecode("o72_talkEgo", msg_op );
		a_mOPCodes[0xBC] = Bytecode("o72_dimArray", extended_bw_op );
		a_mOPCodes[0xC0] = Bytecode("o72_dim2dimArray", extended_bw_op );
		a_mOPCodes[0xC1] = Bytecode("o72_traceStatus" );
		a_mOPCodes[0xC8] = Bytecode("o72_kernelGetFunctions" );
		a_mOPCodes[0xCE] = Bytecode("o72_drawWizImage" );
		a_mOPCodes[0xCF] = Bytecode("o72_debugInput" );
		a_mOPCodes[0xD5] = Bytecode("o72_jumpToScript", extended_b_op );
		a_mOPCodes[0xDA] = Bytecode("o72_openFile" );
		a_mOPCodes[0xDB] = Bytecode("o72_readFile", file_op );
		a_mOPCodes[0xDC] = Bytecode("o72_writeFile", file_op );
		a_mOPCodes[0xDD] = Bytecode("o72_findAllObjects" );
		a_mOPCodes[0xDE] = Bytecode("o72_deleteFile" );
		a_mOPCodes[0xDF] = Bytecode("o72_rename" );
		a_mOPCodes[0xE1] = Bytecode("o72_getPixel", extended_b_op );
		// TODO: a_mOPCodes[0xe3] = bytecode("o72_pickVarRandom" );
		a_mOPCodes[0xEA] = Bytecode("o72_redimArray", extended_bw_op );
		a_mOPCodes[0xF3] = Bytecode("o72_readINI", extended_b_op );
		a_mOPCodes[0xF4] = Bytecode("o72_writeINI", extended_b_op );
		a_mOPCodes[0xF8] = Bytecode("o72_getResourceSize" );
		a_mOPCodes[0xF9] = Bytecode("o72_createDirectory" );
		a_mOPCodes[0xFA] = Bytecode("o72_setSystemMessage", extended_b_op );
	}

	//======================================================================================
	inline void GetHE73codes(OPCodeMap& a_mOPCodes)
	{
		GetHE72codes(a_mOPCodes);

		a_mOPCodes[0xF8] = Bytecode("o73_getResourceSize", extended_b_op );
	}

	//======================================================================================
	inline void GetHE80codes(OPCodeMap& a_mOPCodes)
	{
		GetHE73codes(a_mOPCodes);

		a_mOPCodes[0x45] = Bytecode("o80_createSound", extended_b_op );
		a_mOPCodes[0x46] = Bytecode("o80_getFileSize" );
		a_mOPCodes[0x48] = Bytecode("o80_stringToInt" );
		a_mOPCodes[0x49] = Bytecode("o80_getSoundVar" );
		a_mOPCodes[0x4A] = Bytecode("o80_localizeArrayToRoom" );
		// TODO: a_mOPCodes[0x4C] = bytecode("o80_sourceDebug" );
		a_mOPCodes[0x4D] = Bytecode("o80_readConfigFile", extended_b_op );
		a_mOPCodes[0x4E] = Bytecode("o80_writeConfigFile", extended_b_op );
		a_mOPCodes[0x69] = Bytecode();
		a_mOPCodes[0x6B] = Bytecode("o80_cursorCommand", extended_b_op );
		a_mOPCodes[0x70] = Bytecode("o80_setState" );
		a_mOPCodes[0x76] = Bytecode();
		a_mOPCodes[0x94] = Bytecode();
		a_mOPCodes[0x9E] = Bytecode();
		a_mOPCodes[0xA5] = Bytecode();
		a_mOPCodes[0xAC] = Bytecode("o80_drawWizPolygon" );
		a_mOPCodes[0xE0] = Bytecode("o80_drawLine", extended_b_op );
		a_mOPCodes[0xE3] = Bytecode("o80_pickVarRandom", extended_w_op );
	}

	//======================================================================================
	inline void GetHE90codes(OPCodeMap& a_mOPCodes)
	{
		GetHE80codes(a_mOPCodes);

		a_mOPCodes[0x0A] = Bytecode("o90_dup_n", extended_w_op );
		a_mOPCodes[0x1C] = Bytecode("o90_wizImageOps", extended_b_op );
		a_mOPCodes[0x1D] = Bytecode("o90_min" );
		a_mOPCodes[0x1E] = Bytecode("o90_max" );
		a_mOPCodes[0x1F] = Bytecode("o90_sin" );
		a_mOPCodes[0x20] = Bytecode("o90_cos" );
		a_mOPCodes[0x21] = Bytecode("o90_sqrt" );
		a_mOPCodes[0x22] = Bytecode("o90_atan2" );
		a_mOPCodes[0x23] = Bytecode("o90_getSegmentAngle" );
		a_mOPCodes[0x24] = Bytecode("o90_getDistanceBetweenPoints", extended_b_op );
		a_mOPCodes[0x25] = Bytecode("o90_getSpriteInfo", extended_b_op );
		a_mOPCodes[0x26] = Bytecode("o90_setSpriteInfo", extended_b_op );
		a_mOPCodes[0x27] = Bytecode("o90_getSpriteGroupInfo", extended_b_op );
		a_mOPCodes[0x28] = Bytecode("o90_setSpriteGroupInfo", extended_b_op );
		a_mOPCodes[0x29] = Bytecode("o90_getWizData", extended_b_op );
		a_mOPCodes[0x2A] = Bytecode("o90_getActorData" );
		a_mOPCodes[0x2B] = Bytecode("o90_startScriptUnk", extended_b_op );
		a_mOPCodes[0x2C] = Bytecode("o90_jumpToScriptUnk", extended_b_op );
		a_mOPCodes[0x2D] = Bytecode("o90_videoOps", extended_b_op );
		a_mOPCodes[0x2E] = Bytecode("o90_getVideoData", extended_b_op );
		a_mOPCodes[0x2F] = Bytecode("o90_floodFill", extended_b_op );
		a_mOPCodes[0x30] = Bytecode("o90_mod" );
		a_mOPCodes[0x31] = Bytecode("o90_shl" );
		a_mOPCodes[0x32] = Bytecode("o90_shr" );
		a_mOPCodes[0x33] = Bytecode("o90_xor" );
		a_mOPCodes[0x34] = Bytecode("o90_findAllObjectsWithClassOf" );
		a_mOPCodes[0x35] = Bytecode("o90_getPolygonOverlap" );
		a_mOPCodes[0x36] = Bytecode("o90_cond" );
		a_mOPCodes[0x37] = Bytecode("o90_dim2dim2Array", extended_bw_op );
		a_mOPCodes[0x38] = Bytecode("o90_redim2dimArray", extended_bw_op );
		a_mOPCodes[0x39] = Bytecode("o90_getLinesIntersectionPoint", extended_ww_op );
		a_mOPCodes[0x3A] = Bytecode("o90_sortArray", extended_bw_op );
		a_mOPCodes[0x44] = Bytecode("o90_getObjectData", extended_b_op );
		a_mOPCodes[0x69] = Bytecode("o90_disabled_windowOps", extended_b_op );
		a_mOPCodes[0x94] = Bytecode("o90_getPaletteData", extended_b_op );
		a_mOPCodes[0x9E] = Bytecode("o90_paletteOps", extended_b_op );
		a_mOPCodes[0xA5] = Bytecode("o90_fontEnum", extended_b_op );
		// TODO: a_mOPCodes[0xab] = bytecode("o90_getActorAnimProgress" );
		// TODO: a_mOPCodes[0xc8] = bytecode("o90_kernelGetFunctions" );
		// TODO: a_mOPCodes[0xc9] = bytecode("o90_kernelSetFunctions" );
	}

	//======================================================================================
	inline void GetHE100codes(OPCodeMap& a_mOPCodes)
	{
		a_mOPCodes[0x00] = Bytecode("o100_actorOps", extended_b_op );
		a_mOPCodes[0x01] = Bytecode("o6_add" );
		a_mOPCodes[0x02] = Bytecode("o6_faceActor" );
		a_mOPCodes[0x03] = Bytecode("o90_sortArray", extended_bw_op );
		a_mOPCodes[0x04] = Bytecode("o100_arrayOps", array_ops_he100 );
		a_mOPCodes[0x05] = Bytecode("o6_band" );
		a_mOPCodes[0x06] = Bytecode("o6_bor" );
		a_mOPCodes[0x07] = Bytecode("o6_breakHere" );
		a_mOPCodes[0x08] = Bytecode("o6_delayFrames" );
		a_mOPCodes[0x09] = Bytecode("o90_shl" );
		a_mOPCodes[0x0A] = Bytecode("o90_shr" );
		a_mOPCodes[0x0B] = Bytecode("o90_xor" );
		a_mOPCodes[0x0C] = Bytecode("o6_setCameraAt" );
		a_mOPCodes[0x0D] = Bytecode("o6_actorFollowCamera" );
		a_mOPCodes[0x0E] = Bytecode("o6_loadRoom" );
		// TODO: a_mOPCodes[0x0f] = bytecode("o6_panCameraTo" );
		// TODO: a_mOPCodes[0x10] = bytecode("o72_captureWizImage" );
		a_mOPCodes[0x11] = Bytecode("o100_jumpToScript", extended_b_op );
		a_mOPCodes[0x12] = Bytecode("o6_setClass" );
		a_mOPCodes[0x13] = Bytecode("o60_closeFile" );
		// TODO: a_mOPCodes[0x14] = bytecode("o6_loadRoomWithEgo" );
		a_mOPCodes[0x16] = Bytecode("o72_createDirectory" );
		a_mOPCodes[0x17] = Bytecode("o100_createSound", extended_b_op );
		// TODO: a_mOPCodes[0x18] = bytecode("o6_cutscene" );
		a_mOPCodes[0x19] = Bytecode("o6_pop" );
		a_mOPCodes[0x1A] = Bytecode("o72_traceStatus" );
		a_mOPCodes[0x1B] = Bytecode("o6_wordVarDec", extended_w_op );
		a_mOPCodes[0x1C] = Bytecode("o6_wordArrayDec", extended_w_op );
		a_mOPCodes[0x1D] = Bytecode("o72_deleteFile" );
		a_mOPCodes[0x1E] = Bytecode("o100_dim2dimArray", extended_bw_op );
		a_mOPCodes[0x1F] = Bytecode("o100_dimArray", extended_bw_op );
		a_mOPCodes[0x20] = Bytecode("o6_div" );
		a_mOPCodes[0x21] = Bytecode("o6_animateActor" );
		// TODO: a_mOPCodes[0x22] = bytecode("o6_doSentence" );
		a_mOPCodes[0x23] = Bytecode("o6_drawBox" );
		// TODO: a_mOPCodes[0x24] = bytecode("o72_drawWizImage" );
		// TODO: a_mOPCodes[0x25] = bytecode("o80_drawWizPolygon" );
		a_mOPCodes[0x26] = Bytecode("o100_drawLine", extended_b_op );
		a_mOPCodes[0x27] = Bytecode("o100_drawObject", extended_b_op );
		a_mOPCodes[0x28] = Bytecode("o6_dup" );
		a_mOPCodes[0x29] = Bytecode("o90_dup_n", extended_w_op );
		// TODO: a_mOPCodes[0x2a] = bytecode("o6_endCutscene" );
		a_mOPCodes[0x2B] = Bytecode("o6_stopObjectCodeObject" );  // o6_stopObjectCode
		a_mOPCodes[0x2C] = Bytecode("o6_stopObjectCodeScript" );  // o6_stopObjectCode
		a_mOPCodes[0x2D] = Bytecode("o6_eq" );
		// TODO: a_mOPCodes[0x2e] = bytecode("o100_floodFill" );
		// TODO: a_mOPCodes[0x2f] = bytecode("o6_freezeUnfreeze" );
		a_mOPCodes[0x30] = Bytecode("o6_ge" );
		a_mOPCodes[0x31] = Bytecode("o6_getDateTime" );
		a_mOPCodes[0x32] = Bytecode("o100_setSpriteGroupInfo", extended_b_op );
		a_mOPCodes[0x33] = Bytecode("o6_gt" );
		a_mOPCodes[0x34] = Bytecode("o100_resourceRoutines", extended_b_op );
		a_mOPCodes[0x35] = Bytecode("o6_if", jump_cmd );
		a_mOPCodes[0x36] = Bytecode("o6_ifNot", jump_cmd );
		a_mOPCodes[0x37] = Bytecode("o100_wizImageOps", extended_b_op );
		a_mOPCodes[0x38] = Bytecode("o72_isAnyOf" );
		a_mOPCodes[0x39] = Bytecode("o6_wordVarInc", extended_w_op );
		a_mOPCodes[0x3A] = Bytecode("o6_wordArrayInc", extended_w_op );
		a_mOPCodes[0x3B] = Bytecode("o6_jump", jump_cmd );
		a_mOPCodes[0x3C] = Bytecode("o90_kernelSetFunctions" );
		a_mOPCodes[0x3D] = Bytecode("o6_land" );
		a_mOPCodes[0x3E] = Bytecode("o6_le" );
		a_mOPCodes[0x3F] = Bytecode("o60_localizeArrayToScript" );
		a_mOPCodes[0x40] = Bytecode("o6_wordArrayRead", extended_w_op );
		a_mOPCodes[0x41] = Bytecode("o6_wordArrayIndexedRead", extended_w_op );
		a_mOPCodes[0x42] = Bytecode("o6_lor" );
		a_mOPCodes[0x43] = Bytecode("o6_lt" );
		a_mOPCodes[0x44] = Bytecode("o90_mod" );
		a_mOPCodes[0x45] = Bytecode("o6_mul" );
		a_mOPCodes[0x46] = Bytecode("o6_neq" );
		a_mOPCodes[0x47] = Bytecode("o100_dim2dim2Array", extended_bw_op );
		a_mOPCodes[0x49] = Bytecode("o100_redim2dimArray", extended_bw_op );
		a_mOPCodes[0x4A] = Bytecode("o6_not" );
		a_mOPCodes[0x4C] = Bytecode("o6_beginOverride" );
		a_mOPCodes[0x4D] = Bytecode("o6_endOverride" );
		a_mOPCodes[0x4E] = Bytecode("o72_resetCutscene" );
		a_mOPCodes[0x4F] = Bytecode("o6_setOwner" );
		a_mOPCodes[0x50] = Bytecode("o100_paletteOps", extended_b_op );
		a_mOPCodes[0x51] = Bytecode("o70_pickupObject" );
		a_mOPCodes[0x52] = Bytecode("o100_polygonOps", extended_b_op );  // o71_polygonOps
		a_mOPCodes[0x53] = Bytecode("o6_pop" );
		a_mOPCodes[0x54] = Bytecode("o100_printDebug", msg_cmd_he100 );  // o6_printDebug
		a_mOPCodes[0x55] = Bytecode("o72_printWizImage" );
		a_mOPCodes[0x56] = Bytecode("o100_printLine", msg_cmd_he100 );  // o6_printLine
		a_mOPCodes[0x57] = Bytecode("o100_printSystem", msg_cmd_he100 );  // o6_printSystem
		a_mOPCodes[0x58] = Bytecode("o100_printText", msg_cmd_he100 );  // o6_printText
		// TODO: a_mOPCodes[0x59] = bytecode("o100_jumpToScriptUnk" );
		a_mOPCodes[0x5A] = Bytecode("o100_startScriptUnk", extended_b_op );
		// TODO: a_mOPCodes[0x5b] = bytecode("o6_pseudoRoom" );
		a_mOPCodes[0x5C] = Bytecode("o6_pushByte", extended_b_op );
		a_mOPCodes[0x5D] = Bytecode("o72_pushDWord", extended_dw_op );
		a_mOPCodes[0x5E] = Bytecode("o72_getScriptString", msg_op );
		a_mOPCodes[0x5F] = Bytecode("o6_pushWord", extended_w_op );
		a_mOPCodes[0x60] = Bytecode("o6_pushWordVar", extended_w_op );
		a_mOPCodes[0x61] = Bytecode("o6_putActorAtObject" );
		a_mOPCodes[0x62] = Bytecode("o6_putActorAtXY" );
		a_mOPCodes[0x64] = Bytecode("o100_redimArray", extended_bw_op );
		a_mOPCodes[0x65] = Bytecode("o72_rename" );
		a_mOPCodes[0x66] = Bytecode("o6_stopObjectCodeReturn" );  // o6_stopObjectCode
		// TODO: a_mOPCodes[0x67] = bytecode("o80_localizeArrayToRoom" );
		a_mOPCodes[0x68] = Bytecode("o100_roomOps", extended_b_op );
		a_mOPCodes[0x69] = Bytecode("o100_printActor", msg_cmd_he100 );  // o6_printActor
		a_mOPCodes[0x6A] = Bytecode("o100_printEgo", msg_cmd_he100 );  // o6_printEgo
		a_mOPCodes[0x6B] = Bytecode("o72_talkActor", msg_op );
		a_mOPCodes[0x6C] = Bytecode("o72_talkEgo", msg_op );
		a_mOPCodes[0x6E] = Bytecode("o60_seekFilePos" );
		a_mOPCodes[0x6F] = Bytecode("o6_setBoxFlags" );
		// TODO: a_mOPCodes[0x71] = bytecode("o6_setBoxSet" );
		a_mOPCodes[0x72] = Bytecode("o100_setSystemMessage", extended_b_op );
		a_mOPCodes[0x73] = Bytecode("o6_shuffle", extended_w_op );
		a_mOPCodes[0x74] = Bytecode("o6_delay" );
		// TODO: a_mOPCodes[0x75] = bytecode("o6_delayMinutes" );
		a_mOPCodes[0x76] = Bytecode("o6_delaySeconds" );
		a_mOPCodes[0x77] = Bytecode("o100_soundOps", extended_b_op );
		a_mOPCodes[0x78] = Bytecode("o80_sourceDebug", extended_ddw_op );
		a_mOPCodes[0x79] = Bytecode("o100_setSpriteInfo", extended_b_op );
		a_mOPCodes[0x7A] = Bytecode("o6_stampObject" );
		a_mOPCodes[0x7B] = Bytecode("o72_startObject", extended_b_op );
		a_mOPCodes[0x7C] = Bytecode("o100_startScript", extended_b_op );
		// TODO: a_mOPCodes[0x7d] = bytecode("o6_startScriptQuick" );
		a_mOPCodes[0x7E] = Bytecode("o80_setState" );
		a_mOPCodes[0x7F] = Bytecode("o6_stopObjectScript" );
		a_mOPCodes[0x80] = Bytecode("o6_stopScript" );
		a_mOPCodes[0x81] = Bytecode("o6_stopSentence" );
		a_mOPCodes[0x82] = Bytecode("o6_stopSound" );
		a_mOPCodes[0x83] = Bytecode("o6_stopTalking" );
		a_mOPCodes[0x84] = Bytecode("o6_writeWordVar", extended_w_op );
		a_mOPCodes[0x85] = Bytecode("o6_wordArrayWrite", extended_w_op );
		a_mOPCodes[0x86] = Bytecode("o6_wordArrayIndexedWrite", extended_w_op );
		a_mOPCodes[0x87] = Bytecode("o6_sub" );
		a_mOPCodes[0x88] = Bytecode("o100_systemOps", extended_b_op );
		a_mOPCodes[0x8A] = Bytecode("o72_setTimer", extended_b_op );
		a_mOPCodes[0x8B] = Bytecode("o100_cursorCommand", extended_b_op );
		a_mOPCodes[0x8C] = Bytecode("o100_videoOps", extended_b_op );
		a_mOPCodes[0x8D] = Bytecode("o100_wait", wait_ops_he100 );
		// TODO: a_mOPCodes[0x8e] = bytecode("o6_walkActorToObj" );
		a_mOPCodes[0x8F] = Bytecode("o6_walkActorTo" );
		a_mOPCodes[0x89] = Bytecode("o100_disabled_windowOps", extended_b_op );
		a_mOPCodes[0x90] = Bytecode("o100_writeFile", file_op_he100 );
		a_mOPCodes[0x91] = Bytecode("o72_writeINI", extended_b_op );
		a_mOPCodes[0x92] = Bytecode("o80_writeConfigFile", extended_b_op );
		a_mOPCodes[0x93] = Bytecode("o6_abs" );
		// TODO: a_mOPCodes[0x94] = bytecode("o6_getActorWalkBox" );
		a_mOPCodes[0x95] = Bytecode("o6_getActorCostume" );
		a_mOPCodes[0x96] = Bytecode("o6_getActorElevation" );
		a_mOPCodes[0x97] = Bytecode("o6_getObjectOldDir" );
		a_mOPCodes[0x98] = Bytecode("o6_getActorMoving" );
		a_mOPCodes[0x99] = Bytecode("o90_getActorData" );
		a_mOPCodes[0x9A] = Bytecode("o6_getActorRoom" );
		a_mOPCodes[0x9B] = Bytecode("o6_getActorScaleX" );
		a_mOPCodes[0x9C] = Bytecode("o6_getAnimateVariable" );
		// TODO: a_mOPCodes[0x9d] = bytecode("o6_getActorWidth" );
		a_mOPCodes[0x9E] = Bytecode("o6_getObjectX" );
		a_mOPCodes[0x9F] = Bytecode("o6_getObjectY" );
		a_mOPCodes[0xA0] = Bytecode("o90_atan2" );
		a_mOPCodes[0xA1] = Bytecode("o90_getSegmentAngle" );
		// TODO: a_mOPCodes[0xa2] = bytecode("o90_getActorAnimProgress" );
		a_mOPCodes[0xA3] = Bytecode("o90_getDistanceBetweenPoints", extended_b_op );
		a_mOPCodes[0xA4] = Bytecode("o6_ifClassOfIs" );
		a_mOPCodes[0xA6] = Bytecode("o90_cond" );
		a_mOPCodes[0xA7] = Bytecode("o90_cos" );
		a_mOPCodes[0xA8] = Bytecode("o100_debugInput", extended_b_op );
		a_mOPCodes[0xA9] = Bytecode("o80_getFileSize" );
		a_mOPCodes[0xAA] = Bytecode("o6_getActorFromXY" );
		a_mOPCodes[0xAB] = Bytecode("o72_findAllObjects" );
		a_mOPCodes[0xAC] = Bytecode("o90_findAllObjectsWithClassOf" );
		// TODO: a_mOPCodes[0xad] = bytecode("o71_findBox" );
		// TODO: a_mOPCodes[0xae] = bytecode("o6_findInventory" );
		a_mOPCodes[0xAF] = Bytecode("o72_findObject" );
		// TODO: a_mOPCodes[0xb0] = bytecode("o72_findObjectWithClassOf" );
		a_mOPCodes[0xB1] = Bytecode("o71_polygonHit" );
		// TODO: a_mOPCodes[0xb2] = bytecode("o90_getLinesIntersectionPoint" );
		a_mOPCodes[0xB3] = Bytecode("o90_fontEnum", extended_b_op );
		a_mOPCodes[0xB4] = Bytecode("o72_getNumFreeArrays" );
		a_mOPCodes[0xB5] = Bytecode("o72_getArrayDimSize", extended_bw_op );
		a_mOPCodes[0xB6] = Bytecode("o100_isResourceLoaded", extended_b_op );
		a_mOPCodes[0xB7] = Bytecode("o100_getResourceSize", extended_b_op );
		a_mOPCodes[0xB8] = Bytecode("o100_getSpriteGroupInfo", extended_b_op );
		a_mOPCodes[0xB9] = Bytecode("o100_getHeap", extended_b_op );
		a_mOPCodes[0xBA] = Bytecode("o100_getWizData", extended_b_op );
		// TODO: a_mOPCodes[0xbb] = bytecode("o6_isActorInBox" );
		a_mOPCodes[0xBC] = Bytecode("o6_isAnyOf" );
		// TODO: a_mOPCodes[0xbd] = bytecode("o6_getInventoryCount" );
		a_mOPCodes[0xBE] = Bytecode("o90_kernelGetFunctions" );
		a_mOPCodes[0xBF] = Bytecode("o90_max" );
		a_mOPCodes[0xC0] = Bytecode("o90_min" );
		a_mOPCodes[0xC1] = Bytecode("o72_getObjectImageX" );
		a_mOPCodes[0xC2] = Bytecode("o72_getObjectImageY" );
		a_mOPCodes[0xC3] = Bytecode("o6_isRoomScriptRunning" );
		// TODO: a_mOPCodes[0xc4] = bytecode("o90_getObjectData" );
		a_mOPCodes[0xC5] = Bytecode("o72_openFile" );
		a_mOPCodes[0xC6] = Bytecode("o90_getPolygonOverlap" );
		a_mOPCodes[0xC7] = Bytecode("o6_getOwner" );
		a_mOPCodes[0xC8] = Bytecode("o100_getPaletteData", extended_b_op );
		a_mOPCodes[0xC9] = Bytecode("o6_pickOneOf" );
		a_mOPCodes[0xCA] = Bytecode("o6_pickOneOfDefault" );
		a_mOPCodes[0xCB] = Bytecode("o80_pickVarRandom", extended_w_op );
		// TODO: a_mOPCodes[0xcc] = bytecode("o72_getPixel" );
		// TODO: a_mOPCodes[0xcd] = bytecode("o6_distObjectObject" );
		// TODO: a_mOPCodes[0xce] = bytecode("o6_distObjectPt" );
		// TODO: a_mOPCodes[0xcf] = bytecode("o6_distPtPt" );
		a_mOPCodes[0xD0] = Bytecode("o6_getRandomNumber" );
		a_mOPCodes[0xD1] = Bytecode("o6_getRandomNumberRange" );
		a_mOPCodes[0xD3] = Bytecode("o100_readFile", file_op_he100 );
		a_mOPCodes[0xD4] = Bytecode("o72_readINI", extended_b_op );
		a_mOPCodes[0xD5] = Bytecode("o80_readConfigFile", extended_b_op );
		a_mOPCodes[0xD6] = Bytecode("o6_isScriptRunning" );
		a_mOPCodes[0xD7] = Bytecode("o90_sin" );
		a_mOPCodes[0xD8] = Bytecode("o72_getSoundPosition" );
		a_mOPCodes[0xD9] = Bytecode("o6_isSoundRunning" );
		// TODO: a_mOPCodes[0xda] = bytecode("o80_getSoundVar" );
		a_mOPCodes[0xDB] = Bytecode("o100_getSpriteInfo", extended_b_op );
		a_mOPCodes[0xDC] = Bytecode("o90_sqrt" );
		a_mOPCodes[0xDD] = Bytecode("o6_startObjectQuick" );
		a_mOPCodes[0xDE] = Bytecode("o6_startScriptQuick2" );
		a_mOPCodes[0xDF] = Bytecode("o6_getState" );
		a_mOPCodes[0xE0] = Bytecode("o71_compareString" );
		a_mOPCodes[0xE1] = Bytecode("o71_copyString" );
		a_mOPCodes[0xE2] = Bytecode("o71_appendString" );
		// TODO: a_mOPCodes[0xe3] = bytecode("o71_concatString" );
		a_mOPCodes[0xE4] = Bytecode("o70_getStringLen" );
		a_mOPCodes[0xE5] = Bytecode("o71_getStringLenForWidth" );
		a_mOPCodes[0xE6] = Bytecode("o80_stringToInt" );
		a_mOPCodes[0xE7] = Bytecode("o71_getCharIndexInString" );
		a_mOPCodes[0xE8] = Bytecode("o71_getStringWidth" );
		a_mOPCodes[0xE9] = Bytecode("o60_readFilePos" );
		a_mOPCodes[0xEA] = Bytecode("o72_getTimer", extended_b_op );
		a_mOPCodes[0xEB] = Bytecode("o6_getVerbEntrypoint" );
		a_mOPCodes[0xEC] = Bytecode("o100_getVideoData", extended_b_op );
	}

	//======================================================================================
	inline void GetHE101codes(OPCodeMap& a_mOPCodes)
	{
		GetHE100codes(a_mOPCodes);

		a_mOPCodes[0xA8] = Bytecode("o72_debugInput" );
	}

	//======================================================================================
	inline void GetV8codes(OPCodeMap& a_mOPCodes)
	{
		a_mOPCodes[0x01] = Bytecode("o6_pushWord", extended_dw_op );
		a_mOPCodes[0x02] = Bytecode("o6_pushWordVar", extended_dw_op );
		a_mOPCodes[0x03] = Bytecode("o6_wordArrayRead", extended_dw_op );
		a_mOPCodes[0x04] = Bytecode("o6_wordArrayIndexedRead", extended_dw_op );
		a_mOPCodes[0x05] = Bytecode("o6_dup" );
		a_mOPCodes[0x06] = Bytecode("o6_pop" );
		a_mOPCodes[0x07] = Bytecode("o6_not" );
		a_mOPCodes[0x08] = Bytecode("o6_eq" );
		a_mOPCodes[0x09] = Bytecode("o6_neq" );
		a_mOPCodes[0x0A] = Bytecode("o6_gt" );
		a_mOPCodes[0x0B] = Bytecode("o6_lt" );
		a_mOPCodes[0x0C] = Bytecode("o6_le" );
		a_mOPCodes[0x0D] = Bytecode("o6_ge" );
		a_mOPCodes[0x0E] = Bytecode("o6_add" );
		a_mOPCodes[0x0F] = Bytecode("o6_sub" );
		a_mOPCodes[0x10] = Bytecode("o6_mul" );
		a_mOPCodes[0x11] = Bytecode("o6_div" );
		a_mOPCodes[0x12] = Bytecode("o6_land" );
		a_mOPCodes[0x13] = Bytecode("o6_lor" );
		a_mOPCodes[0x14] = Bytecode("o6_band" );
		a_mOPCodes[0x15] = Bytecode("o6_bor" );
		a_mOPCodes[0x16] = Bytecode("o8_mod" );
		a_mOPCodes[0x64] = Bytecode("o6_if" );
		a_mOPCodes[0x65] = Bytecode("o6_ifNot", djump_cmd );
		a_mOPCodes[0x66] = Bytecode("o6_jump", djump_cmd );
		a_mOPCodes[0x67] = Bytecode("o6_breakHere" );
		a_mOPCodes[0x68] = Bytecode("o6_delayFrames" );
		a_mOPCodes[0x69] = Bytecode("o8_wait", wait_ops_v8 );
		a_mOPCodes[0x6A] = Bytecode("o6_delay" );
		a_mOPCodes[0x6B] = Bytecode("o6_delaySeconds" );
		a_mOPCodes[0x6C] = Bytecode("o6_delayMinutes" );
		a_mOPCodes[0x6D] = Bytecode("o6_writeWordVar", extended_dw_op );
		a_mOPCodes[0x6E] = Bytecode("o6_wordVarInc", extended_dw_op );
		a_mOPCodes[0x6F] = Bytecode("o6_wordVarDec", extended_dw_op );
		a_mOPCodes[0x70] = Bytecode("o8_dimArray", extended_bdw_op );
		a_mOPCodes[0x71] = Bytecode("o6_wordArrayWrite", extended_dw_op );
		a_mOPCodes[0x72] = Bytecode("o6_wordArrayInc", extended_dw_op );
		a_mOPCodes[0x73] = Bytecode("o6_wordArrayDec", extended_dw_op );
		a_mOPCodes[0x74] = Bytecode("o8_dim2dimArray", extended_bdw_op );
		a_mOPCodes[0x75] = Bytecode("o6_wordArrayIndexedWrite", extended_dw_op );
		a_mOPCodes[0x76] = Bytecode("o8_arrayOps", array_ops_v8 );
		a_mOPCodes[0x79] = Bytecode("o6_startScript" );
		a_mOPCodes[0x7A] = Bytecode("o6_startScriptQuick" );
		a_mOPCodes[0x7B] = Bytecode("o6_stopObjectCodeScript" );  // o6_stopObjectCode
		a_mOPCodes[0x7C] = Bytecode("o6_stopScript" );
		a_mOPCodes[0x7D] = Bytecode("o6_jumpToScript" );
		a_mOPCodes[0x7E] = Bytecode("o6_dummy" );
		a_mOPCodes[0x7F] = Bytecode("o6_startObject" );
		a_mOPCodes[0x80] = Bytecode("o6_stopObjectScript" );
		a_mOPCodes[0x81] = Bytecode("o6_cutscene" );
		a_mOPCodes[0x82] = Bytecode("o6_endCutscene" );
		a_mOPCodes[0x83] = Bytecode("o6_freezeUnfreeze" );
		a_mOPCodes[0x84] = Bytecode("o6_beginOverride" );
		a_mOPCodes[0x85] = Bytecode("o6_endOverride" );
		a_mOPCodes[0x86] = Bytecode("o6_stopSentence" );
		a_mOPCodes[0x87] = Bytecode("o8_debug" );
		a_mOPCodes[0x89] = Bytecode("o6_setClass" );
		a_mOPCodes[0x8A] = Bytecode("o6_setState" );
		a_mOPCodes[0x8B] = Bytecode("o6_setOwner" );
		a_mOPCodes[0x8C] = Bytecode("o6_panCameraTo" );
		a_mOPCodes[0x8D] = Bytecode("o6_actorFollowCamera" );
		a_mOPCodes[0x8E] = Bytecode("o6_setCameraAt" );
		a_mOPCodes[0x8F] = Bytecode("o8_printActor", msg_cmd_v8 );  // o6_printActor
		a_mOPCodes[0x90] = Bytecode("o8_printEgo", msg_cmd_v8 );  // o6_printEgo
		a_mOPCodes[0x91] = Bytecode("o8_talkActor", msg_op_v8 );  // o6_talkActor
		a_mOPCodes[0x92] = Bytecode("o8_talkEgo", msg_op_v8 );  // o6_talkEgo
		a_mOPCodes[0x93] = Bytecode("o8_printLine", msg_cmd_v8 );  // o6_printLine
		a_mOPCodes[0x94] = Bytecode("o8_printText", msg_cmd_v8 );  // o6_printText
		a_mOPCodes[0x95] = Bytecode("o8_printDebug", msg_cmd_v8 );  // o6_printDebug
		a_mOPCodes[0x96] = Bytecode("o8_printSystem", msg_cmd_v8 );  // o6_printSystem
		a_mOPCodes[0x97] = Bytecode("o8_blastText", msg_cmd_v8 );
		a_mOPCodes[0x98] = Bytecode("o8_drawObject" );
		a_mOPCodes[0x9C] = Bytecode("o8_cursorCommand", extended_b_op );
		a_mOPCodes[0x9D] = Bytecode("o6_loadRoom" );
		a_mOPCodes[0x9E] = Bytecode("o6_loadRoomWithEgo" );
		a_mOPCodes[0x9F] = Bytecode("o6_walkActorToObj" );
		a_mOPCodes[0xA0] = Bytecode("o6_walkActorTo" );
		a_mOPCodes[0xA1] = Bytecode("o6_putActorAtXY" );
		a_mOPCodes[0xA2] = Bytecode("o6_putActorAtObject" );
		a_mOPCodes[0xA3] = Bytecode("o6_faceActor" );
		a_mOPCodes[0xA4] = Bytecode("o6_animateActor" );
		a_mOPCodes[0xA5] = Bytecode("o8_doSentence" );  // o6_doSentence
		a_mOPCodes[0xA6] = Bytecode("o6_pickupObject" );
		a_mOPCodes[0xA7] = Bytecode("o6_setBoxFlags" );
		a_mOPCodes[0xA8] = Bytecode("o6_createBoxMatrix" );
		a_mOPCodes[0xAA] = Bytecode("o8_resourceRoutines", extended_b_op );
		a_mOPCodes[0xAB] = Bytecode("o8_roomOps", extended_b_op );
		a_mOPCodes[0xAC] = Bytecode("o8_actorOps", actor_ops_v8 );
		a_mOPCodes[0xAD] = Bytecode("o8_cameraOps", extended_b_op );
		a_mOPCodes[0xAE] = Bytecode("o8_verbOps", verb_ops_v8 );
		a_mOPCodes[0xAF] = Bytecode("o6_startSound" );
		a_mOPCodes[0xB0] = Bytecode("o6_startMusic" );
		a_mOPCodes[0xB1] = Bytecode("o6_stopSound" );
		a_mOPCodes[0xB2] = Bytecode("o6_soundKludge" );
		a_mOPCodes[0xB3] = Bytecode("o8_systemOps", extended_b_op );
		a_mOPCodes[0xB4] = Bytecode("o6_saveRestoreVerbs", extended_b_op );
		a_mOPCodes[0xB5] = Bytecode("o6_setObjectName", msg_op_v8 );
		a_mOPCodes[0xB6] = Bytecode("o6_getDateTime" );
		a_mOPCodes[0xB7] = Bytecode("o6_drawBox" );
		a_mOPCodes[0xB9] = Bytecode("o8_startVideo", msg_op_v8 );
		a_mOPCodes[0xBA] = Bytecode("o8_kernelSetFunctions" );
		a_mOPCodes[0xC8] = Bytecode("o6_startScriptQuick2" );
		a_mOPCodes[0xC9] = Bytecode("o6_startObjectQuick" );
		a_mOPCodes[0xCA] = Bytecode("o6_pickOneOf" );
		a_mOPCodes[0xCB] = Bytecode("o6_pickOneOfDefault" );
		a_mOPCodes[0xCD] = Bytecode("o6_isAnyOf" );
		a_mOPCodes[0xCE] = Bytecode("o6_getRandomNumber" );
		a_mOPCodes[0xCF] = Bytecode("o6_getRandomNumberRange" );
		a_mOPCodes[0xD0] = Bytecode("o6_ifClassOfIs" );
		a_mOPCodes[0xD1] = Bytecode("o6_getState" );
		a_mOPCodes[0xD2] = Bytecode("o6_getOwner" );
		a_mOPCodes[0xD3] = Bytecode("o6_isScriptRunning" );
		a_mOPCodes[0xD5] = Bytecode("o6_isSoundRunning" );
		a_mOPCodes[0xD6] = Bytecode("o6_abs" );
		a_mOPCodes[0xD8] = Bytecode("o8_kernelGetFunctions" );
		a_mOPCodes[0xD9] = Bytecode("o6_isActorInBox" );
		a_mOPCodes[0xDA] = Bytecode("o6_getVerbEntrypoint" );
		a_mOPCodes[0xDB] = Bytecode("o6_getActorFromXY" );
		a_mOPCodes[0xDC] = Bytecode("o6_findObject" );
		a_mOPCodes[0xDD] = Bytecode("o6_getVerbFromXY" );
		a_mOPCodes[0xDF] = Bytecode("o6_findInventory" );
		a_mOPCodes[0xE0] = Bytecode("o6_getInventoryCount" );
		a_mOPCodes[0xE1] = Bytecode("o6_getAnimateVariable" );
		a_mOPCodes[0xE2] = Bytecode("o6_getActorRoom" );
		a_mOPCodes[0xE3] = Bytecode("o6_getActorWalkBox" );
		a_mOPCodes[0xE4] = Bytecode("o6_getActorMoving" );
		a_mOPCodes[0xE5] = Bytecode("o6_getActorCostume" );
		a_mOPCodes[0xE6] = Bytecode("o6_getActorScaleX" );
		a_mOPCodes[0xE7] = Bytecode("o6_getActorLayer" );
		a_mOPCodes[0xE8] = Bytecode("o6_getActorElevation" );
		a_mOPCodes[0xE9] = Bytecode("o6_getActorWidth" );
		a_mOPCodes[0xEA] = Bytecode("o6_getObjectNewDir" );
		a_mOPCodes[0xEB] = Bytecode("o6_getObjectX" );
		a_mOPCodes[0xEC] = Bytecode("o6_getObjectY" );
		a_mOPCodes[0xED] = Bytecode("o8_getActorChore" );
		a_mOPCodes[0xEE] = Bytecode("o6_distObjectObject" );
		a_mOPCodes[0xEF] = Bytecode("o6_distPtPt" );
		a_mOPCodes[0xF0] = Bytecode("o8_getObjectImageX" );
		a_mOPCodes[0xF1] = Bytecode("o8_getObjectImageY" );
		a_mOPCodes[0xF2] = Bytecode("o8_getObjectImageWidth" );
		a_mOPCodes[0xF3] = Bytecode("o8_getObjectImageHeight" );
		a_mOPCodes[0xF6] = Bytecode("o8_getStringWidth", msg_op_v8 );
		a_mOPCodes[0xF7] = Bytecode("o8_getActorZPlane" );
	}

	//======================================================================================
	inline void GetOPCodeTable(OPCodeMap& a_mMap, int version, int heVersion)
	{
		if (heVersion >= 101)
		{
			GetHE101codes(a_mMap);
		}
		else if (heVersion >= 100)
		{
			GetHE100codes(a_mMap);
		}
		else if (heVersion >= 90)
		{
			GetHE90codes(a_mMap);
		}
		else if (heVersion >= 80)
		{
			GetHE80codes(a_mMap);
		}
		else if (heVersion >= 73)
		{
			GetHE73codes(a_mMap);
		}
		else if (heVersion >= 72)
		{
			GetHE72codes(a_mMap);
		}
		else if (heVersion >= 71)
		{
			GetHE71codes(a_mMap);
		}
		else if (heVersion >= 70)
		{
			GetHE70codes(a_mMap);
		}
		else if (heVersion >= 60)
		{
			GetHE60codes(a_mMap);
		}
		else if (version >= 8)
		{
			GetV8codes(a_mMap);
		}
		else if (version >= 6)
		{
			GetV6codes(a_mMap);
		}
		else
		{
			GetV6codes(a_mMap);
		}
	}
}