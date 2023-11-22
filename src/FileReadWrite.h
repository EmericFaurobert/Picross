#pragma once

#include <fstream>


// Trim spaces of the string passed at both ends
inline void TrimBothStringEnds(std::string & str) {	str.erase(remove_if(str.begin(), str.end(), isspace), str.end()); };


class FileStream
{
public:
	FileStream(const std::string & filepath, std::ios_base::openmode mode = std::ios_base::in);
	~FileStream() { file.close(); };

	bool parseValue(const std::string& attrName, std::string& val);	// Parse "attrName: val" strings
	bool readNextLine(std::string & str);							// Read the file next line
	bool readAllContent(std::string& str);							// Read the whole file content into a string
	bool writeContent(const std::string & str);						// Write the passed string to the file

private:
	std::fstream file;
};