#include <QHeaderView>
#include <QDir>
#include "GridSelectWindow.h"

GridSelectWindow::GridSelectWindow(const QString & folderName)
{
	gridsFolderPath = "resources\\Picross\\" + folderName;

	// ListView
	pGridsTable = new QTableWidget();
	pGridsTable->setColumnCount(2);
	pGridsTable->setHorizontalHeaderLabels({ "Grid", "Time"} );
	pGridsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	pGridsTable->verticalHeader()->setVisible(false);
	pGridsTable->setEditTriggers(QTableWidget::NoEditTriggers);

	// Read grids
	QDir directory(gridsFolderPath);
	QStringList gridsName = directory.entryList(QStringList() << "*.pix" << "*.PIX" , QDir::Files);
	pGridsTable->setRowCount(gridsName.size());
	for (int i = 0; i < gridsName.size(); ++i)
	{
		pGridsTable->setItem(i, 0, new QTableWidgetItem(gridsName[i]));
		pGridsTable->setItem(i, 1, new QTableWidgetItem("00:00:00"));
	}

	// Layouts
	pGridsLayout = new QVBoxLayout();
	pGridsLayout->addWidget(pGridsTable);

	setLayout(pGridsLayout);

	// Signals / Slots
	QObject::connect(pGridsTable, &QTableWidget::cellClicked, this, [this](int row, int col) { 
		GridSelectWindow::OnLoad(pGridsTable->item(row, 0)->text());
	});
}

void GridSelectWindow::OnLoad(const QString & gridName)
{
	this->close();

	std::string gridPath = QString(gridsFolderPath + "\\" + gridName).toStdString();
	PlayWindow *grid = new PlayWindow(Picross(gridPath));
	grid->show();
}