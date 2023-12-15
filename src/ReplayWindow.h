#pragma once

#include <QMainWindow>
#include <QLayout>
#include <QLabel>
#include <QPushButton>

class ReplayWindow : public QMainWindow
{
public:
	ReplayWindow(QWidget* parent = nullptr, const QString &score = "", bool newBestScore = false);

private:
	void OnReplay();
	void OnMenu();

private:
	QWidget *centralWidget { nullptr };
	QVBoxLayout *mainLayout { nullptr };
	QHBoxLayout *buttonsLayout { nullptr };

	QLabel *congratsLabel { nullptr };
	QPushButton *replayButton { nullptr };
	QPushButton *menuButton { nullptr };
};