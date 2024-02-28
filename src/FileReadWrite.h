#pragma once

#include <fstream>


// Trim spaces of the string passed at both ends
inline void TrimBothStringEnds(std::wstring & str) { str.erase(remove_if(str.begin(), str.end(), isspace), str.end()); };


class FileStream
{
public:
	FileStream(const std::wstring & filepath, std::ios_base::openmode mode = std::ios_base::in);
	~FileStream() { file.close(); };

	bool parseValue(const std::wstring& attrName, std::wstring& val);	// Parse "attrName: val" strings
	bool readNextLine(std::wstring & str);							// Read the file next line
	bool readAllContent(std::wstring& str);							// Read the whole file content into a string
	bool writeContent(const std::wstring & str);						// Write the passed string to the file

private:
	std::wfstream file;
};