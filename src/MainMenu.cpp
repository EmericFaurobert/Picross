#include "MainMenu.h"
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
	QString fileName = QFileDialog::getOpenFileName(this, "Open Picross", "resources\\Picross", "Picross File (*.pix)");
	if (!fileName.isEmpty())
	{
		this->close();

		PlayWindow* grid = new PlayWindow(Picross(fileName.toUtf8().constData()));
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