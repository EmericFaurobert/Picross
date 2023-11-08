#include "MainMenu.h"
#include "FileReadWrite.h"
#include "Constants.h"

#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>

MainMenu::MainMenu()
{
	setFixedSize(300, 150);

	// Buttons
	playButton = new QPushButton("Jouer");
	loadButton = new QPushButton("Charger");
	editButton = new QPushButton("Creer");
	rulesButton = new QPushButton("Tuto");

	// Layouts
	mainLayout = new QVBoxLayout();
	mainLayout->addWidget(playButton);
	mainLayout->addWidget(loadButton);
	mainLayout->addWidget(editButton);
	mainLayout->addWidget(rulesButton);

	setLayout(mainLayout);

	// Signals / Slots
	QObject::connect(playButton, &QPushButton::clicked, this, &MainMenu::OnPlay);
	QObject::connect(loadButton, &QPushButton::clicked, this, &MainMenu::OnLoad);
	QObject::connect(editButton, &QPushButton::clicked, this, &MainMenu::OnEdit);
	QObject::connect(rulesButton, &QPushButton::clicked, this, &MainMenu::OnRules);
}

void MainMenu::OnPlay()
{
	this->close();

	GridPreSelectWindow* gridSelector = new GridPreSelectWindow();
	gridSelector->show();
}

void MainMenu::OnLoad() 
{
	const QString fileName = QFileDialog::getOpenFileName(this, "Open Picross", QString::fromStdString(pixsFolder), "Picross File (*.pix)");
	if (!fileName.isEmpty())
	{
		std::string currentScore = "";
		const std::string truncedFileName = TruncatePicrossFileName(fileName.toStdString());

		FileStream scoresStream(pixsFolder + scoresFileName, std::fstream::in);
		scoresStream.parseValue(truncedFileName, currentScore);

		this->close();

		PlayWindow* grid = new PlayWindow(Picross(fileName.toStdString(), currentScore));
		grid->show();
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