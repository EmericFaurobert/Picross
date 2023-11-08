#include <QHeaderView>
#include <QDir>
#include "GridSelectWindow.h"
#include "FileReadWrite.h"

GridSelectWindow::GridSelectWindow(const QString & folderName)
{
	gridsFolderPath = QString::fromStdString(pixsFolder) + folderName;

	// ListView
	pGridsTable = new QTableWidget();
	pGridsTable->setColumnCount(2);
	pGridsTable->setHorizontalHeaderLabels({ "Grid", "Time"} );
	pGridsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	pGridsTable->verticalHeader()->setVisible(false);
	pGridsTable->setEditTriggers(QTableWidget::NoEditTriggers);

	// Read grids
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
	pGridsLayout = new QVBoxLayout();
	pGridsLayout->addWidget(pGridsTable);

	setLayout(pGridsLayout);

	// Signals / Slots
	QObject::connect(pGridsTable, &QTableWidget::cellClicked, this, [this](int row, int col) {
		GridSelectWindow::OnLoad(pGridsTable->item(row, 0)->text(), pGridsTable->item(row, 1)->text());
	});
}

void GridSelectWindow::OnLoad(const QString & gridName, const QString & gridScore)
{
	this->close();

	std::string gridPath = QString(gridsFolderPath + "/" + gridName).toStdString() + pixsExtension;
	PlayWindow *grid = new PlayWindow(Picross(gridPath, gridScore.toStdString()));
	grid->show();
}