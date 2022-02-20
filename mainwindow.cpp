#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
		: QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

bool MainWindow::set_language(const QString& language)
{
	if (m_translator)
	{
		if (QCoreApplication::removeTranslator(m_translator))
		{ delete m_translator; }
		m_translator = new QTranslator(this);
	}
	QString lang_name = QLocale(language).name();
	QString base_name = "privacy-protection-messenger-qt_" + lang_name + ".qm";
	if (lang_name != "en_US" && !lang_name.isEmpty() && m_translator->load(":/language/" + base_name))
	{
		return QCoreApplication::installTranslator(m_translator);
	}
	return false;
}

bool MainWindow::reset_language()
{
	if (m_translator)
	{
		if (QCoreApplication::removeTranslator(m_translator))
		{
			delete m_translator;
			m_translator = nullptr;
			return true;
		}
	}
	return false;
}

