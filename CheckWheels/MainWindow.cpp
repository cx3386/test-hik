#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

void MainWindow::on_action_Start_triggered()
{
	hikRealPlay(getRealPlayHandle());
	qDebug() << "realplay finished";
}

HWND MainWindow::getRealPlayHandle()
{
	return (HWND)ui.realPlayViewer->winId();
}
