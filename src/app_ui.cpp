#include <iostream>
#include <QApplication>
#include <QDesktopWidget>
#include "task.h"
#include "coex/coex.h"
#include "coex/config.h"
#include "coex/typeos.h"
#include "ui/window.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	PidginWindow mainWindow;
	
	// center of desctop
	QDesktopWidget wid;
	int screenWidth = wid.screen()->width();
	int screenHeight = wid.screen()->height();
	mainWindow.setGeometry((screenWidth/2)-(mainWindow.width()/2),(screenHeight/2)-(mainWindow.height()/2),mainWindow.width(),mainWindow.height());
	
	mainWindow.show();
	return app.exec();
};
