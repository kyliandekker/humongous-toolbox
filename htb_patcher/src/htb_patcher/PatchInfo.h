#pragma once

#include <cstdint>
#include <array>

namespace htb::patch
{
	constexpr uint8_t PATCHER_BG_MOBCOM = 4;
	constexpr uint8_t PATCHER_BG_JUNGLE = 8;
	constexpr uint8_t PATCHER_BG_PYRAMID = 10;
	constexpr uint8_t PATCHER_BG_PIZZA = 13;
	constexpr uint8_t PATCHER_BG_PRO = 16;
	constexpr uint8_t PATCHER_BG_LAKESHORE = 24;
	constexpr uint8_t PATCHER_BG_CACTUS = 33;
	constexpr uint8_t PATCHER_BG_TOILET = 48;

	constexpr std::array<uint8_t, 8> PATCHER_BACKGROUND = {
		PATCHER_BG_MOBCOM,
		PATCHER_BG_JUNGLE,
		PATCHER_BG_PYRAMID,
		PATCHER_BG_PIZZA,
		PATCHER_BG_PRO,
		PATCHER_BG_LAKESHORE,
		PATCHER_BG_CACTUS,
		PATCHER_BG_TOILET
	};

	constexpr uint32_t SF3_VO_INTERESTING_BUT_I_DONT_THINK_THAT_WILL_DO_ME_ANY_GOOD =												37;
	constexpr uint32_t SF3_VO_THATS_NOT_GOING_TO_DO_ME_ANY_GOOD =																	38;
	constexpr uint32_t SF3_VO_I_CANT_USE_THAT_THERE =																				39;
	constexpr uint32_t SF3_VO_THATS_NOT_THE_BEST_USE_OF_MY_SPY_GADGET =																40;

	constexpr uint32_t SF3_VO_I_NEED_TO_FIGURE_OUT_HOW_TO_GET_INSIDE =																78;

	constexpr uint32_t SF3_VO_PANCAKES =																							87;

	constexpr uint32_t SF3_VO_SPIONNENSPULLEN =																						103;
	constexpr uint32_t SF3_VO_AANTEKENINGEN =																						104;
	constexpr uint32_t SF3_VO_TEKSTBALLONNEN =																						105;
	constexpr uint32_t SF3_VO_SPIONKLOK =																							106;

	constexpr uint32_t SF3_VO_SAVE =																								238;
	constexpr uint32_t SF3_VO_LOAD =																								239;
	constexpr uint32_t SF3_VO_QUIT =																								240;
	constexpr uint32_t SF3_VO_FUN =																									241;
	constexpr uint32_t SF3_VO_MOBILE_COMMAND_CENTER =																				242;

	constexpr uint32_t SF3_VO_GREAT =																								276;
	constexpr uint32_t SF3_VO_THANKS_FOR_THE_INFORMATION =																			277;

	constexpr uint32_t SF3_VO_GOOD_LUCK_SPY_FOX =																					318;
	constexpr uint32_t SF3_VO_HI_SPY_FOX =																							319;
	constexpr uint32_t SF3_VO_HOWS_THE_MISSION_GOING =																				320;

	constexpr uint32_t SF3_VO_MONKEY_PENNY_OUT =																					330;
	constexpr uint32_t SF3_VO_WHAT_IS_YOUR_MISSION_UPDATES_SPY_FOX =																331;
	
	constexpr uint32_t SF3_VO_SPY_FOX_WHY_ARE_YOU_USING_YOUR_VERY_EXPENSIVE_SPY_WATCH_TO_CALL_ME_WHEN_IM_RIGHT_HERE_IN_THE_ROOM =	340;
	
	constexpr uint32_t SF3_VO_SPYFOX_OUT =																							342;
	constexpr uint32_t SF3_VO_IM_RIGHT_BEHIND_YOU_SPY_FOX =																			343;

	constexpr uint32_t SF3_VO_IM_RIGHT_HERE_IN_THE_ROOM_WITH_YOU_SPY_FOX =															347;

	constexpr uint32_t SF3_VO_HOWS_THE_MISSION_GOING_SPY_FOX =																		380;

	constexpr uint32_t SF3_VO_WHAT_IS_YOUR_MISSION_UPDATES_SPY_FOX_2 =																395;

