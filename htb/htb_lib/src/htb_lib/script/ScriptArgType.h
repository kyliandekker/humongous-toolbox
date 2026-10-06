#pragma once

namespace htb::script
{
	//======================================================================================
	// EScriptArgType
	//======================================================================================
	/// <summary>
	/// The argument type of a script argument. Determines the size and the behaviour.
	/// </summary>
	enum class EScriptArgType
	{
		BYTE, // Unsigned.
		INT16, // Signed.
		INT32, // Signed.
		REF, // Signed, Int16 or Int32 depending on version. Signed because it can jump to a previous position.
		STRING, // Keep in mind it is the size of string + null termination.
	};
}
