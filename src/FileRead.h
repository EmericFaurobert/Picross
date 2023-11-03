#pragma once

#include <fstream>
#include <string>

inline void bothEndStrTrim(std::string & str) { str.erase(remove_if(str.begin(), str.end(), isspace), str.end()); };	// Trim a string both ends

class FileStream
{
public:
	FileStream(const std::string & filepath);
	~FileStream() { file.close(); };

	bool nextLine(std::string & str);			// Gives the file stream next line (or the end of the current one if not already)
	bool parseValue(const std::string & attrName, std::string & val);	// Parse "attrName: val" strings (not case sensitive)

private:
	bool findSubStr(std::string searchedStr);	// Seach for a substring and set the stream after its position (or at eof if not found)

private:
	std::fstream file;
};