#include <QMessageBox>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <execute-process-linux-defs>
#include <QInputDialog>
#include <arpa/inet.h>
#include <QProxyStyle>

#define CLASS_NAME_STR(class) #class

#define DYNAMIC_TEXT_TRANSLATE(str) QCoreApplication::translate("HardcodedString", str)

#define INVALID_CREDENTIALS_TITLE DYNAMIC_TEXT_TRANSLATE("Invalid credentials!")
#define INVALID_SERVER_TITLE DYNAMIC_TEXT_TRANSLATE("Invalid server address!")
#define ASK_FOR_SERVER_TITLE DYNAMIC_TEXT_TRANSLATE("Enter server address")
#define SERVER_COLON_STR DYNAMIC_TEXT_TRANSLATE("Server:")
#define ON_SERVER_STR DYNAMIC_TEXT_TRANSLATE("on server")

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	refresh_address_indicators();
}

MainWindow::~MainWindow()
{
	delete ui;
}

bool MainWindow::set_language(const QString& language)
{
	if (m_translator)
	{
		if (QCoreApplication::removeTranslator(m_translator.get()))
		{
			m_translator = nullptr;
			this->ui->retranslateUi(this);
		}
	}
	
	QString lang_name = QLocale(language).name();
	if (lang_name == "en_US" || lang_name.isEmpty())
	{
		m_translator = nullptr;
		return true;
	}
	
	m_translator = std::make_unique<QTranslator>(this);
	QString base_name = "privacy-protection-messenger-qt_" + lang_name;
	if (m_translator->load(":/lang/" + base_name))
	{
		if (QCoreApplication::installTranslator(m_translator.get()))
		{
			this->ui->retranslateUi(this);
			refresh_address_indicators();
			return true;
		}
	}
	return false;
}


void MainWindow::on_action_English_triggered()
{
	set_language("en_US");
}


void MainWindow::on_action_Ukrainian_triggered()
{
	set_language("uk_UA");
}


void MainWindow::on_action_Transcarpatian_triggered()
{
	set_language("ru_UA");
}

void MainWindow::on_actionMoskalian_triggered()
{
	set_language("ru_RU");
}


void MainWindow::on_button_already_registered_clicked()
{
	switch_to_log_in();
}


void MainWindow::on_button_not_registered_clicked()
{
	switch_to_sign_up();
}

inline static bool validate_number(char* str)
{
	for (; *str; ++str)
		if (!isdigit(*str))
			return false;
	return true;
}

inline static bool validate_ip(std::string ip)
{
	auto tmp = ip.data();
	for (; *tmp; ++tmp)
		if (*tmp == ':')
		{
			*tmp = 0;
			if (!validate_number(tmp + 1)) return false;
			long port = ::atoi(tmp + 1);
			if (port != (uint16_t)port) return false;
			break;
		}
	
	in_addr addr{ };
	if (::inet_aton(ip.data(), &addr) == 0)
		return false;
	
	return true;
}

inline bool MainWindow::assert_data(const QString& login, const QString& password)
{
	if (!validate_ip(server_address.toStdString()))
	{
		QMessageBox::critical(this, INVALID_SERVER_TITLE, DYNAMIC_TEXT_TRANSLATE("Invalid ip address") + " \"" + server_address + "\"");
		return false;
	}
	
	if (login.isEmpty())
	{
		QMessageBox::critical(this, INVALID_CREDENTIALS_TITLE, DYNAMIC_TEXT_TRANSLATE("Login must not be empty"));
		return false;
	}
	
	if (password.isEmpty())
	{
		QMessageBox::critical(this, INVALID_CREDENTIALS_TITLE, DYNAMIC_TEXT_TRANSLATE("Password must not be empty"));
		return false;
	}
	
	if (password.size() < 8)
	{
		QMessageBox::critical(this, INVALID_CREDENTIALS_TITLE, DYNAMIC_TEXT_TRANSLATE("Password must be at least 8 characters long"));
		return false;
	}
	
	return true;
}

void MainWindow::on_button_sign_up_clicked()
{
	QString login = this->ui->line_login_reg->text(),
			password = this->ui->line_pass_reg->text();
	
	if (!assert_data(login, password)) return;
	
	backend = std::make_unique<call_backend>(server_address.toStdString(), login.toStdString(), password.toStdString());
	if (!backend->register_user(this->ui->line_display_name->text().toStdString()))
	{
		QMessageBox::critical(
				this, DYNAMIC_TEXT_TRANSLATE("Registration failed"),
				DYNAMIC_TEXT_TRANSLATE("This user already exists on this server or server is compromised.")
		);
		backend = nullptr;
		return;
	}
	if (!backend->begin_session())
	{
		QMessageBox::critical(
				this, DYNAMIC_TEXT_TRANSLATE("Authorisation failed"),
				DYNAMIC_TEXT_TRANSLATE("You typed incorrect login or password or you are not registered yet, or server is compromised.")
		);
		backend = nullptr;
		return;
	}
	switch_to_messaging();
	this->ui->line_login_reg->clear();
	this->ui->line_pass_reg->clear();
	this->ui->line_display_name->clear();
}


