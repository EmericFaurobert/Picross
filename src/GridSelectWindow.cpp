#include <QHeaderView>
#include <QDir>
#include "GridSelectWindow.h"

GridSelectWindow::GridSelectWindow(QWidget *parent, const QString &folderName) : QMainWindow(parent)
{
	// Allocations
	centralWidget = new QWidget(this);
	pGridsLayout = new QVBoxLayout(centralWidget);
	pGridsTable = new QTableWidget(centralWidget);

	// Table Properties
	pGridsTable->setColumnCount(2);
	pGridsTable->setHorizontalHeaderLabels({ "Grid", "Time"} );
	pGridsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	pGridsTable->verticalHeader()->setVisible(false);
	pGridsTable->setEditTriggers(QTableWidget::NoEditTriggers);

	// Table Values
	gridsFolderPath = QString::fromStdString(pixsFolder) + folderName;
	const QDir directory(gridsFolderPath);
	const QStringList gridsName = directory.entryList(QStringList() << "*.pix" << "*.PIX" , QDir::Files);
	pGridsTable->setRowCount(gridsName.size());
	for (int i = 0; i < gridsName.size(); ++i)
	{
		// Name
		QString gridName = gridsName[i].section('.', 0, 0);
		pGridsTable->setItem(i, 0, new QTableWidgetItem(gridName));

		// Best score
		const std::string  scoresFilePath = pixsFolder + scoresFileName;
		FileStream scoresStream(scoresFilePath, std::fstream::in);

		std::string currentScore = "";
		std::string fileName = folderName.toStdString() + "/" + gridName.toStdString();

		if (scoresStream.parseValue(fileName, currentScore))
		{
			pGridsTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(currentScore)));
		}
		else
		{
			pGridsTable->setItem(i, 1, new QTableWidgetItem(""));
		}
	}

	// Layouts
	pGridsLayout->addWidget(pGridsTable);

	// Central Widget
	centralWidget->setLayout(pGridsLayout);
	setCentralWidget(centralWidget);

	// General Poperties
	setAttribute(Qt::WA_DeleteOnClose, true);

	// Signals / Slots
	QObject::connect(pGridsTable, &QTableWidget::cellClicked, this, [this](int row, int col) {
		GridSelectWindow::OnLoad(pGridsTable->item(row, 0)->text(), pGridsTable->item(row, 1)->text());
	});
}

void GridSelectWindow::OnLoad(const QString & gridName, const QString & gridScore)
{
	std::string gridPath = QString(gridsFolderPath + "/" + gridName).toStdString() + pixsExtension;
	PlayWindow *grid = new PlayWindow(nullptr, Picross(gridPath, gridScore.toStdString()));
	grid->show();

	parentWidget()->parentWidget()->close();	// Deleting MainMenu window...
}