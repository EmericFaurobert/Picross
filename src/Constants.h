#pragma once

#include "Color.h"

// Paths
const std::wstring pixsFolder= L"resources/Picross/";
const std::wstring pixsExtension = L".pix";
const std::wstring scoresFileName = L"scores.txt";

// Colors
const Color defaultGridCaseColor(224, 224, 224);	// Grid background color
const Color caseColorPickerBorder(96, 96, 96);		// Highlight of selected color
const Color defaultCluesLabelColor(0, 0, 0);		// Clue color for uncolored Picrosses