	constexpr uint32_t SF3_VO_GOOD_LUCK_SPY_FOX_2 =																					430;

	constexpr uint32_t SF3_VO_THANKS_MONKEY_PENNY =																					439;

	constexpr uint32_t SF3_VO_THANKS =																								551;

	constexpr uint32_t SF3_VO_HAVE_YOU_SEEN_IT_ANYWHERE =																			564;


	constexpr uint32_t SF3_VO_INTERESTING =																							627;

	constexpr uint32_t SF3_VO_WHATS_THE_SPY_ACTION_ON_THIS_GADGET_QUACK =															689;

	constexpr uint32_t SF3_VO_HOW_DOES_THIS_GADGET_WORK_QUACK =																		691;

	constexpr uint32_t SF3_VO_WHAT_DOES_THIS_GADGET_DO_PROFESSOR_QUACK =															693;

	constexpr uint32_t SF3_VO_A =																									902;
	constexpr uint32_t SF3_VO_B =																									903;
	constexpr uint32_t SF3_VO_C =																									904;
	constexpr uint32_t SF3_VO_D =																									905;
	constexpr uint32_t SF3_VO_E =																									906;
	constexpr uint32_t SF3_VO_F =																									907;
	constexpr uint32_t SF3_VO_G =																									908;
	constexpr uint32_t SF3_VO_H =																									909;
	constexpr uint32_t SF3_VO_I =																									910;
	constexpr uint32_t SF3_VO_J =																									911;
	constexpr uint32_t SF3_VO_K =																									912;
	constexpr uint32_t SF3_VO_L =																									913;
	constexpr uint32_t SF3_VO_M =																									914;
	constexpr uint32_t SF3_VO_N =																									915;
	constexpr uint32_t SF3_VO_O =																									916;
	constexpr uint32_t SF3_VO_P =																									917;
	constexpr uint32_t SF3_VO_Q =																									918;
	constexpr uint32_t SF3_VO_R =																									919;
	constexpr uint32_t SF3_VO_S =																									920;
	constexpr uint32_t SF3_VO_T =																									921;
	constexpr uint32_t SF3_VO_U =																									922;
	constexpr uint32_t SF3_VO_V =																									923;
	constexpr uint32_t SF3_VO_W =																									924;
	constexpr uint32_t SF3_VO_X =																									925;
	constexpr uint32_t SF3_VO_Y =																									926;
	constexpr uint32_t SF3_VO_Z =																									927;

	constexpr uint32_t SF3_VO_FASCINATING =																							1167;

	constexpr uint32_t SF3_VO_ZERO =																								3691;
	constexpr uint32_t SF3_VO_ONE =																									3692;
	constexpr uint32_t SF3_VO_TWO =																									3693;
	constexpr uint32_t SF3_VO_THREE =																								3694;
	constexpr uint32_t SF3_VO_FOUR =																								3695;
	constexpr uint32_t SF3_VO_FIVE =																								3696;
	constexpr uint32_t SF3_VO_SIX =																									3697;
	constexpr uint32_t SF3_VO_SEVEN =																								3698;
	constexpr uint32_t SF3_VO_EIGHT =																								3699;
	constexpr uint32_t SF3_VO_NINE =																								3700;

	constexpr uint32_t SF3_VO_I_DID_IT =																							3844;

	constexpr uint32_t SF3_VO_ILL_COME_BACK_TO_THIS_LATER =																			3907;

	constexpr uint32_t SF3_VO_ILL_TAKE_A_BREAK_AND_COME_BACK_TO_THIS_LATER =														3909;

	constexpr uint32_t SF3_VO_I_HAVE_TO_GET_THIS_SAFE_OPEN =																		4013;

	constexpr uint32_t SF3_VO_IM_GOING_TO_NEED_THAT_KEY_BUT_ILL_COME_BACK_LATER =													4018;

	constexpr uint32_t SF3_VO_ILL_TRY_IT_AGAIN_LATER =																				4020;

	constexpr uint32_t SF3_VO_I_NEED_TO_ENTER_THE_CODE_CORRECTLY_TO_OPEN_THAT_DOOR_A =												4022;

