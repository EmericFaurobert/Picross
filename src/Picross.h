#pragma once

#include <vector>
#include <QLabel>
#include "Color.h"
#include "QRightClickableButton.h"
#include "Constants.h"

using ColorPalette = std::map<uchar, Color>;

enum class LineOrientation : char { row, column };
enum class State : char { unknow, unchecked, checked };

struct Case 
{ 
	State currentState = State::unknow; 
	State expectedState = State::unchecked; 
	uchar currentColorIdx = 0;
	uchar expectedColorIdx = 0; 
	QRightClickableButton *button { nullptr };
};

struct Clue
{
	Color color = defaultCluesLabelColor;
	int startIdx = 1;
	int endIdx = -1;

	int Length() const { return endIdx - startIdx + 1; }
};

using ClueLine = std::vector<Clue>;

class Picross
{
public:
	Picross(const std::string & fileName);

	uint GetWidth() const { return width; }
	uint GetHeight() const { return height; }
	bool IsColored() const { return isColored; }

	const ColorPalette & GetColorPalette() const { return colorPalette; }
	Color GetColor(uchar colorIdx) const;
	Color GetExpectedColor(const uint row, const uint col) const;
	Color GetCurrentColor(const uint row, const uint col) const;
	uchar GetExpectedColorIdx(const uint row, const uint col) const { return GetCase(row, col).expectedColorIdx; };
	uchar GetCurrentColorIdx(const uint row, const uint col) const { return GetCase(row, col).currentColorIdx; };
	bool IsColorNecessary(const uchar idxColor) const;

	QRightClickableButton* GetBtn(const int row, const int col) { return GetCase(row, col).button; }

	void SetExpectedState(const uint row, const uint col, const State exp) { GetCase(row, col).expectedState = exp; }
	void SetCurrentState(const uint row, const uint col, const State sol) { GetCase(row, col).currentState = sol; }
	void SetExpectedColor(const uint row, const uint col, const uchar colorIdx) { GetCase(row, col).expectedColorIdx = colorIdx; }
	void SetCurrentColor(const uint row, const uint col, const uchar colorIdx) { GetCase(row, col).currentColorIdx = colorIdx; }
	void SetBtn(const uint row, const uint col, QRightClickableButton *btn) { GetCase(row, col).button = btn; }

	QLabel* GenerateClueLabel(const LineOrientation dir, const int idx);
	bool IsCorrect();

private:
	uchar LastRow() const { return height - 1; }
	uchar LastCol() const { return width - 1; }

	uint GetIndex(const uint row, const uint col) const { return row * width + col; }
	Case & GetCase(const uint row, const uint col) { return grid[GetIndex(row, col)]; }
	const Case & GetCase(const uint row, const uint col) const { return grid[GetIndex(row, col)]; }

	State GetExpectedState(const uint row, const uint col) const { return GetCase(row, col).expectedState; }
	State GetCurrentState(const uint row, const uint col) const { return GetCase(row, col).currentState; }
	
	void GenerateClues();
	ClueLine GetClueLine(const LineOrientation dir, const uint idx) const;

private:
	uint width { 5 };
	uint height { 5 };
	bool isColored { false };
	ColorPalette colorPalette = { { 0, CustomColor::black } };
	std::vector<Case> grid { width * height };
	std::vector<ClueLine> clues { width + height };
};

