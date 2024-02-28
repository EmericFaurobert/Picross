#pragma once

#include <QLabel>
#include "QRightClickableButton.h"
#include "FileReadWrite.h"
#include "Constants.h"
#include "Color.h"


enum class LineOrientation : char { row, column };
enum class State : char { unknow, unchecked, checked };

// A case of the Picross grid
struct Case 
{ 
	State currentState = State::unknow; 
	State expectedState = State::unchecked; 
	uchar currentColorIdx = 0;
	uchar expectedColorIdx = 0; 
	QRightClickableButton *button { nullptr };
};

// Indicates the start/end/color of a clue
struct Clue
{
	Color color = defaultCluesLabelColor;
	int startIdx = 1;
	int endIdx = -1;

	inline int Length() const { return endIdx - startIdx + 1; }
};

using ClueLine = std::vector<Clue>;				// All the clues for a given row/column
using ColorPalette = std::map<uchar, Color>;	// Collection of colors used for a Picross


// Return a truncated Picross filepath (without Picross folder nor extension)
std::wstring TruncatePicrossFileName(const std::wstring &str);


class Picross
{
public:
	Picross(const std::wstring &filePath = L"", const std::wstring &score = L"");

	inline std::wstring GetFileName() const { return TruncatePicrossFileName(pixFilePath); };
	inline std::wstring GetBestScore() { return bestScore; };

	inline uint GetWidth() const { return width; }				// Return the Picross width
	inline uint GetHeight() const { return height; }			// Return the Picross height
	inline bool IsMultiColored() const { return isMultiColor; }	// Return if Picross uses multiple colors

	inline void SetBestScore(const std::wstring& newScore) { bestScore = newScore; }
	void SetCurrentState(const uint row, const uint col, const State sol) { GetCase(row, col).currentState = sol; }
	void SetCurrentColor(const uint row, const uint col, const uchar colorIdx) { GetCase(row, col).currentColorIdx = colorIdx; }

	void SetBtn(const uint row, const uint col, QRightClickableButton* btn) { GetCase(row, col).button = btn; }
	QRightClickableButton* GetBtn(const int row, const int col) { return GetCase(row, col).button; }

	QLabel* GenerateClueLabel(const LineOrientation dir, const int idx);	

	inline const ColorPalette& GetColorPalette() const { return colorPalette; }
	Color GetColor(uchar colorIdx) const;
	Color GetExpectedColor(const uint row, const uint col) const;
	bool IsColorNecessary(const uchar idxColor) const;

	bool IsCorrect();
	void ResetCurrentStates();

private:
	inline uint LastRow() const { return height - 1; }	// Return last row index
	inline uint LastCol() const { return width - 1; }	// Return last column index

	inline uint GetIndex(const uint row, const uint col) const { return row * width + col; }
	Case& GetCase(const uint row, const uint col) { return grid[GetIndex(row, col)]; }
	const Case& GetCase(const uint row, const uint col) const { return grid[GetIndex(row, col)]; }

	void SetExpectedState(const uint row, const uint col, const State exp) { GetCase(row, col).expectedState = exp; }
	void SetExpectedColor(const uint row, const uint col, const uchar colorIdx) { GetCase(row, col).expectedColorIdx = colorIdx; }
	State GetExpectedState(const uint row, const uint col) const { return GetCase(row, col).expectedState; }
	State GetCurrentState(const uint row, const uint col) const { return GetCase(row, col).currentState; }
	
	void GenerateClues();
	ClueLine GetClueLine(const LineOrientation dir, const uint idx) const;

	Color GetCurrentColor(const uint row, const uint col) const;
	uchar GetExpectedColorIdx(const uint row, const uint col) const { return GetCase(row, col).expectedColorIdx; };
	uchar GetCurrentColorIdx(const uint row, const uint col) const { return GetCase(row, col).currentColorIdx; };

private:
	std::wstring pixFilePath = L"";
	std::wstring bestScore = L"";
	uint width { 5 };
	uint height { 5 };
	bool isMultiColor { false };
	ColorPalette colorPalette = { { 0, CustomColor::black } };
	std::vector<Case> grid { size_t(width) * size_t(height) };
	std::vector<ClueLine> clues { size_t(width) + size_t(height) };
};

