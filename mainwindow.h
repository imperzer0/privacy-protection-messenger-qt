#ifndef PRIVACY_PROTECTION_MESSENGER_QT_MAINWINDOW_H
#define PRIVACY_PROTECTION_MESSENGER_QT_MAINWINDOW_H

#include <QMainWindow>
#include <QTranslator>
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
	
	void on_button_already_registered_clicked();
	
	void on_button_not_registered_clicked();
	
	void on_button_sign_in_clicked();
	
	void on_button_log_in_clicked();
	
	void on_button_send_clicked();
	
	void on_search_friends_textChanged(const QString& arg1);
	
	void on_action_Set_server_triggered();
	
	void on_action_Disconnect_from_server_triggered();

private:
	Ui::MainWindow* ui;
	QTranslator* m_translator = nullptr;
	call_backend* backend = nullptr;
	QString server_address;
	
	void set_address_in_menu_bar();
	
	inline bool check_limits(const QString& login, const QString& password);
};

#endif // PRIVACY_PROTECTION_MESSENGER_QT_MAINWINDOW_H