void MainWindow::on_button_log_in_clicked()
{
	QString login = this->ui->line_login_log->text(),
			password = this->ui->line_pass_log->text();
	
	if (!assert_data(login, password)) return;
	
	backend = std::make_unique<call_backend>(server_address.toStdString(), login.toStdString(), password.toStdString());
	if (!backend->begin_session())
	{
		QMessageBox::critical(
				this, DYNAMIC_TEXT_TRANSLATE("Authorisation failed"),
				DYNAMIC_TEXT_TRANSLATE("You typed incorrect login or password or you are not registered yet, or server is compromised.")
		);
		backend = nullptr;
		return;
	}
	switch_to_messaging();
	this->ui->line_login_log->clear();
	this->ui->line_pass_log->clear();
}


void MainWindow::on_button_send_clicked()
{
	/// TODO: message sending procedure
}


void MainWindow::on_search_friends_textChanged(const QString& text)
{
	this->ui->friends_list_widget->clear();
	if (text.isEmpty())
		return;
	
	std::list<std::string> result;
	if (backend)
		if (!backend->find_users_by_login(result, text.toStdString()))
		{
			QMessageBox::warning(
					this, DYNAMIC_TEXT_TRANSLATE("Search failed"),
					DYNAMIC_TEXT_TRANSLATE("Nothing was found or server is compromised.")
			);
			return;
		}
	
	QIcon icon = QProxyStyle().standardIcon(QStyle::SP_ComputerIcon);
	for (auto& e: result)
		this->ui->friends_list_widget->addItem(new QListWidgetItem(icon, e.c_str()));
}


void MainWindow::on_action_Set_server_triggered()
{
	auto address = QInputDialog::getText(this, ASK_FOR_SERVER_TITLE, SERVER_COLON_STR);
	if (!address.isEmpty())
	{
		server_address = address;
		refresh_address_indicators();
	}
}


void MainWindow::on_action_Disconnect_from_server_triggered()
{
	server_address = "";
	refresh_address_indicators();
}

void MainWindow::refresh_address_indicators()
{
	setWindowTitle("Privacy Protection Messenger (" + ON_SERVER_STR + " \"" + server_address + "\")");
	this->ui->action_Server_address->setText(SERVER_COLON_STR + " " + server_address);
}


void MainWindow::on_action_Logout_triggered()
{
	if (backend)
	{
		switch_to_log_in();
		backend = nullptr;
	}
}

void MainWindow::switch_to_sign_up()
{
	auto* root = this->ui->stackedWidget->widget(0);
	this->ui->stackedWidget->currentWidget()->setEnabled(false);
	this->ui->stackedWidget->setCurrentWidget(root);
	root->setEnabled(!backend.operator bool());
	this->ui->line_login_reg->setEnabled(!backend.operator bool());
	this->ui->line_pass_reg->setEnabled(!backend.operator bool());
	this->ui->line_display_name->setEnabled(!backend.operator bool());
	this->ui->button_sign_up->setEnabled(!backend.operator bool());
	this->ui->button_already_registered->setEnabled(!backend.operator bool());
}

void MainWindow::switch_to_log_in()
{
	auto* root = this->ui->stackedWidget->widget(1);
	this->ui->stackedWidget->currentWidget()->setEnabled(false);
	this->ui->stackedWidget->setCurrentWidget(root);
	root->setEnabled(!backend.operator bool());
	this->ui->line_login_log->setEnabled(!backend.operator bool());
	this->ui->line_pass_log->setEnabled(!backend.operator bool());
	this->ui->button_log_in->setEnabled(!backend.operator bool());
	this->ui->button_not_registered->setEnabled(!backend.operator bool());
}

void MainWindow::switch_to_messaging()
{
	auto* root = this->ui->stackedWidget->widget(2);
	this->ui->stackedWidget->currentWidget()->setEnabled(false);
	this->ui->stackedWidget->setCurrentWidget(root);
	root->setEnabled(backend.operator bool());
	this->ui->button_send->setEnabled(backend.operator bool());
	this->ui->message_browser->setEnabled(backend.operator bool());
	this->ui->line_message->setEnabled(backend.operator bool());
	this->ui->search_friends->setEnabled(backend.operator bool());
	this->ui->friends_list_widget->setEnabled(backend.operator bool());
}

