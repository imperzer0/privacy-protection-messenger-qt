#ifndef PRIVACY_PROTECTION_MESSENGER_QT_MAINWINDOW_H
#define PRIVACY_PROTECTION_MESSENGER_QT_MAINWINDOW_H

#include <QMainWindow>
#include <QTranslator>
#include <QMessageBox>
#include <QInputDialog>
#include <QProxyStyle>
#include <QPlainTextDocumentLayout>
#include <QtXml/QtXml>
#include <QListWidgetItem>
#include <memory>
#include <arpa/inet.h>
#include <string>
#include "call_backend.hpp"

QT_BEGIN_NAMESPACE
namespace Ui
{
	class MainWindow;
}
QT_END_NAMESPACE

typedef enum : int
{
	page_invalid = -1,
	page_password,
	page_signup,
	page_login,
	page_chat
} pages;

class poll_incoming_msg_thread;

class switch_performer;

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	
	~MainWindow() override;
	
	bool set_language(const QString& language);

private slots:
	
	void on_action_English_triggered();
	
	void on_action_Ukrainian_triggered();
	
	void on_action_Transcarpatian_triggered();
	
	void on_actionMoskalian_triggered();
	
	void on_button_already_registered_clicked();
	
	void on_button_not_registered_clicked();
	
	void on_button_sign_up_clicked();
	
	void on_button_log_in_clicked();
	
	void on_button_send_clicked();
	
	void on_action_Set_server_triggered();
	
	void on_action_Disconnect_from_server_triggered();
	
	void on_action_Logout_triggered();
	
	void line_message_height_changed(const QSizeF& new_size);
	
	void on_friends_list_widget_itemActivated(QListWidgetItem* item);
	
	void on_line_display_name_returnPressed();
	
	void on_line_pass_log_returnPressed();
	
	void on_search_friends_returnPressed();
	
	void insert_mine_message_into_history(const std::string& msg);
	
	void insert_extraneous_message_into_history(const std::string& msg, const std::string& username);
	
	void on_button_donate_clicked();

private:
	Ui::MainWindow* ui;
	std::unique_ptr<QTranslator> m_translator = nullptr;
	std::unique_ptr<call_backend> backend = nullptr;
	QString server_address = "127.0.0.1";
	std::unique_ptr<poll_incoming_msg_thread> thread = nullptr;
	std::unique_ptr<switch_performer> current_page = std::make_unique<switch_performer>(this);
	time_t prev = 0;
	std::string current_user;
	
	void refresh_address_indicators();
	
	inline bool assert_data(const QString& login, const QString& password);
	
	void remove_all_attributes(QDomElement& node);
	
	void insert_message_into_history(const std::string& msg, const std::string& username, const std::string& border_color, const std::string& align);
	
	void set_online_status_label(bool online);
	
	void log_in();
	
	void sign_up();

private:
	friend class poll_incoming_msg_thread;
	
	friend class switcher;
	
	friend class signup;
	
	friend class login;
	
	friend class chat;
};


class poll_incoming_msg_thread : public QThread
{
Q_OBJECT

public:
	poll_incoming_msg_thread(MainWindow* main_window);
	
	~poll_incoming_msg_thread();

signals:
	
	void append_message_to_history(const std::string&, const std::string&);

protected:
	void run() override;

private:
	MainWindow* main_window;
};


class switcher
{
public:
	inline explicit switcher(MainWindow* mw);
	
	virtual void _switch() = 0;
	
	virtual void _unswitch() = 0;

protected:
	friend class switch_performer;
	
	MainWindow* mw;
};

class signup : public switcher
{
public:
	explicit signup(MainWindow* mw);
	
	void _switch() override;
	
	void _unswitch() override;
};

class login : public switcher
{
public:
	explicit login(MainWindow* mw);
	
	void _switch() override;
	
	void _unswitch() override;
};

class chat : public switcher
{
public:
	explicit chat(MainWindow* mw);
	
	void _switch() override;
	
	void _unswitch() override;
};

class switch_performer
{
public:
	switch_performer(MainWindow* mw);
	
	template <typename _Tp>
	inline void switch_to();
	
	~switch_performer();

private:
	std::unique_ptr<switcher> sw;
	MainWindow* mw;
};

#endif // PRIVACY_PROTECTION_MESSENGER_QT_MAINWINDOW_H
