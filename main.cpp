#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include <QDesktopWidget>
#include <QRect>
#include <QPoint>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
	
	const QStringList uiLanguages = QLocale::system().uiLanguages();
	MainWindow w;


    QDesktopWidget desktop;
    QRect rect = desktop.availableGeometry(desktop.primaryScreen());
    QPoint center = rect.center();
    int x = center.x() - (w.width()/2);
    int y = center.y() - (w.height()/2);
    center.setX(x);
    center.setY(y);
    w.move(center);

	for (const QString& locale: uiLanguages)
	{
		if (w.set_language(locale)) break;
	}
	w.set_language("uk_UA");
	w.show();
	return a.exec();
}