	// PAIR IS: SF2 INDEX, SF3 INDEX.
	const std::vector<std::pair<uint32_t, uint32_t>> SF2_VO_INDEX = {
		/* CHECKED ✓ */ { 85,			SF3_VO_INTERESTING_BUT_I_DONT_THINK_THAT_WILL_DO_ME_ANY_GOOD },
		/* CHECKED ✓ */ { 3,			SF3_VO_THATS_NOT_GOING_TO_DO_ME_ANY_GOOD },
		/* CHECKED ✓ */ { 1,			SF3_VO_I_CANT_USE_THAT_THERE },
		/* CHECKED ✓ */ { 2334,			SF3_VO_I_NEED_TO_FIGURE_OUT_HOW_TO_GET_INSIDE },
		/* CHECKED ✓ */ { 59,			SF3_VO_PANCAKES },
		/* CHECKED ✓ */ { 105,			SF3_VO_SPIONNENSPULLEN },
		/* CHECKED ✓ */ { 106,			SF3_VO_AANTEKENINGEN },
		/* CHECKED ✓ */ { 107,			SF3_VO_TEKSTBALLONNEN },
		/* CHECKED ✓ */ { 108,			SF3_VO_SPIONKLOK },
		/* CHECKED ✓ */ { 174,			SF3_VO_SAVE },
		/* CHECKED ✓ */ { 175,			SF3_VO_LOAD },
		/* CHECKED ✓ */ { 176,			SF3_VO_QUIT },
		/* CHECKED ✓ */ { 177,			SF3_VO_FUN },
		/* CHECKED ✓ */ { 178,			SF3_VO_MOBILE_COMMAND_CENTER },
		/* CHECKED ✓ */ { 946,			SF3_VO_GREAT },
		/* CHECKED ✓ */ { 1171,			SF3_VO_THANKS_FOR_THE_INFORMATION },
		/* CHECKED ✓ */ { 184,			SF3_VO_MONKEY_PENNY_OUT },
		/* CHECKED ✓ */ { 300,			SF3_VO_WHAT_IS_YOUR_MISSION_UPDATES_SPY_FOX },
		/* CHECKED ✓ */ { 216,			SF3_VO_SPY_FOX_WHY_ARE_YOU_USING_YOUR_VERY_EXPENSIVE_SPY_WATCH_TO_CALL_ME_WHEN_IM_RIGHT_HERE_IN_THE_ROOM },
		/* CHECKED ✓ */ { 185,			SF3_VO_SPYFOX_OUT },
		/* CHECKED ✓ */ { 212,			SF3_VO_IM_RIGHT_BEHIND_YOU_SPY_FOX },
		/* CHECKED ✓ */ { 3634,			SF3_VO_THANKS },
		/* CHECKED ✓ */ { 1181,			SF3_VO_HAVE_YOU_SEEN_IT_ANYWHERE }, /*"HEB JE DIE GEZIEN (1881)" VS "HEB JE DAT GEZIEN (2216)"*/
		/* CHECKED ✓ */ { 509,			SF3_VO_INTERESTING },
		/* CHECKED ✓ */ { 1180,			SF3_VO_WHATS_THE_SPY_ACTION_ON_THIS_GADGET_QUACK },
		/* CHECKED ✓ */ { 1194,			SF3_VO_HOW_DOES_THIS_GADGET_WORK_QUACK },
		/* CHECKED ✓ */ { 1195,			SF3_VO_WHAT_DOES_THIS_GADGET_DO_PROFESSOR_QUACK },
		/* CHECKED ✓ */ { 3993,			SF3_VO_A },
		/* CHECKED ✓ */ { 3994,			SF3_VO_B },
		/* CHECKED ✓ */ { 3995,			SF3_VO_C },
		/* CHECKED ✓ */ { 3996,			SF3_VO_D },
		/* CHECKED ✓ */ { 3997,			SF3_VO_E },
		/* CHECKED ✓ */ { 3998,			SF3_VO_F },
		/* CHECKED ✓ */ { 3999,			SF3_VO_G },
		/* CHECKED ✓ */ { 4000,			SF3_VO_H },
		/* CHECKED ✓ */ { 4001,			SF3_VO_I },
		/* CHECKED ✓ */ { 4002,			SF3_VO_J },
		/* CHECKED ✓ */ { 4003,			SF3_VO_K },
		/* CHECKED ✓ */ { 4004,			SF3_VO_L },
		/* CHECKED ✓ */ { 4005,			SF3_VO_M },
		/* CHECKED ✓ */ { 4006,			SF3_VO_N },
		/* CHECKED ✓ */ { 4007,			SF3_VO_O },
		/* CHECKED ✓ */ { 4008,			SF3_VO_P },
		/* CHECKED ✓ */ { 4009,			SF3_VO_Q },
		/* CHECKED ✓ */ { 4010,			SF3_VO_R },
		/* CHECKED ✓ */ { 4011,			SF3_VO_S },
		/* CHECKED ✓ */ { 4012,			SF3_VO_T },
		/* CHECKED ✓ */ { 4013,			SF3_VO_U },
		/* CHECKED ✓ */ { 4014,			SF3_VO_V },
		/* CHECKED ✓ */ { 4015,			SF3_VO_W },
		/* CHECKED ✓ */ { 4016,			SF3_VO_X },
		/* CHECKED ✓ */ { 4017,			SF3_VO_Y },
		/* CHECKED ✓ */ { 4018,			SF3_VO_Z },
		/* CHECKED ✓ */ { 738,			SF3_VO_FASCINATING },
		/* CHECKED ✓ */ { 3983,			SF3_VO_ZERO },
		/* CHECKED ✓ */ { 3984,			SF3_VO_ONE },
		/* CHECKED ✓ */ { 3985,			SF3_VO_TWO },
		/* CHECKED ✓ */ { 3986,			SF3_VO_THREE },
		/* CHECKED ✓ */ { 3987,			SF3_VO_FOUR },
		/* CHECKED ✓ */ { 3988,			SF3_VO_FIVE },
		/* CHECKED ✓ */ { 3989,			SF3_VO_SIX },
		/* CHECKED ✓ */ { 3990,			SF3_VO_SEVEN },
		/* CHECKED ✓ */ { 3991,			SF3_VO_EIGHT },
		/* CHECKED ✓ */ { 3992,			SF3_VO_NINE },
		/* CHECKED ✓ */ { 2337,			SF3_VO_I_NEED_TO_ENTER_THE_CODE_CORRECTLY_TO_OPEN_THAT_DOOR_A },
	};

