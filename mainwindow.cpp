#include "mainwindow.h"
#include "./ui_mainwindow.h"

#define CLASS_NAME_STR(class) #class
#define DYNAMIC_TEXT_TRANSLATE(str) QCoreApplication::translate("HardcodedString", str)

#define INVALID_CREDENTIALS_TITLE DYNAMIC_TEXT_TRANSLATE("Invalid credentials!")
#define INVALID_SERVER_TITLE DYNAMIC_TEXT_TRANSLATE("Invalid server address!")
#define ASK_FOR_SERVER_TITLE DYNAMIC_TEXT_TRANSLATE("Enter server address")
#define SERVER_COLON_STR DYNAMIC_TEXT_TRANSLATE("Server:")
#define ON_SERVER_STR DYNAMIC_TEXT_TRANSLATE("on server")
#define INVALID_SERVER_IP_STR DYNAMIC_TEXT_TRANSLATE("Invalid server IP")
#define EMPTY_LOGIN_STR DYNAMIC_TEXT_TRANSLATE("Login must not be empty")
#define EMPTY_PASSWORD_STR DYNAMIC_TEXT_TRANSLATE("Password must not be empty")
#define PASSWORD_LIMIT_STR DYNAMIC_TEXT_TRANSLATE("Password must be at least 8 characters long")
#define USER_EXISTS_STR DYNAMIC_TEXT_TRANSLATE("This user already exists on this server.")
#define SESSION_NOT_STARTED_STR DYNAMIC_TEXT_TRANSLATE("You typed incorrect login or password or you are not registered yet.")
#define NOTHING_FOUND_STR DYNAMIC_TEXT_TRANSLATE("Nothing was found.")
#define NO_ONLINE_STATUS_STR DYNAMIC_TEXT_TRANSLATE("Can not get user online status. Maybe user does not exist.")
#define USER_OFFLINE_STR DYNAMIC_TEXT_TRANSLATE("You can't send messages to offline users.")
#define NO_USER_SEL_STR DYNAMIC_TEXT_TRANSLATE("First select user for conversation.")


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	refresh_address_indicators();
	auto doclayout = this->ui->line_message->document()->documentLayout();
	connect(doclayout, SIGNAL(documentSizeChanged(const QSizeF&)), this, SLOT(line_message_height_changed(const QSizeF&)));
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
		QMessageBox::critical(this, INVALID_SERVER_TITLE, INVALID_SERVER_IP_STR + " \"" + server_address + "\"");
		return false;
	}
	
	if (login.isEmpty())
	{
		QMessageBox::critical(this, INVALID_CREDENTIALS_TITLE, EMPTY_LOGIN_STR);
		return false;
	}
	
	if (password.isEmpty())
	{
		QMessageBox::critical(this, INVALID_CREDENTIALS_TITLE, EMPTY_PASSWORD_STR);
		return false;
	}
	
	if (password.size() < 8)
	{
		QMessageBox::critical(this, INVALID_CREDENTIALS_TITLE, PASSWORD_LIMIT_STR);
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
		QMessageBox::critical(this, DYNAMIC_TEXT_TRANSLATE("Registration failed"), USER_EXISTS_STR);
		backend = nullptr;
		return;
	}
	if (!backend->begin_session())
	{
		QMessageBox::critical(this, DYNAMIC_TEXT_TRANSLATE("Authorisation failed"), SESSION_NOT_STARTED_STR);
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
		QMessageBox::critical(this, DYNAMIC_TEXT_TRANSLATE("Authorisation failed"), SESSION_NOT_STARTED_STR);
		backend = nullptr;
		return;
	}
	switch_to_messaging();
	this->ui->line_login_log->clear();
	this->ui->line_pass_log->clear();
}


void MainWindow::remove_all_attributes(QDomElement& element)
{
	auto attrs = element.attributes();
	for (int i = 0; i < attrs.size(); ++i)
		element.removeAttribute(attrs.item(i).nodeName());
	auto n = element.firstChild();
	while (!n.isNull())
	{
		auto e = n.toElement();
		if (!e.isNull()) remove_all_attributes(e);
		n = n.nextSibling();
	}
}

