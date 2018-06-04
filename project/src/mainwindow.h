#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

//include for error box at try..catch.. statement
#include <QMessageBox>

#include "database.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool isConnected(); //returns true when DB connection successful

private slots:
    //Previous button
    void on_buttonPrevious_clicked();

    //PAGE 0
    void on_checkBox_stateChanged(int arg1); //checkbox to change database settings
    void on_buttonConnectDB_clicked(); //database connection
    void on_buttonLoginStart_clicked(); //big login button on page 0
    void on_buttonNewUser_clicked(); //big new user button on page 0

    //PAGE 1 - Login screen
    void on_buttonLogin_clicked(); //login button

    //PAGE 2 - Admin screen
    void on_buttonListAllMenu_clicked(); //list all menu items
    void on_buttonListOrders_clicked(); //list all orders so far
    void on_buttonListUsers_clicked(); //list all users
    void on_buttonLogout_clicked(); //logout button on admin screen

    //PAGE 3 - User screen
    void on_buttonNewOrder_clicked(); //new order for user
    void on_buttonShowPrevOrders_clicked(); //show previous orders for this user
    void on_buttonLogout_2_clicked(); //logout button on user screen

    //PAGE 4 - New user
    void on_buttonClearUserFields_clicked(); //clear fields
    void on_buttonSubmitNewUser_clicked(); //submit new user information

private:
    Ui::MainWindow *ui;

    Database *db;

    QTimer *countdownConnect;
};



#endif // MAINWINDOW_H
