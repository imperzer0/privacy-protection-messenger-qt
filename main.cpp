#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include <QScreen>
#include <QRect>
#include <QPoint>


int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
	MainWindow w;

    auto screen = QGuiApplication::primaryScreen();
    QRect rect = screen->availableGeometry();
    QPoint center = rect.center();
    int x = center.x() - (w.width()/2);
    int y = center.y() - (w.height()/2);
    center.setX(x);
    center.setY(y);
    w.move(center);

	w.show();
	return a.exec();
}
