#include <QMessageBox>
#include "PlayWindow.h"
#include "ReplayWindow.h"

PlayWindow::PlayWindow(QWidget *parent, const Picross & picx) : QMainWindow(parent), picross(picx)
{
	// Allocations
	centralWidget = new QWidget(this);
	mainLayout = new QHBoxLayout(centralWidget);
	leftLayout = new QVBoxLayout(centralWidget);
	gridLayout = new QGridLayout(centralWidget);
	rightLayout = new QVBoxLayout(centralWidget);

	// Initialize all the layouts
	InitLayouts();

	// Central Widget
	centralWidget->setLayout(mainLayout);
	setCentralWidget(centralWidget);

	// Initialize the grid section
	InitGrid();
	InitClues();

	// Initialize the color picker
	InitColorPicker();

	// Initialize the chrono
	InitChrono();

	// General Poperties
	setAttribute(Qt::WA_DeleteOnClose, true);
}

void PlayWindow::Reset()
{
	// Reset the Picross current states
	picross.ResetCurrentStates();

	// Reset the colorPalette index
	if (picross.IsMultiColored())
		ChangeCurrentColor(initColorIdx);
	else
		selectedColorIdx = initColorIdx;

	// Reset the chrono & timer
	chronoLabel->setText("00:00:00");
	chrono = QTime(0, 0, 0);
	StartStopChrono();

	// Reset grid default background color
	for (uint row = 0; row < picross.GetHeight(); ++row)
	{
		for (uint col = 0; col < picross.GetWidth(); ++col)
		{
			SetBtnStyle(row, col, defaultGridCaseColor);
		}
	}
}

void PlayWindow::InitLayouts()
{
	// The left chrono layout
	leftLayout->setAlignment(Qt::AlignCenter);
	leftLayout->setContentsMargins(10, 0, 30, 0);

	// The middle picross layout
	gridLayout->setSpacing(0);
	gridWidget = new QWidget(this);
	gridWidget->setLayout(gridLayout);
	QSizePolicy spRetain = gridWidget->sizePolicy();
	spRetain.setRetainSizeWhenHidden(true);
	gridWidget->setSizePolicy(spRetain);

	// The right color picker layout
	rightLayout->setAlignment(Qt::AlignCenter);
	rightLayout->setContentsMargins(30, 0, 10, 0);
	rightLayout->setSpacing(10);

	// The main layout (which contains all others)
	mainLayout->setAlignment(Qt::AlignCenter);
	mainLayout->addLayout(leftLayout);
	mainLayout->addWidget(gridWidget);
	mainLayout->addLayout(rightLayout);
}

void PlayWindow::InitGrid()
{
	for (uint row = 0; row < picross.GetHeight(); ++row)
	{
		for (uint col = 0; col < picross.GetWidth(); ++col)
		{
			QRightClickableButton *btn = new QRightClickableButton();
			btn->setFixedSize(QSize(30, 30));
			picross.SetBtn(row, col, btn);
			SetBtnStyle(row, col, defaultGridCaseColor);
			gridLayout->addWidget(btn, row + 1, col + 1);
			QObject::connect(btn, &QRightClickableButton::leftClicked, [this, col, row] { ClickOnCase(row, col, Qt::LeftButton); });
			QObject::connect(btn, &QRightClickableButton::rightClicked, [this, col, row] { ClickOnCase(row, col, Qt::RightButton); });
		}
	}
}

void PlayWindow::InitClues()
{
	// Rows
	for (uint row = 0; row < picross.GetHeight(); ++row)
	{
		QLabel *label = picross.GenerateClueLabel(LineOrientation::row, row);
		label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
		label->setContentsMargins(0, 0, 5, 0);
		gridLayout->addWidget(label, row + 1, 0);
	}

	// Columns
	for (uint col = 0; col < picross.GetWidth(); ++col)
	{
		QLabel *label = picross.GenerateClueLabel(LineOrientation::column, col);
		label->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
		label->setContentsMargins(0, 0, 0, 5);
		gridLayout->addWidget(label, 0, col + 1);
	}
}

void PlayWindow::InitColorPicker()
{
	ColorPalette colPal = picross.GetColorPalette();

	for (ColorPalette::const_iterator it = colPal.begin(); it != colPal.end(); ++it)
	{
		uchar colorIdx = it->first;

		// Check if color is used for grid completion (or only for display)
		if (picross.IsColorNecessary(colorIdx))
		{
			// Set up the initial selected color
			if (initColorIdx == -1)
			{
				initColorIdx = colorIdx;
				selectedColorIdx = colorIdx;
			}

			// Creation of the color picker GUI
			if (picross.IsMultiColored())
			{
				QPushButton* btn = new QPushButton();
				colorPicker[colorIdx] = btn;
				rightLayout->addWidget(btn);

				btn->setFixedSize(QSize(40, 40));
				btn->setStyleSheet(QString("background-color: %1;").arg(it->second.ToRgb()));
				QObject::connect(btn, &QPushButton::clicked, [this, colorIdx] { ChangeCurrentColor(colorIdx); });
			}
		}
	}

	// Selected color GUI highlight
	if (picross.IsMultiColored())
	{
		colorPicker[selectedColorIdx]->setStyleSheet(colorPicker[selectedColorIdx]->styleSheet() + QString("border: 3px solid %1;").arg(caseColorPickerBorder.ToHex()));
	}
}

void PlayWindow::InitChrono()
{
	chronoLabel = new QLabel("00:00:00");
	chronoLabel->setAlignment(Qt::AlignHCenter);
	chronoLabel->setStyleSheet("font: bold 30px");
	leftLayout->addWidget(chronoLabel);

	pauseButton = new QPushButton("Pause");
	leftLayout->addWidget(pauseButton);

	timer.start(1000);

	QObject::connect(&timer, &QTimer::timeout, [this] { UpdateChrono(); });
	QObject::connect(pauseButton, &QPushButton::clicked, [this] { StartStopChrono(); });
}

