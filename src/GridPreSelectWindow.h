#pragma once

#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>

class GridPreSelectWindow : public QMainWindow
{
public:
	GridPreSelectWindow(QWidget *parent = nullptr);

private:
	void OnPreLoad(const QString& selectedDifficulty);

private:
	QWidget* centralWidget{ nullptr };
	QVBoxLayout* buttonsLayout { nullptr };

	QPushButton* easyGridsButton { nullptr };
	QPushButton* mediumGridsButton { nullptr };
	QPushButton* hardGridsButton { nullptr };
	QPushButton* veryHardGridsButton { nullptr };
};