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
	
	void on_search_friends_textChanged(const QString&);
	
	void on_action_Set_server_triggered();
	
	void on_action_Disconnect_from_server_triggered();
	
	void on_action_Logout_triggered();
	
	void line_message_height_changed(const QSizeF& new_size);
	
	void on_friends_list_widget_itemActivated(QListWidgetItem* item);

private:
	Ui::MainWindow* ui;
	std::unique_ptr<QTranslator> m_translator = nullptr;
	std::unique_ptr<call_backend> backend = nullptr;
	QString server_address = "127.0.0.1";
	
	void refresh_address_indicators();
	
	inline bool assert_data(const QString& login, const QString& password);
	
	void switch_to_sign_up();
	
	void switch_to_log_in();
	
	void switch_to_messaging();
	
	void remove_all_attributes(QDomElement& node);
	
	void insert_mine_message_into_history(const std::string& msg);
	
	void insert_extraneous_message_into_history(const std::string& msg, const std::string& username);
	
	void insert_message_into_history(const std::string& msg, const std::string& username, const std::string& border_color, const std::string& align);
};

#endif // PRIVACY_PROTECTION_MESSENGER_QT_MAINWINDOW_H
