#include "PlayWindow.h"
#include <QMessageBox>
#include "FileReadWrite.h"
#include "Constants.h"

PlayWindow::PlayWindow(const Picross & picx) : picross(picx)
{
	// Initialize all the layouts
	InitLayouts();

	// Initialize the grid section
	InitGrid();
	InitClues();

	// Initialize the color picker
	InitColorPicker();

	// Initialize the chrono
	InitChrono();
}

void PlayWindow::InitLayouts()
{
	// The left chrono layout
	leftLayout = new QVBoxLayout();
	leftLayout->setAlignment(Qt::AlignCenter);
	leftLayout->setContentsMargins(10, 0, 30, 0);

	// The middle picross layout
	gridLayout = new QGridLayout();
	gridLayout->setSpacing(0);
	centralWidget = new QWidget();
	centralWidget->setLayout(gridLayout);

	QSizePolicy spRetain = centralWidget->sizePolicy();
	spRetain.setRetainSizeWhenHidden(true);
	centralWidget->setSizePolicy(spRetain);

	// The right color picker layout
	rightLayout = new QVBoxLayout();
	rightLayout->setAlignment(Qt::AlignCenter);
	rightLayout->setContentsMargins(30, 0, 10, 0);
	rightLayout->setSpacing(10);

	// The main layout (which contains all others)
	mainLayout = new QHBoxLayout();
	mainLayout->setAlignment(Qt::AlignCenter);
	mainLayout->addLayout(leftLayout);
	mainLayout->addWidget(centralWidget);
	mainLayout->addLayout(rightLayout);

	setLayout(mainLayout);
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
	bool isSelectColorInit = false;

	for (ColorPalette::const_iterator it = colPal.begin(); it != colPal.end(); ++it)
	{
		uchar colorIdx = it->first;

		// Check if color is used for grid completion (or only for display)
		if (picross.IsColorNecessary(colorIdx))
		{
			// Set up the initial selected color
			if (!isSelectColorInit)
			{
				selectedColorIdx = colorIdx;
				isSelectColorInit = true;
			}

			// Creation of the color picker GUI
			if (picross.IsColored())
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
	if (picross.IsColored())
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

	chrono = new QTime(0, 0, 0);
	timer = new QTimer();
	timer->start(1000);

	QObject::connect(timer, &QTimer::timeout, [this] { UpdateChrono(); });
	QObject::connect(pauseButton, &QPushButton::clicked, [this] { StartStopChrono(); });
}

void PlayWindow::UpdateChrono(const uint sec)
{
	*chrono = chrono->addSecs(sec);
	chronoLabel->setText(chrono->toString("HH:mm:ss"));
}

void PlayWindow::StartStopChrono()
{
	if (timer->isActive())
	{
		timer->stop();
		pauseButton->setText("Reprendre");
		centralWidget->setVisible(false);
	}
	else
	{
		timer->start();
		pauseButton->setText("Pause");
		centralWidget->setVisible(true);
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
		timer->stop();
		pauseButton->setDisabled(true);

		// Writing best score...
		const std::string  scoresFilePath = pixsFolder + scoresFileName;
		FileStream scoresStream(scoresFilePath, std::fstream::in | std::fstream::out | std::fstream::app | std::fstream::binary);

		std::string oldScore = picross.GetBestScore();
		QTime oldTime = QTime::fromString(QString::fromStdString(oldScore), "HH:mm:ss");

		if (!oldTime.isValid())
		{
			// Append new score at eof if not already existing
			scoresStream.writeContent(picross.GetFileName() + ": " + chrono->toString("HH:mm:ss").toStdString() + "\n");
		}
		else if (*chrono < oldTime)	// Otherwise compare if new one is better	
		{
			std::string fileContent = "";

			// Replace and rewrite all file content
			if (scoresStream.readAllContent(fileContent))
			{
				std::string oldStrScore = picross.GetFileName() + ": " + oldScore;
				std::string newStrScore = picross.GetFileName() + ": " + chrono->toString("HH:mm:ss").toStdString();

				fileContent.replace(fileContent.find(oldStrScore), oldStrScore.length(), newStrScore);

				FileStream newStream(scoresFilePath, std::fstream::out | std::fstream::trunc | std::fstream::binary);
				newStream.writeContent(fileContent);
			}
		}

		// Print congrats & Show solution
		QMessageBox::information(this, "Fini", "Vous avez gagne !");
		ShowSolution();
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
			picross.GetBtn(row, col)->setDisabled(true);
		}
	}
}

