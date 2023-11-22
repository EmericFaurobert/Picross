#pragma once

#include <QTableWidget>
#include "PlayWindow.h"

class GridSelectWindow : public QMainWindow
{
public:
	GridSelectWindow(QWidget *parent = nullptr, const QString &folderName = "");

private:
	void OnLoad(const QString &gridName, const QString &gridScore);

private:
	QWidget* centralWidget{ nullptr };
	QVBoxLayout *pGridsLayout { nullptr };
	QTableWidget *pGridsTable { nullptr };
	QString gridsFolderPath = "";
};