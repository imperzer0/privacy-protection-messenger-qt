#include <QMessageBox>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <execute-process-linux-defs>
#include <QInputDialog>

#define INVALID_CREDENTIALS_TITLE "Invalid credentials!"
#define INVALID_SERVER_TITLE "Invalid server address!"
#define ASK_FOR_SERVER_TITLE "Enter server address"

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


void MainWindow::on_action_Transcarpatian_triggered()
{
	this->set_language("ru_UA");
}


void MainWindow::on_button_already_registered_clicked()
{
	this->ui->stackedWidget->setCurrentWidget(this->ui->stackedWidget->widget(1));
}


void MainWindow::on_button_not_registered_clicked()
{
	this->ui->stackedWidget->setCurrentWidget(this->ui->stackedWidget->widget(0));
}

void MainWindow::on_button_sign_in_clicked()
{
	QString login = this->ui->line_login->text(),
			password = this->ui->line_pass->text();
	
	if (login.isEmpty())
	{
		QMessageBox::critical(this, INVALID_CREDENTIALS_TITLE, "Login must not be empty");
		return;
	}
	
	if (password.isEmpty())
	{
		QMessageBox::critical(this, INVALID_CREDENTIALS_TITLE, "Password must not be empty");
		return;
	}
	
	if (password.size() < 8)
	{
		QMessageBox::critical(this, INVALID_CREDENTIALS_TITLE, "Password must be at least 8 characters long");
		return;
	}
	
	if (server_address.isEmpty())
	{
		QMessageBox::critical(this, INVALID_SERVER_TITLE, "No such server \"" + server_address + "\"");
		return;
	}
	
	delete backend;
	backend = new call_backend(server_address.toStdString(), login.toStdString(), password.toStdString());
	backend->register_user(this->ui->line_display_name->text().toStdString());
	backend->begin_session();
	this->ui->stackedWidget->setCurrentWidget(this->ui->stackedWidget->widget(2));
}


void MainWindow::on_button_log_in_clicked()
{
	QString login = this->ui->line_login_2->text(),
			password = this->ui->line_pass_2->text();
	
	if (login.isEmpty())
	{
		QMessageBox::critical(this, INVALID_CREDENTIALS_TITLE, "Login must not be empty");
		return;
	}
	
	if (password.isEmpty())
	{
		QMessageBox::critical(this, INVALID_CREDENTIALS_TITLE, "Password must not be empty");
		return;
	}
	
	if (password.size() < 8)
	{
		QMessageBox::critical(this, INVALID_CREDENTIALS_TITLE, "Password must be at least 8 characters long");
		return;
	}
	
	if (server_address.isEmpty())
	{
		QMessageBox::critical(this, INVALID_SERVER_TITLE, "No such server \"" + server_address + "\"");
		return;
	}
	
	delete backend;
	backend = new call_backend(server_address.toStdString(), login.toStdString(), password.toStdString());
	backend->register_user(this->ui->line_display_name->text().toStdString());
	backend->begin_session();
	this->ui->stackedWidget->setCurrentWidget(this->ui->stackedWidget->widget(2));
}


void MainWindow::on_button_send_clicked()
{

}


void MainWindow::on_search_friends_textChanged(const QString& text)
{

}


void MainWindow::on_action_Set_server_triggered()
{
	server_address = QInputDialog::getText(this, ASK_FOR_SERVER_TITLE, "Server: ");
	this->set_address_in_menu_bar();
}


void MainWindow::on_action_Disconnect_from_server_triggered()
{
	server_address = "";
	this->set_address_in_menu_bar();
}

void MainWindow::set_address_in_menu_bar()
{
	this->ui->action_Server_address->setText("Server: " + server_address);
}

