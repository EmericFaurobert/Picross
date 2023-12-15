#include <QApplication>
#include "MainMenuWindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	MainMenuWindow *menu = new MainMenuWindow();
	menu->show();

	int status = app.exec();

	return status;
}
