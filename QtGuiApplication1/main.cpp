#include "QtGuiApplication1.h"
#include <QApplication>
#include <iostream>
#include <hikvision.h>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QtGuiApplication1 w;
	w.show();
	RealPlay();
	//printf("i fuck the world");
	//std::cout << "Hello World!" << std::endl;
	return a.exec();
}
