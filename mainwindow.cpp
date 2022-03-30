#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "call_backend.hpp"


MainWindow::MainWindow(QWidget* parent)
		: QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
    ui->line_sign_in->setStyleSheet(
            "QLineEdit{ "
            "color: #e9eeee;"
            "font-size: 10px;"
            "border-bottom: 2px solid rgb(73, 73, 73);"
            "border-radius: 0px;"
            "padding: 0 8px;"
            "selection-background-color: darkgray;"
            "font-size: 16px;}"
            "QLineEdit:focus { "
            "border-bottom: 2px solid rgb(109, 109, 109);}"
    );
//
    ui->line_pass->setStyleSheet(
            "QLineEdit{ "
            "border-bottom: 2px solid rgb(73, 73, 73);"
            "border-radius: 0px;"
            "padding: 0 8px;"
            "selection-background-color: darkgray;"
            "font-size: 16px;}"
            "QLineEdit:focus { "
            "border-bottom: 2px solid rgb(109, 109, 109);}"
    );

    ui->search_friends->setStyleSheet(
            "QLineEdit{"
            "padding: 0.24em;"
            "background-color: #242F3D;"
            "color: #F4F4F4;}"
            );

    ui->lineEdit->setStyleSheet(
            "QLineEdit{"
            "padding: 0.29em;"
            "background-color: #242F3D;"
            "color: #F4F4F4;}"
            );



    ui->button_sign_in->setStyleSheet(
            "QAbstractButton{"
            "color: #F4F4F4;"
            "background-color: #3476AB;}"
            );
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
		{
			delete m_translator;
			this->ui->retranslateUi(this);
		}
	}
	
	QString lang_name = QLocale(language).name();
	if (lang_name == "en_US" || lang_name.isEmpty())
	{
		m_translator = nullptr;
		return true;
	}
	
	m_translator = new QTranslator(this);
	QString base_name = "privacy-protection-messenger-qt_" + lang_name;
	if (m_translator->load(":/lang/" + base_name))
	{
		if (QCoreApplication::installTranslator(m_translator))
		{
			this->ui->retranslateUi(this);
			return true;
		}
	}
	return false;
}