	// PAIR IS: SF1 INDEX, SF3 INDEX.
	const std::vector<std::pair<uint32_t, uint32_t>> SF1_VO_INDEX = {
		/* CHECKED ✓ */ { 3245,			SF3_VO_THATS_NOT_THE_BEST_USE_OF_MY_SPY_GADGET },
		/* CHECKED ✓ */ { 2826,			SF3_VO_GOOD_LUCK_SPY_FOX },
		/* CHECKED ✓ */ { 3915,			SF3_VO_HI_SPY_FOX },
		/* CHECKED ✓ */ { 3955,			SF3_VO_HOWS_THE_MISSION_GOING },
		/* CHECKED ✓ */ { 4213,			SF3_VO_THANKS_MONKEY_PENNY },
		/* CHECKED ✓ */ { 3951,			SF3_VO_IM_RIGHT_HERE_IN_THE_ROOM_WITH_YOU_SPY_FOX },
		/* CHECKED ✓ */ { 4117,			SF3_VO_HOWS_THE_MISSION_GOING_SPY_FOX },
		/* CHECKED ✓ */ { 4117,			SF3_VO_WHAT_IS_YOUR_MISSION_UPDATES_SPY_FOX_2 },
		/* CHECKED ✓ */ { 2826,			SF3_VO_GOOD_LUCK_SPY_FOX_2 },
		/* CHECKED ✓ */ { 4720,			SF3_VO_I_DID_IT },
		/* CHECKED ✓ */ { 3096,			SF3_VO_ILL_COME_BACK_TO_THIS_LATER },
		/* CHECKED ✓ */ { 3093,			SF3_VO_ILL_TAKE_A_BREAK_AND_COME_BACK_TO_THIS_LATER },
		/* CHECKED ✓ */ { 3094,			SF3_VO_I_HAVE_TO_GET_THIS_SAFE_OPEN },
		/* CHECKED ✓ */ { 3099,			SF3_VO_IM_GOING_TO_NEED_THAT_KEY_BUT_ILL_COME_BACK_LATER },
		/* CHECKED ✓ */ { 3100,			SF3_VO_ILL_TRY_IT_AGAIN_LATER },
	};
}