#include "GridPreSelectWindow.h"
#include "GridSelectWindow.h"

GridPreSelectWindow::GridPreSelectWindow(QWidget *parent) : QMainWindow(parent)
{
	// Allocations
	centralWidget = new QWidget(this);
	buttonsLayout = new QVBoxLayout(centralWidget);

	// Buttons
	easyGridsButton = new QPushButton("Easy Grids (5x5) : ~ 5 min");
	mediumGridsButton = new QPushButton("Medium Grids (10x10) : ~ 15 min");
	hardGridsButton = new QPushButton("Hard Grids (15x15) : ~ 30 min");
	veryHardGridsButton = new QPushButton("Very Hard Grids (15x15) : ~ 1 hour");

	// Layouts
	buttonsLayout->addWidget(easyGridsButton);
	buttonsLayout->addWidget(mediumGridsButton);
	buttonsLayout->addWidget(hardGridsButton);
	buttonsLayout->addWidget(veryHardGridsButton);

	// Central Widget
	centralWidget->setLayout(buttonsLayout);
	setCentralWidget(centralWidget);

	// General Poperties
	setAttribute(Qt::WA_DeleteOnClose, true);
	setFixedSize(300, 150);

	// Signals / Slots
	QObject::connect(easyGridsButton, &QPushButton::clicked, this, [this] { GridPreSelectWindow::OnPreLoad("Easy"); });
	QObject::connect(mediumGridsButton, &QPushButton::clicked, this, [this] { GridPreSelectWindow::OnPreLoad("Medium"); });
	QObject::connect(hardGridsButton, &QPushButton::clicked, this, [this] { GridPreSelectWindow::OnPreLoad("Hard"); });
	QObject::connect(veryHardGridsButton, &QPushButton::clicked, this, [this] { GridPreSelectWindow::OnPreLoad("VeryHard"); });
}

void GridPreSelectWindow::OnPreLoad(const QString & selectedDifficulty)
{
	GridSelectWindow *gridSelectWindow = new GridSelectWindow(this, selectedDifficulty);
	gridSelectWindow->setWindowModality(Qt::WindowModal);
	gridSelectWindow->move(this->x() + 10, this->y() + 10);
	gridSelectWindow->show();
}