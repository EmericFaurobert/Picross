#include "Picross.h"


std::string TruncatePicrossFileName(const std::string& str) {
	size_t startOffset = str.find(pixsFolder) + pixsFolder.size();
	return str.substr(startOffset, str.size() - startOffset - pixsExtension.size());
}


Picross::Picross(const std::string &filePath, const std::string &score)
{
	// Picross path & best score
	pixFilePath = filePath;
	bestScore = score;

	// Starting .pix file read...
	FileStream file(pixFilePath);
	std::string strDump = "";	// For consuming attributes with no values

	// Width & Height
	std::string widthVal = "", heightVal = "";
	file.parseValue("Width", widthVal);
	file.parseValue("Height", heightVal);

	if (widthVal != "" && heightVal != "")
	{
		width = std::min(std::stoi(widthVal), UCHAR_MAX);
		height = std::min(std::stoi(heightVal), UCHAR_MAX);

		assert(("Picross grid size is too small", width > 1 && height > 1));		// Only handling larger than 1x1 grids
		assert(("Picross grid size is too large", width <= 25 && height <= 25));	// But not grids which exceed 25x25 !

		grid.resize(size_t(width) * size_t(height));
	}
	else
	{
		assert(("No grid size defined !", false));	// No grid size read
	}

	// Multi color grid
	std::string coloredVal = "";
	file.parseValue("Colored", coloredVal);
	if (coloredVal == "true" || coloredVal == "yes" || coloredVal == "y")
	{
		isColored = true;
	}

	// ColorPalette
	std::string nbColorsVal = "";
	file.parseValue("ColorPalette", nbColorsVal);
	if (nbColorsVal != "")
	{
		int nbColors = std::min(std::stoi(nbColorsVal), UCHAR_MAX);
		assert(("Too much colors defined", nbColors <= 12));	// ColorPalette is too large for the GUI

		for (int i = 0; i < nbColors; ++i)
		{
			std::string paletteLine = "";
			if (file.readNextLine(paletteLine))
			{
				std::stringstream sstream(paletteLine);
				uint idx = 0, red = 0, green = 0, blue = 0;

				sstream >> idx >> strDump >> red >> green >> blue;
				colorPalette[idx] = Color(red, green, blue);
			}
		}
	}

	// States grid
	if (file.parseValue("StateGrid", strDump))
	{
		std::string statesLine = "";

		for (uint row = 0; row < height; ++row)
		{
			if (file.readNextLine(statesLine) && statesLine != "")
			{
				for (uint col = 0; col < width; ++col)
				{
					if (statesLine.size() >= width)
					{
						if (statesLine[col] == 'X' || statesLine[col] == 'x')
							SetExpectedState(row, col, State::checked);
					}
					else
						assert(("Missing columns state !", false));	// Not enough columns states read
				}
			}
			else
				assert(("Missing rows state !", false));	// Not enough rows states read
		}
	}
	else
		assert(("No grid states found !", false));	// No grid states read

	// Color grid
	if (file.parseValue("ColorGrid", strDump))
	{
		std::string colorsLine = "";

		for (uint row = 0; row < height; ++row)
		{
			if (file.readNextLine(colorsLine))
			{
				std::stringstream sstream(colorsLine);

				for (uint col = 0; col < width; ++col)
				{
					uint colorIdx = 0;
					sstream >> colorIdx;
					SetExpectedColor(row, col, colorIdx);
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
	auto FillClueLine = [&](const LineOrientation dir, const uint idx)
	{
		Clue currentClue;
		State lastState = State::unchecked;
		Color lastColor = (dir == LineOrientation::row) ? GetExpectedColor(idx, 0) : GetExpectedColor(0, idx);;
		uint lineLimit = (dir == LineOrientation::row) ? width : height;
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

	clues.resize(size_t(width) + size_t(height));

	// Fill rows clues
	for (uint row = 0; row < height; ++row)
	{
		FillClueLine(LineOrientation::row, row);
	}

	// Fill columns clues
	for (uint col = 0; col < width; ++col)
	{
		FillClueLine(LineOrientation::column, col);
	}
}

ClueLine Picross::GetClueLine(const LineOrientation dir, const uint idx) const
{
	return dir == LineOrientation::row ? clues[idx] : clues[size_t(height) + size_t(idx)];
}

QLabel *Picross::GenerateClueLabel(const LineOrientation dir, const int idx)
{
	const ClueLine clueLine = GetClueLine(dir, idx);

	QString str = "";
	const QString separator = (dir == LineOrientation::row) ? " " : "<br>";	// Adapting string separator to row/column

	for (uint i = 0; i < clueLine.size(); ++i)
	{
		// Genrate the corresponding string
		const QString clueColor = isColored ? clueLine[i].color.ToHex() : defaultCluesLabelColor.ToHex();
		int clueValue = std::max(clueLine[i].Length(), 0);	// To handle empty clues
		str += QString("<font color='%1'>").arg(clueColor) + QString::number(clueValue) + "</font>";

		// Only add seperator if not the last clue
		if (i < clueLine.size() - 1)
			str += separator;
	}

	// Generate the QLabel
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