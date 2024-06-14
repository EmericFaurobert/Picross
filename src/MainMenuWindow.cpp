#include <QFileDialog>
#include <QMessageBox>
#include "MainMenuWindow.h"

MainMenuWindow::MainMenuWindow(QWidget *parent) : QMainWindow(parent)
{
	// Allocations
	centralWidget = new QWidget(this);
	mainLayout = new QVBoxLayout(centralWidget);

	// Buttons
	playButton = new QPushButton("Jouer");
	playButton->setMaximumHeight(40);

	loadButton = new QPushButton("Charger");
	loadButton->setMaximumHeight(25);

	editButton = new QPushButton("Creer");
	editButton->setMaximumHeight(25);

	rulesButton = new QPushButton("Tuto");
	rulesButton->setMaximumHeight(25);

	// Layouts
	mainLayout->addWidget(playButton);
	mainLayout->addWidget(loadButton);
	mainLayout->addWidget(editButton);
	mainLayout->addWidget(rulesButton);

	// Central Widget
	centralWidget->setLayout(mainLayout);
	setCentralWidget(centralWidget);

	// General Poperties
	setAttribute(Qt::WA_DeleteOnClose, true);
	setFixedSize(300, 180);

	// Signals / Slots
	QObject::connect(playButton, &QPushButton::clicked, this, &MainMenuWindow::OnPlay);
	QObject::connect(loadButton, &QPushButton::clicked, this, &MainMenuWindow::OnLoad);
	QObject::connect(editButton, &QPushButton::clicked, this, &MainMenuWindow::OnEdit);
	QObject::connect(rulesButton, &QPushButton::clicked, this, &MainMenuWindow::OnRules);
}

void MainMenuWindow::OnPlay()
{
	GridPreSelectWindow *gridSelector = new GridPreSelectWindow(this);
	gridSelector->setWindowModality(Qt::WindowModal);
	gridSelector->move(this->x() + 10, this->y() + 10);
	gridSelector->show();
}

void MainMenuWindow::OnLoad()
{
	// QFileDialog seems to be caching stuff, so it's increasing a lot the RAM consumtpion while called...
	const QString fileName = QFileDialog::getOpenFileName(this, "Open Picross", QString::fromStdWString(pixsFolder), "Picross File (*.pix)");

	if (!fileName.isEmpty())
	{
		std::wstring currentScore = L"";
		const std::wstring truncedFileName = TruncatePicrossFileName(fileName.toStdWString());

		FileStream scoresStream(pixsFolder + scoresFileName, std::fstream::in);
		scoresStream.parseValue(truncedFileName, currentScore);

		PlayWindow* grid = new PlayWindow(nullptr, Picross(fileName.toStdWString(), currentScore));
		grid->show();

		this->close();
	}
}

void MainMenuWindow::OnEdit()
{
	QMessageBox::information(this, "Not yet implemented", "Fonctionnalite en cours de developpement...");
	// TO DO...
}

void MainMenuWindow::OnRules()
{
	QMessageBox::information(this, "Not yet implemented", "Fonctionnalite en cours de developpement...");
	// TO DO...
}