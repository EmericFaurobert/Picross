#pragma once

#include <QWidget>
#include <QGridLayout>
#include <QTime>
#include <QTimer>
#include "Picross.h"

class PlayWindow : public QWidget
{
public:
	PlayWindow(const Picross &picross);

private:
	void InitLayouts();
	void InitGrid();
	void InitClues();
	void InitColorPicker();
	void InitChrono();

	void UpdateChrono(const uint sec = 1);
	void StartStopChrono();

	void SetBtnStyle(const uint row, const uint col, const Color & color);
	void SetBtnBackground(const uint row, const uint col, const QString & imgPath);

	void ClickOnCase(const uint row, const uint col, const Qt::MouseButton click);
	void ChangeCurrentColor(const uchar newColor);
	void ShowSolution();

private:
	Picross picross;
	
	QHBoxLayout *mainLayout { nullptr };
	QVBoxLayout *leftLayout { nullptr };
	QWidget *centralWidget { nullptr };
	QGridLayout *gridLayout { nullptr };
	QVBoxLayout *rightLayout { nullptr };
	
	uchar selectedColorIdx = 0;
	std::map<uchar, QPushButton*> colorPicker;

	QTime *chrono { nullptr };
	QTimer *timer { nullptr };
	QLabel *chronoLabel { nullptr };
	QPushButton *pauseButton { nullptr };
};