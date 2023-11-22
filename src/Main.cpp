#include <QApplication>
#include "mainMenu.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	MainMenu *menu = new MainMenu();
	menu->show();

	int status = app.exec();

	return status;
}
