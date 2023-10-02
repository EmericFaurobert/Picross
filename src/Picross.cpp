#include "Picross.h"
#include <fstream>
#include <iostream>

Picross::Picross(const std::string & fileName)
{
	auto trimStr = [](std::string & str)
	{
		str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
	};

	auto readCorrespondingValue = [&trimStr](std::ifstream & fileStream, const std::string & searchedStr, std::string & foundLine)
	{
		// Reset the stream
		fileStream.clear();
		fileStream.seekg(0, fileStream.beg);

		std::string readLine = "";

		while (!fileStream.eof())
		{	
			std::getline(fileStream, readLine);

			if (readLine.compare(0, searchedStr.size(), searchedStr) == 0)
			{
				foundLine = readLine.substr(searchedStr.size() + 1, std::string::npos);
				trimStr(foundLine);
				return true;
			}
		}

		return false;
	};

	std::ifstream inFile(fileName);

	if (inFile.is_open())
	{
		std::string readVal = "";

		// Grid Size
		if (readCorrespondingValue(inFile, "Width", readVal))
			width = std::min(std::stoi(readVal), UCHAR_MAX);

		if (readCorrespondingValue(inFile, "Height", readVal))
			height = std::min(std::stoi(readVal), UCHAR_MAX);

		grid.resize(width * height);

		assert(("Picross size is larger than 20x20", width <= 20 || height <= 20));	// We don't handle larger than 20x20 grids

		// StateGrid
		if (readCorrespondingValue(inFile, "StateGrid", readVal))
		{
			char tempChr = ' ';

			for (uint row = 0; row < height; ++row)
			{
				for (uint col = 0; col < width; ++col)
				{
					inFile >> tempChr;
					if (tempChr == 'X')
						SetExpectedState(row, col, State::checked);
				}
			}
		}

		// Colors
		if (readCorrespondingValue(inFile, "Colored", readVal))
			isColored = (readVal == "yes" || readVal == "true") ? true : false;

		// ColorPalette
		if (readCorrespondingValue(inFile, "ColorPalette", readVal))
		{
			std::string tempStr = "";
			uint idx = 0, red = 0, green = 0, blue = 0;

			for (uint i = 0; i < std::stoi(readVal); ++i)
			{
				inFile >> idx >> tempStr >> red >> green >> blue;
				colorPalette[idx] = Color(red, green, blue);
			}

			// ColorGrid
			if (readCorrespondingValue(inFile, "ColorGrid", readVal))
			{
				uint colorIdx = 0;

				for (uint row = 0; row < height; ++row)
				{
					for (uint col = 0; col < width; ++col)
					{
						inFile >> colorIdx;
						SetExpectedColor(row, col, colorIdx);
					}
				}
			}
		}
	}

	// Compute Clues
	GenerateClues();
}

Color Picross::GetColor(uchar colorIdx) const
{
	ColorPalette::const_iterator result = colorPalette.find(colorIdx);
	return (result != colorPalette.end()) ? result->second : Color(CustomColor::white);
};

Color Picross::GetExpectedColor(const uint row, const uint col) const
{
	ColorPalette::const_iterator result = colorPalette.find(GetCase(row, col).expectedColorIdx);
	return (result != colorPalette.end()) ? result->second : Color(CustomColor::white);
}

Color Picross::GetCurrentColor(const uint row, const uint col) const
{
	ColorPalette::const_iterator result = colorPalette.find(GetCase(row, col).currentColorIdx);
	return (result != colorPalette.end()) ? result->second : Color(CustomColor::white);
}

bool Picross::IsColorNecessary(const uchar idxColor) const
{
	for (uint row = 0; row < height; ++row)
	{
		for (uint col = 0; col < width; ++col)
		{
			if (GetExpectedState(row, col) == State::checked && GetExpectedColorIdx(row, col) == idxColor)
				return true;
		}
	}

	return false;
}

void Picross::GenerateClues()
{
	auto FillClueLine = [&](const LineOrientation dir, const uchar idx)
	{
		Clue currentClue;
		State lastState = State::unchecked;
		Color lastColor = (dir == LineOrientation::row) ? GetExpectedColor(idx, 0) : GetExpectedColor(0, idx);;
		uint lineLimit = (dir == LineOrientation::row) ? height : width;
		uint insertIdx = (dir == LineOrientation::row) ? idx : height + idx;

		for (uint i = 0; i < lineLimit; ++i)
		{
			State expectedState = (dir == LineOrientation::row) ? GetExpectedState(idx, i) : GetExpectedState(i, idx);
			Color expectedColor = (dir == LineOrientation::row) ? GetExpectedColor(idx, i) : GetExpectedColor(i, idx);

			if ((expectedState == State::unchecked && lastState == State::checked) || (expectedState == State::checked && lastState == State::checked && lastColor != expectedColor))	// Current Clue has ended
			{
				currentClue.endIdx = i - 1;
				clues[insertIdx].push_back(currentClue);
			}
			if (expectedState == State::checked && (lastState == State::unchecked || lastColor != expectedColor))	// Starting new Clue
			{
				currentClue.startIdx = i;
				currentClue.color = expectedColor;
			}
			if (expectedState == State::checked && i == lineLimit - 1)	// Current Clue is reaching the line end
			{
				currentClue.endIdx = i;
				clues[insertIdx].push_back(currentClue);
			}

			lastState = expectedState;
			lastColor = expectedColor;
		}

		// Still insert one default clue if none was generated
		if (clues[insertIdx].size() == 0)
			clues[insertIdx].push_back(currentClue);
	};

	clues.resize(width + height);

	// Rows
	for (uint row = 0; row < width; ++row)
	{
		FillClueLine(LineOrientation::row, row);
	}

	// Columns
	for (uint col = 0; col < height; ++col)
	{
		FillClueLine(LineOrientation::column, col);
	}
}

ClueLine Picross::GetClueLine(const LineOrientation dir, const uint idx) const
{
	return dir == LineOrientation::row ? clues[idx] : clues[width + idx];
}

QLabel * Picross::GenerateClueLabel(const LineOrientation dir, const int idx)
{
	const ClueLine clueLine = GetClueLine(dir, idx);

	QString str = "";
	const QString separator = (dir == LineOrientation::row) ? " " : "<br>";

	for (uint i = 0; i < clueLine.size(); ++i)
	{
		const QString clueColor = isColored ? clueLine[i].color.ToHex() : defaultCluesLabelColor.ToHex();
		int clueValue = std::max(clueLine[i].Length(), 0);	// We handle empty clues
		str += QString("<font color='%1'>").arg(clueColor) + QString::number(clueValue) + "</font>";
		if(i < clueLine.size() - 1)
			str += separator;
	}

	// Generate the label
	QLabel* clueLabel = new QLabel(str);
	clueLabel->setStyleSheet("font: bold 15px");
	return clueLabel;
}

bool Picross::IsCorrect()
{
	for (uint row = 0; row < height; ++row)
	{
		for (uint col = 0; col < width; ++col)
		{
			if (GetExpectedState(row, col) == State::checked)
			{
				if (GetCurrentState(row, col) != State::checked)
					return false;

				if(isColored && GetCurrentColor(row, col) != GetExpectedColor(row, col))
					return false;
			}
			else
			{
				if (GetCurrentState(row, col) == State::checked)
					return false;
			}
		}
	}

	return true;
}