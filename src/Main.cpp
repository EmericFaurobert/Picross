#include <QApplication>
#include <QTextCodec>
#include "mainMenu.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	MainMenu menu;
	menu.show();

	return app.exec();
}
