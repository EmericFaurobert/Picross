#include <sstream> 
#include <algorithm>
#include "FileReadWrite.h"

FileStream::FileStream(const std::wstring & filepath, std::ios_base::openmode mode)
{
	file = std::wfstream(filepath, mode);
}

bool FileStream::parseValue(const std::wstring & attrName, std::wstring & val)
{
	if (file.is_open())
	{		
		// Reset the file stream (prevent order sensitive)
		file.clear();
		file.seekg(0, file.beg);

		std::wstring searchedAttr = attrName;
		TrimBothStringEnds(searchedAttr);																// Prevent space sensitive
		std::transform(searchedAttr.begin(), searchedAttr.end(), searchedAttr.begin(), std::tolower);	// Prevent case sensitive

		int currentPos = file.beg;
		std::wstring currentFileLine = L"";

		while (std::getline(file, currentFileLine))
		{
			std::wstringstream sstream(currentFileLine);
			std::wstring currentFileWord = L"";

			std::getline(sstream, currentFileWord, L':');	// Parse ':' delimitor
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

bool FileStream::readNextLine(std::wstring & str)
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

bool FileStream::readAllContent(std::wstring & str)
{
	if (file.is_open())
	{
		// Reset the file stream
		file.clear();
		file.seekg(0, file.beg);

		// Read whole content
		std::wostringstream outStringStream;
		outStringStream << file.rdbuf();
		str = outStringStream.str();

		return true;
	}

	return false;
}

bool FileStream::writeContent(const std::wstring & str)
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
