#include "FileReadWrite.h"
#include <string>
#include <sstream> 
#include <algorithm>

FileStream::FileStream(const std::string & filepath, std::ios_base::openmode mode)
{
	file = std::fstream(filepath, mode);
}

bool FileStream::parseValue(const std::string & attrName, std::string & val)
{
	if (file.is_open())
	{		
		// Reset the file stream (prevent order sensitive)
		file.clear();
		file.seekg(0, file.beg);

		std::string searchedAttr = attrName;
		TrimBothStringEnds(searchedAttr);																// Prevent space sensitive
		std::transform(searchedAttr.begin(), searchedAttr.end(), searchedAttr.begin(), std::tolower);	// Prevent case sensitive

		int currentPos = file.beg;
		std::string currentFileLine = "";

		while (std::getline(file, currentFileLine))
		{
			std::stringstream sstream(currentFileLine);
			std::string currentFileWord = "";

			std::getline(sstream, currentFileWord, ':');	// Parse ':' delimitor
			TrimBothStringEnds(currentFileWord);			// Prevent space sensitive
			std::transform(currentFileWord.begin(), currentFileWord.end(), currentFileWord.begin(), std::tolower);	// Prevent case sensitive

			if (currentFileWord == searchedAttr)
			{
				std::getline(sstream, val);
				TrimBothStringEnds(val);
				file.clear();
				return true;
			}
		}
	}

	return false;
};

bool FileStream::readNextLine(std::string & str)
{
	if (file.is_open())
	{
		std::getline(file, str);

		if (file.bad())
		{
			file.clear();
			return false;
		}

		return true;
	}

	return false;
}

bool FileStream::readAllContent(std::string & str)
{
	if (file.is_open())
	{
		// Reset the file stream
		file.clear();
		file.seekg(0, file.beg);

		// Read whole content
		std::ostringstream outStringStream;
		outStringStream << file.rdbuf();
		str = outStringStream.str();

		return true;
	}

	return false;
}

bool FileStream::writeContent(const std::string & str)
{
	if (file.is_open())
	{
		file << str;

		if (file.bad())
		{
			file.clear();
			return false;
		}

		return true;
	}

	return false;
}
