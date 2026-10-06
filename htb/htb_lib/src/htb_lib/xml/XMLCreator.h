#pragma once

#include <cstddef>

namespace tinyxml2
{
	class XMLElement;
	class XMLDocument;
}
namespace htb::archive
{
	class Archive;
}
namespace htb::parsing
{
	class Chunk;
}
namespace htb::xml
{
	struct XMLSettings;

	/// <summary>
	/// Creates an XML from an archive.
	/// </summary>
	/// <param name="a_Archive">The archive to create an XML from.</param>
	/// <param name="a_Document">The document to put the XML data into.</param>
	/// <param name="a_XMLInfo">XML settings.</param>
	/// <returns>True if XML creation was successful, false otherwise.</returns>
	bool CreateXMLFromArchive(const archive::Archive& a_Archive, tinyxml2::XMLDocument& a_Document, const XMLSettings& a_XMLInfo);

	/// <summary>
	/// Creates an XML from a chunk.
	/// </summary>
	/// <param name="a_Chunk">The chunk to createa an XML from.</param>
	/// <param name="a_Element">The element to put the XML data into.</param>
	/// <param name="a_iOffset">The offset of the chunk.</param>
	/// <param name="a_XMLInfo">XML settings.</param>
	/// <param name="a_iCurrentDepth">The current depth of XML elements.</param>
	/// <returns>True if XML creation was successful, false otherwise.</returns>
	bool CreateXMLElementFromChunk(const parsing::Chunk& a_Chunk, tinyxml2::XMLElement& a_Element, size_t& a_iOffset, const XMLSettings& a_XMLInfo, int a_iCurrentDepth = 0);
}
