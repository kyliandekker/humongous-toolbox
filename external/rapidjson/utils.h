#pragma once

#include <rapidjson/document.h>
#include <string>

namespace rapidjson
{
	inline bool GetString(const rapidjson::Value& a_Document, const std::string& a_sMemberName, std::string& a_Value)
	{
		if (!a_Document.HasMember(a_sMemberName.c_str()))
		{
			return false;
		}
		if (!a_Document[a_sMemberName.c_str()].IsString())
		{
			return false;
		}
		a_Value = a_Document[a_sMemberName.c_str()].GetString();
		return true;
	}

	inline bool GetBool(const rapidjson::Value& a_Document, const std::string& a_sMemberName, bool& a_Value)
	{
		if (!a_Document.HasMember(a_sMemberName.c_str()))
		{
			return false;
		}
		if (!a_Document[a_sMemberName.c_str()].IsBool())
		{
			return false;
		}
		a_Value = a_Document[a_sMemberName.c_str()].GetBool();
		return true;
	}

	inline bool GetInt(const rapidjson::Value& a_Document, const std::string& a_sMemberName, int& a_Value)
	{
		if (!a_Document.HasMember(a_sMemberName.c_str()))
		{
			return false;
		}
		if (!a_Document[a_sMemberName.c_str()].IsInt())
		{
			return false;
		}
		a_Value = a_Document[a_sMemberName.c_str()].GetInt();
		return true;
	}
}
