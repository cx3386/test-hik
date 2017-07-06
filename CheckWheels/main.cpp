#include <MainWindow.h>
#include <QtWidgets/QApplication>
#include <QPointer>



QPointer<MainWindow> logBrowser;
void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	if (logBrowser)
		logBrowser->myMessageOutput(type, context, msg);
}

int main(int argc, char *argv[])
{
	QApplication::addLibraryPath("./plugins");
	QApplication a(argc, argv);
	
	logBrowser = new MainWindow();
	logBrowser->show();
	qInstallMessageHandler(outputMessage);
	
	
	return a.exec();
}