void MainWindow::on_button_send_clicked()
{
	QDomDocument doc("message");
	auto msg_text = this->ui->line_message->toPlainText();
	if (!msg_text.isEmpty())
	{
		while (msg_text.back() == '\n') msg_text.chop(1);
		msg_text.replace("\n", "<br/>");
		auto message = msg_text.toStdString();
		auto current_user = this->ui->label_current_user->text();
		if (!current_user.isEmpty())
		{
			bool self_send = backend->my_login() == current_user.toStdString();
			bool online = false;
			if (!self_send && !backend->check_online_status(online, current_user.toStdString()))
			{
				QMessageBox::critical(this, DYNAMIC_TEXT_TRANSLATE("Can't obtain online status"), NO_ONLINE_STATUS_STR);
				return;
			}
			
			if (self_send || online)
			{
				if (self_send || backend->send_message(
						current_user.toStdString(),
						std::vector<uint8_t>{message.begin(), message.end()}
				))
				{
					this->ui->line_message->clear();
					insert_mine_message_into_history(message);
				}
			}
			else QMessageBox::warning(this, DYNAMIC_TEXT_TRANSLATE("User is offline"), USER_OFFLINE_STR);
		}
		else QMessageBox::warning(this, DYNAMIC_TEXT_TRANSLATE("No user selected"), NO_USER_SEL_STR);
	}
}

void MainWindow::insert_mine_message_into_history(const std::string& msg)
{
	insert_message_into_history(msg, "You", "yellow", "right");
}

void MainWindow::insert_extraneous_message_into_history(const std::string& msg, const std::string& username)
{
	insert_message_into_history(msg, username, "rgb(12, 120, 255)", "left");
}

void MainWindow::insert_message_into_history(
		const std::string& msg, const std::string& username, const std::string& border_color, const std::string& align)
{
	QDomDocument message("msg");
	message.setContent(
			QString{
					("<table><tr><td>" + username + "</td></tr><tr><td><pre>" + msg + "</pre></td></tr></table>")
							.c_str()
			}
	);
	auto me = message.documentElement();
	me.setAttribute("cellspacing", "0");
	me.setAttribute("cellpadding", "5");
	me.setAttribute("align", align.c_str());
	me.setAttribute("style", ("margin: 6px; background-color: rgb(30, 22, 1); border-width: 2px; border-color: " + border_color + ";").c_str());
	
	QDomDocument history("history");
	history.setContent(this->ui->message_browser->toHtml());
	auto he = history.documentElement();
	auto n = he.firstChild();
	while (!n.isNull())
	{
		auto e = n.toElement();
		if (!e.isNull() && e.nodeName() == "body")
		{
			e.insertAfter(me, e.lastChild());
			break;
		}
		n = n.nextSibling();
	}
	
	QString exported_history;
	{
		QTextStream stream(&exported_history);
		history.save(stream, 0);
	}
	this->ui->message_browser->setHtml(exported_history);
}

void MainWindow::on_friends_list_widget_itemActivated(QListWidgetItem* item)
{
	bool online = false;
	backend->check_online_status(online, item->text().toStdString());
	this->ui->label_online_status->setText(online ? "[online]" : "[offline]");
	this->ui->label_online_status->setStyleSheet(
			online ?
			"color: rgb(81, 255, 0);\n"
			"background-color: rgb(112, 62, 0);\n"
			"margin-left: 50;\n"
			"margin-right: 50;\n"
			"padding: 2;" :
			"color: rgb(255, 42, 92);\n"
			"background-color: rgb(112, 62, 0);\n"
			"margin-left: 50;\n"
			"margin-right: 50;\n"
			"padding: 2;"
	);
	this->ui->label_current_user->setText(item->text());
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
			QMessageBox::warning(this, DYNAMIC_TEXT_TRANSLATE("Search failed"), NOTHING_FOUND_STR);
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
		backend = nullptr;
		switch_to_log_in();
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

void MainWindow::line_message_height_changed(const QSizeF& new_size)
{
	this->ui->line_message->setMinimumHeight(new_size.height());
}

