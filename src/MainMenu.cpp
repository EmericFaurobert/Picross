#include <QFileDialog>
#include <QMessageBox>
#include "MainMenu.h"

MainMenu::MainMenu(QWidget *parent) : QMainWindow(parent)
{
	// Allocations
	centralWidget = new QWidget(this);
	mainLayout = new QVBoxLayout(centralWidget);

	// Buttons
	playButton = new QPushButton("Jouer");
	loadButton = new QPushButton("Charger");
	editButton = new QPushButton("Creer");
	rulesButton = new QPushButton("Tuto");

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
	setFixedSize(300, 150);

	// Signals / Slots
	QObject::connect(playButton, &QPushButton::clicked, this, &MainMenu::OnPlay);
	QObject::connect(loadButton, &QPushButton::clicked, this, &MainMenu::OnLoad);
	QObject::connect(editButton, &QPushButton::clicked, this, &MainMenu::OnEdit);
	QObject::connect(rulesButton, &QPushButton::clicked, this, &MainMenu::OnRules);
}

void MainMenu::OnPlay()
{
	GridPreSelectWindow *gridSelector = new GridPreSelectWindow(this);
	gridSelector->setWindowModality(Qt::WindowModal);
	gridSelector->move(this->x() + 10, this->y() + 10);
	gridSelector->show();
}

void MainMenu::OnLoad() 
{
	// QFileDialog seems to be caching stuff, so it's increasing a lot the RAM consumtpion while called...
	const QString fileName = QFileDialog::getOpenFileName(this, "Open Picross", QString::fromStdString(pixsFolder), "Picross File (*.pix)");

	if (!fileName.isEmpty())
	{
		std::string currentScore = "";
		const std::string truncedFileName = TruncatePicrossFileName(fileName.toStdString());

		FileStream scoresStream(pixsFolder + scoresFileName, std::fstream::in);
		scoresStream.parseValue(truncedFileName, currentScore);

		PlayWindow* grid = new PlayWindow(nullptr, Picross(fileName.toStdString(), currentScore));
		grid->show();

		this->close();
	}
}

void MainMenu::OnEdit() 
{
	QMessageBox::information(this, "Not yet implemented", "Fonctionnalite en cours de developpement...");
	// TO DO...
}

void MainMenu::OnRules() 
{
	QMessageBox::information(this, "Not yet implemented", "Fonctionnalite en cours de developpement...");
	// TO DO...
}