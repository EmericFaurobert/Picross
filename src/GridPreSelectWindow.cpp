#include "GridPreSelectWindow.h"
#include "GridSelectWindow.h"

GridPreSelectWindow::GridPreSelectWindow()
{
	setFixedSize(300, 150);

	// Buttons
	easyGridsButton = new QPushButton("Easy Grids (5x5) : ~ 5 min");
	mediumGridsButton = new QPushButton("Medium Grids (10x10) : ~ 15 min");
	hardGridsButton = new QPushButton("Hard Grids (15x15) : ~ 30 min");
	veryHardGridsButton = new QPushButton("Very Hard Grids (15x15) : ~ 1 hour");

	// Layouts
	buttonsLayout = new QVBoxLayout();
	buttonsLayout->addWidget(easyGridsButton);
	buttonsLayout->addWidget(mediumGridsButton);
	buttonsLayout->addWidget(hardGridsButton);
	buttonsLayout->addWidget(veryHardGridsButton);

	setLayout(buttonsLayout);

	// Signals / Slots
	QObject::connect(easyGridsButton, &QPushButton::clicked, this, [this] { GridPreSelectWindow::OnPreLoad("Easy"); });
	QObject::connect(mediumGridsButton, &QPushButton::clicked, this, [this] { GridPreSelectWindow::OnPreLoad("Medium"); });
	QObject::connect(hardGridsButton, &QPushButton::clicked, this, [this] { GridPreSelectWindow::OnPreLoad("Hard"); });
	QObject::connect(veryHardGridsButton, &QPushButton::clicked, this, [this] { GridPreSelectWindow::OnPreLoad("VeryHard"); });
}

void GridPreSelectWindow::OnPreLoad(const QString & selectedDifficulty)
{
	this->close();

	GridSelectWindow *gridSelectWindow = new GridSelectWindow(selectedDifficulty);
	gridSelectWindow->show();
}