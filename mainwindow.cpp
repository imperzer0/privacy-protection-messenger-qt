#include <QMessageBox>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <execute-process-linux-defs>
#include <QInputDialog>
#include <arpa/inet.h>
#include <QProxyStyle>

#define CLASS_NAME_STR(class) #class

#define MESSAGE_BOX_TRANSLATE(str) QCoreApplication::translate(CLASS_NAME_STR(MessageBox), str)

#define INVALID_CREDENTIALS_TITLE MESSAGE_BOX_TRANSLATE("Invalid credentials!")
#define INVALID_SERVER_TITLE MESSAGE_BOX_TRANSLATE("Invalid server address!")
#define ASK_FOR_SERVER_TITLE MESSAGE_BOX_TRANSLATE("Enter server address")

MainWindow::MainWindow(QWidget* parent)
		: QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	this->setWindowTitle("Privacy Protection Messenger (on server \"" + server_address + "\")");
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
			this->refresh_address_indicators();
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

void MainWindow::on_actionMoskalian_triggered()
{
	this->set_language("ru_RU");
}


void MainWindow::on_button_already_registered_clicked()
{
	this->ui->stackedWidget->setCurrentWidget(this->ui->stackedWidget->widget(1));
}


void MainWindow::on_button_not_registered_clicked()
{
	this->ui->stackedWidget->setCurrentWidget(this->ui->stackedWidget->widget(0));
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
		QMessageBox::critical(this, INVALID_SERVER_TITLE, MESSAGE_BOX_TRANSLATE("Invalid ip address") + " \"" + server_address + "\"");
		return false;
	}
	
	if (login.isEmpty())
	{
		QMessageBox::critical(this, INVALID_CREDENTIALS_TITLE, MESSAGE_BOX_TRANSLATE("Login must not be empty"));
		return false;
	}
	
	if (password.isEmpty())
	{
		QMessageBox::critical(this, INVALID_CREDENTIALS_TITLE, MESSAGE_BOX_TRANSLATE("Password must not be empty"));
		return false;
	}
	
	if (password.size() < 8)
	{
		QMessageBox::critical(this, INVALID_CREDENTIALS_TITLE, MESSAGE_BOX_TRANSLATE("Password must be at least 8 characters long"));
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
				this, MESSAGE_BOX_TRANSLATE("Registration failed"),
				MESSAGE_BOX_TRANSLATE("This user already exists on this server or server is compromised."));
		return;
	}
	if (!backend->begin_session())
	{
		QMessageBox::critical(
				this, MESSAGE_BOX_TRANSLATE("Authorisation failed"),
				MESSAGE_BOX_TRANSLATE("You typed incorrect login or password or you are not registered yet, or server is compromised."));
		return;
	}
	this->ui->stackedWidget->setCurrentWidget(this->ui->stackedWidget->widget(2));
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
				this, MESSAGE_BOX_TRANSLATE("Authorisation failed"),
				MESSAGE_BOX_TRANSLATE("You typed incorrect login or password or you are not registered yet, or server is compromised."));
		return;
	}
	this->ui->stackedWidget->setCurrentWidget(this->ui->stackedWidget->widget(2));
	this->ui->line_login_log->clear();
	this->ui->line_pass_log->clear();
}


void MainWindow::on_button_send_clicked()
{

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
					this, MESSAGE_BOX_TRANSLATE("Search failed"),
					MESSAGE_BOX_TRANSLATE("Nothing was found or server is compromised."));
			return;
		}
	
	QIcon icon = QProxyStyle().standardIcon(QStyle::SP_ComputerIcon);
	for (auto& e: result)
		this->ui->friends_list_widget->addItem(new QListWidgetItem(icon, e.c_str()));
}


void MainWindow::on_action_Set_server_triggered()
{
	auto address = QInputDialog::getText(this, ASK_FOR_SERVER_TITLE, QCoreApplication::translate(CLASS_NAME_STR(MainWindow), "Server:"));
	if (!address.isEmpty())
	{
		server_address = address;
		this->refresh_address_indicators();
	}
}


void MainWindow::on_action_Disconnect_from_server_triggered()
{
	server_address = "";
	this->refresh_address_indicators();
}

void MainWindow::refresh_address_indicators()
{
	this->setWindowTitle("Privacy Protection Messenger (on server \"" + server_address + "\")");
	this->ui->action_Server_address->setText(QCoreApplication::translate(CLASS_NAME_STR(MainWindow), "Server:") + " " + server_address);
}


void MainWindow::on_action_Logout_triggered()
{
	if (backend)
	{
		this->ui->stackedWidget->setCurrentIndex(1);
		backend = nullptr;
	}
}

