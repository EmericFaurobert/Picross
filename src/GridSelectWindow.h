#pragma once

#include <QWidget>
#include <QTableWidget>
#include "PlayWindow.h"

class GridSelectWindow : public QWidget
{
public:
	GridSelectWindow(const QString & folderName);

private:
	void OnLoad(const QString & gridName, const QString & gridScore);

private:
	QVBoxLayout *pGridsLayout { nullptr };
	QTableWidget *pGridsTable { nullptr };
	QString gridsFolderPath = "";
};