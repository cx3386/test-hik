#include <MainWindow.h>
#include <QtWidgets/QApplication>
#include <QPointer>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	MainWindow w;
	//qInstallMessageHandler(w.myMessageOutput);
	w.show();
	
	return a.exec();
}
