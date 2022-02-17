#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	
	const QStringList uiLanguages = QLocale::system().uiLanguages();
	MainWindow w;
	for (const QString& locale: uiLanguages)
	{
		if (w.set_language(locale)) break;
	}
	w.show();
	return a.exec();
}
