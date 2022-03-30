#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTranslator>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui
{
	class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	
	~MainWindow() override;

	bool set_language(const QString& language);

private:
	Ui::MainWindow* ui;
	QTranslator* m_translator = nullptr;
};

#endif // MAINWINDOW_H
