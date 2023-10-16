#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>

class GridPreSelectWindow : public QWidget
{
public:
	GridPreSelectWindow();

private:
	void OnPreLoad(const QString& selectedDifficulty);

	QVBoxLayout* buttonsLayout { nullptr };
	QPushButton* easyGridsButton { nullptr };
	QPushButton* mediumGridsButton { nullptr };
	QPushButton* hardGridsButton { nullptr };
	QPushButton* veryHardGridsButton { nullptr };
};