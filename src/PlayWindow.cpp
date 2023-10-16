#include "PlayWindow.h"
#include <QMessageBox>
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
	// The chrono layout
	leftLayout = new QVBoxLayout();
	leftLayout->setAlignment(Qt::AlignCenter);
	leftLayout->setContentsMargins(10, 0, 30, 0);

	// The picross layout
	gridLayout = new QGridLayout();
	gridLayout->setSpacing(0);
	centralWidget = new QWidget();
	centralWidget->setLayout(gridLayout);

	QSizePolicy spRetain = centralWidget->sizePolicy();
	spRetain.setRetainSizeWhenHidden(true);
	centralWidget->setSizePolicy(spRetain);

	// The color picker layout
	rightLayout = new QVBoxLayout();
	rightLayout->setAlignment(Qt::AlignCenter);
	rightLayout->setContentsMargins(30, 0, 10, 0);
	rightLayout->setSpacing(10);

	// The main layout which contains all others
	mainLayout = new QHBoxLayout();
	mainLayout->setAlignment(Qt::AlignCenter);
	mainLayout->addLayout(leftLayout);
	mainLayout->addWidget(centralWidget);
	mainLayout->addLayout(rightLayout);

	setLayout(mainLayout);
}

void PlayWindow::InitGrid()
{
	for (uchar row = 0; row < picross.GetHeight(); ++row)
	{
		for (uchar col = 0; col < picross.GetWidth(); ++col)
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
	if (picross.IsColored())
	{
		ColorPalette colPal = picross.GetColorPalette();
		bool isSelectColorInit = false;

		for (ColorPalette::const_iterator it = colPal.begin(); it != colPal.end(); ++it)
		{
			uchar colorIdx = it->first;

			if (picross.IsColorNecessary(colorIdx))
			{
				QPushButton *btn = new QPushButton();
				colorPicker[colorIdx] = btn;
				rightLayout->addWidget(btn);

				btn->setFixedSize(QSize(40, 40));
				btn->setStyleSheet(QString("background-color: %1;").arg(it->second.ToRgb()));
				QObject::connect(btn, &QPushButton::clicked, [this, colorIdx] { ChangeCurrentColor(colorIdx); });

				if (!isSelectColorInit)
				{
					colorPicker[colorIdx]->setStyleSheet(colorPicker[colorIdx]->styleSheet() + QString("border: 3px solid %1;").arg(caseColorPickerBorder.ToHex()));
					selectedColorIdx = colorIdx;
					isSelectColorInit = true;
				}
			}
		}
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

	uint hThickness = (row != 0 && row % 5 == 0) ? 2 : 1;
	uint vThickness = (col != 0 && col % 5 == 0) ? 2 : 1;

	QString strBorderColor = Color(128, 128, 128).ToRgb();

	// Common properties
	QString css = QString("background-color: %1;").arg(color.ToRgb()) +
		QString("border-top: %1px solid %2;").arg(hThickness).arg(strBorderColor) +
		QString("border-left: %1px solid %2;").arg(vThickness).arg(strBorderColor);

	if (row < picross.GetHeight() - 1 && col < picross.GetWidth() - 1) // Default case
	{
		btn->setStyleSheet(css);
	}
	else if (row == picross.GetHeight() - 1 && col < picross.GetWidth() - 1)	// Last row
	{
		btn->setStyleSheet(css + QString("border-bottom: 1px solid %1;").arg(strBorderColor));
	}
	else if (row < picross.GetHeight() - 1 && col == picross.GetWidth() - 1)	// Last column
	{
		btn->setStyleSheet(css + QString("border-right: 1px solid %1;").arg(strBorderColor));
	}
	else // Last case (last row & last column)
	{
		btn->setStyleSheet(QString("border: 1px solid %1; background-color: %2;").arg(strBorderColor).arg(color.ToRgb()));
	}
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
	if (click == Qt::LeftButton)
	{
		picross.SetCurrentState(row, col, State::checked);
		picross.SetCurrentColor(row, col, selectedColorIdx);
		SetBtnStyle(row, col, picross.GetColor(selectedColorIdx));
	}
	else if(click == Qt::RightButton)
	{
		picross.SetCurrentState(row, col, State::unchecked);
		SetBtnStyle(row, col, defaultGridCaseColor);
		SetBtnBackground(row, col, "resources/Icons/BlackCross.svg");
	}

	if (picross.IsCorrect())
	{
		timer->stop();
		pauseButton->setDisabled(true);
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

