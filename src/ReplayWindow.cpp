#include "ReplayWindow.h"
#include "MainMenuWindow.h"

ReplayWindow::ReplayWindow(QWidget* parent, const QString &score, bool newBestScore) : QMainWindow(parent)
{
	// Allocations
	centralWidget = new QWidget(this);
	mainLayout = new QVBoxLayout(centralWidget);
	buttonsLayout = new QHBoxLayout(centralWidget);

	// Buttons
	replayButton = new QPushButton("Rejouer");
	menuButton = new QPushButton("Menu Principal");

	// Label
	if(newBestScore)
		congratsLabel = new QLabel(QString("Bravo, vous avez battu votre record en %1 !").arg(score));
	else
		congratsLabel = new QLabel(QString("Vous avez termine en %1, felicitaions !").arg(score));

	congratsLabel->setAlignment(Qt::AlignHCenter);
	congratsLabel->setStyleSheet("font: 20px bold");
	congratsLabel->setWordWrap(true);
	mainLayout->addWidget(congratsLabel);

	// Layouts
	mainLayout->addLayout(buttonsLayout);
	buttonsLayout->addWidget(replayButton);
	buttonsLayout->addWidget(menuButton);

	// Central Widget
	centralWidget->setLayout(mainLayout);
	setCentralWidget(centralWidget);

	// General Poperties
	setWindowFlags(this->windowFlags() & ~Qt::WindowCloseButtonHint);
	setAttribute(Qt::WA_DeleteOnClose, true);
	setFixedSize(300, 100);

	// Signals / Slots
	QObject::connect(replayButton, &QPushButton::clicked, this, &ReplayWindow::OnReplay);
	QObject::connect(menuButton, &QPushButton::clicked, this, &ReplayWindow::OnMenu);
}

void ReplayWindow::OnReplay()
{
	PlayWindow* pParent = (PlayWindow*) this->parentWidget();
	pParent->Reset();

	this->close();
}

void ReplayWindow::OnMenu()
{
	MainMenuWindow *mainMenu = new MainMenuWindow();
	mainMenu->show();

	parentWidget()->close();
}