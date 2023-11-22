#pragma once

#include <QMainWindow>
#include <QGridLayout>
#include <QTime>
#include <QTimer>
#include "Picross.h"

class PlayWindow : public QMainWindow
{
public:
	PlayWindow(QWidget *parent = nullptr, const Picross &picross = Picross());

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
	uchar selectedColorIdx = 0;
	std::map<uchar, QPushButton*> colorPicker;

	QWidget *centralWidget { nullptr };
	QWidget *gridWidget { nullptr };
	QHBoxLayout *mainLayout { nullptr };
	QVBoxLayout *leftLayout { nullptr };
	QGridLayout *gridLayout { nullptr };
	QVBoxLayout *rightLayout { nullptr };
	
	QTime chrono = QTime(0, 0, 0);
	QTimer timer = QTimer(this);
	QLabel *chronoLabel { nullptr };
	QPushButton *pauseButton { nullptr };
};