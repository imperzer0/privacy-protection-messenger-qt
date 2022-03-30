#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "call_backend.hpp"
#include <QPalette>

#define SET_PALETTE(object) (object)->setPalette(palette)

MainWindow::MainWindow(QWidget* parent)
		: QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	QPalette palette("palette.xml");
	SET_PALETTE(this);
	SET_PALETTE(ui->authorization);
	SET_PALETTE(ui->button_already_registered);
	SET_PALETTE(ui->button_log_in);
	SET_PALETTE(ui->button_send);
	SET_PALETTE(ui->button_sign_in);
	SET_PALETTE(ui->button_not_registered);
	SET_PALETTE(ui->centralwidget);
	SET_PALETTE(ui->groupBox_chat);
	SET_PALETTE(ui->groupBox_friends);
	SET_PALETTE(ui->label_login);
	SET_PALETTE(ui->label_login_2);
	SET_PALETTE(ui->label_pass);
	SET_PALETTE(ui->label_pass_2);
	SET_PALETTE(ui->label_welcome);
	SET_PALETTE(ui->listView_friends);
	SET_PALETTE(ui->line_login);
	SET_PALETTE(ui->line_login_2);
	SET_PALETTE(ui->line_message);
	SET_PALETTE(ui->line_pass);
	SET_PALETTE(ui->line_pass_2);
	SET_PALETTE(ui->menu_bar);
	SET_PALETTE(ui->menu_Language);
	SET_PALETTE(ui->messaging);
	SET_PALETTE(ui->message_browser);
	SET_PALETTE(ui->registration);
	SET_PALETTE(ui->search_friends);
	SET_PALETTE(ui->stackedWidget);
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


void MainWindow::on_action_English_triggered()
{
	this->set_language("en_US");
}


void MainWindow::on_action_Ukrainian_triggered()
{
	this->set_language("uk_UA");
}


void MainWindow::on_button_already_registered_clicked()
{
	this->ui->stackedWidget->setCurrentWidget(this->ui->stackedWidget->widget(1));
}


void MainWindow::on_button_not_registered_clicked()
{
	this->ui->stackedWidget->setCurrentWidget(this->ui->stackedWidget->widget(0));
}