void PlayWindow::UpdateChrono(const uint sec)
{
	if (chrono.isValid())
	{
		chrono = chrono.addSecs(sec);
		chronoLabel->setText(chrono.toString("HH:mm:ss"));
	}
}

void PlayWindow::StartStopChrono()
{
	if (timer.isActive())
	{
		timer.stop();
		pauseButton->setText("Reprendre");
		gridWidget->setVisible(false);
	}
	else
	{
		timer.start();
		pauseButton->setText("Pause");
		gridWidget->setVisible(true);
	}
}

void PlayWindow::SetBtnStyle(const uint row, const uint col, const Color &color)
{
	QRightClickableButton *btn = picross.GetBtn(row, col);
	btn->setIcon(QIcon());		// Clean cross icons

	// Thicken borders every 5 rows/colums
	uint hThickness = (row != 0 && row % 5 == 0) ? 2 : 1;
	uint vThickness = (col != 0 && col % 5 == 0) ? 2 : 1;

	QString strBorderColor = Color(128, 128, 128).ToRgb();

	// Common properties (background color + top & left borders)
	QString css = QString("background-color: %1;").arg(color.ToRgb()) +
		QString("border-top: %1px solid %2;").arg(hThickness).arg(strBorderColor) +
		QString("border-left: %1px solid %2;").arg(vThickness).arg(strBorderColor);

	if (row == picross.GetHeight() - 1)	// Last row (adding bottom border)
	{
		css += QString("border-bottom: 1px solid %1;").arg(strBorderColor);
	}
	if (col == picross.GetWidth() - 1)	// Last column (adding right border)
	{
		css += QString("border-right: 1px solid %1;").arg(strBorderColor);
	}

	btn->setStyleSheet(css);
}

void PlayWindow::SetBtnBackground(const uint row, const uint col, const QString & imgPath)
{
	QRightClickableButton *btn = picross.GetBtn(row, col);
	QPixmap pixmap(imgPath);
	QIcon ButtonIcon(pixmap);
	btn->setIcon(ButtonIcon);
	btn->setIconSize(QSize(15, 15));
}

void PlayWindow::ClickOnCase(const uint row, const uint col, const Qt::MouseButton click)
{
	if (click == Qt::LeftButton)		// Case colorization
	{
		picross.SetCurrentState(row, col, State::checked);
		picross.SetCurrentColor(row, col, selectedColorIdx);
		SetBtnStyle(row, col, picross.GetColor(selectedColorIdx));
	}
	else if (click == Qt::RightButton)	// Mark as empty case
	{
		picross.SetCurrentState(row, col, State::unchecked);
		SetBtnStyle(row, col, defaultGridCaseColor);
		SetBtnBackground(row, col, "resources/Icons/BlackCross.svg");
	}

	if (picross.IsCorrect())	// Check if corresponds to solution
	{
		// Stop the chrono
		timer.stop();

		// Writing best score...
		const std::wstring  scoresFilePath = pixsFolder + scoresFileName;
		FileStream scoresStream(scoresFilePath, std::fstream::in | std::fstream::out | std::fstream::app | std::fstream::binary);

		std::wstring oldScore = picross.GetBestScore();
		QTime oldTime = QTime::fromString(QString::fromStdWString(oldScore), "HH:mm:ss");

		std::wstring newScore = chrono.toString("HH:mm:ss").toStdWString();
		bool bestScoreBeaten = false;

		// If no already existing score...
		if (!oldTime.isValid())
		{
			picross.SetBestScore(newScore);
			scoresStream.writeContent(picross.GetFileName() + L": " + newScore + L"\n");
		}
		else if (chrono < oldTime)	// Otherwise compare if new one is better	
		{
			bestScoreBeaten = true;
			picross.SetBestScore(newScore);

			// Replace and rewrite all file content
			std::wstring fileContent = L"";

			if (scoresStream.readAllContent(fileContent))
			{
				std::wstring oldStrScore = picross.GetFileName() + L": " + oldScore;
				std::wstring newStrScore = picross.GetFileName() + L": " + newScore;

				fileContent.replace(fileContent.find(oldStrScore), oldStrScore.length(), newStrScore);

				FileStream newStream(scoresFilePath, std::fstream::out | std::fstream::trunc | std::fstream::binary);
				newStream.writeContent(fileContent);
			}
		}

		// Show solution (including background)
		ShowSolution();

		// Open the replay window
		ReplayWindow *replayWindow = new ReplayWindow(this, QString::fromStdWString(newScore), bestScoreBeaten);
		replayWindow->setWindowModality(Qt::WindowModal);
		replayWindow->show();
	}	
}

void PlayWindow::ChangeCurrentColor(const uchar newColorIdx)
{
	QColor oldColor = colorPicker[selectedColorIdx]->palette().button().color();
	colorPicker[selectedColorIdx]->setStyleSheet(QString("background-color: %1;").arg(oldColor.name()));

	colorPicker[newColorIdx]->setStyleSheet(colorPicker[newColorIdx]->styleSheet() + QString("border: 3px solid %1;").arg(caseColorPickerBorder.ToHex()));

	selectedColorIdx = newColorIdx;
}

void PlayWindow::ShowSolution()
{
	for (uint row = 0; row < picross.GetHeight(); ++row)
	{
		for (uint col = 0; col < picross.GetWidth(); ++col)
		{
			SetBtnStyle(row, col, picross.GetExpectedColor(row, col));
		}
	}
}

