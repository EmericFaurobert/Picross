#pragma once

#include "PlayWindow.h"
#include "GridPreSelectWindow.h"

class MainMenuWindow : public QMainWindow
{
public:
	MainMenuWindow(QWidget *parent = nullptr);

private:
	void OnPlay();
	void OnLoad();
	void OnEdit();
	void OnRules();

private:
	QWidget *centralWidget { nullptr };
	QVBoxLayout *mainLayout { nullptr };

	QPushButton *playButton { nullptr };
	QPushButton *loadButton { nullptr };
	QPushButton *editButton { nullptr };
	QPushButton *rulesButton { nullptr };
};