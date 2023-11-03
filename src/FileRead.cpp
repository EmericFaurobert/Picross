#include "FileRead.h"
#include <string>
#include <sstream> 
#include <algorithm>

FileStream::FileStream(const std::string & filepath)
{
	file = std::fstream(filepath);
}

bool FileStream::nextLine(std::string & str)
{
	std::getline(file, str);

	if (file.bad())
	{
		file.clear();
		return false;
	}

	return true;
}

bool FileStream::findSubStr(std::string searchedStr)
{
	if (file.is_open())
	{
		int currentPos = file.beg;

		// Reset the file stream (prevent order sensitive)
		file.clear();
		file.seekg(0, file.beg);

		bothEndStrTrim(searchedStr);	// Prevent space sensitive
		std::transform(searchedStr.begin(), searchedStr.end(), searchedStr.begin(), std::tolower);	// Prevent case sensitive
		
		std::string currentLine = "";

		while (std::getline(file, currentLine))
		{
			std::stringstream sstream(currentLine);
			std::string currentWord = "";

			std::getline(sstream, currentWord, ':');	// Parse ':' delimitor
			bothEndStrTrim(currentWord);			// Prevent space sensitive
			std::transform(currentWord.begin(), currentWord.end(), currentWord.begin(), std::tolower);	// Prevent case sensitive

			if (currentWord == searchedStr)
			{
				// Put the filestream back to after the substring found
				file.clear();
				file.seekg(currentPos + sstream.tellg(), file.beg);

				return true;
			}

			currentPos = file.tellg();	// We advance stream position to next line
		} 	
	}

	return false;
}

bool FileStream::parseValue(const std::string & attrName, std::string & val)
{
	bool found = false;

	if (findSubStr(attrName))
	{
		std::getline(file, val);
		bothEndStrTrim(val);	// Prevent space sensitive
		found = true;
	}

	return found;	
};
