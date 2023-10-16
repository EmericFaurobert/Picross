#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include "PlayWindow.h"
#include "GridPreSelectWindow.h"

class MainMenu : public QWidget
{
public:
	MainMenu();

	void OnPlay();
	void OnLoad();
	void OnEdit();
	void OnRules();

private:
	QVBoxLayout *mainLayout { nullptr };

	QPushButton *playButton { nullptr };
	QPushButton *loadButton { nullptr };
	QPushButton *editButton { nullptr };
	QPushButton *rulesButton { nullptr };
};