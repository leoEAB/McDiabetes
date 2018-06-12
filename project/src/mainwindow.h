#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "database.h"

#include <QMainWindow>

//include for error box at try..catch.. statement
#include <QMessageBox>

//for the current time
#include <QDateTime>



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    //variable to store index of item that has been clicked on in tableViewMain/Side/etc..
    static QString itemRowString;

    //variable to store item options index (for qtableview)
    static QString itemOptionsRowString;

    //current logged in user
    static QString currentUser;

    //variable to store the current order summary choice
    static QString orderInfoString;

    //variable to store selected user info in the admin screen (for editing their info)
    static QString selectedUser;
    static QString selectedUserUserName;
    static QString selectedUserFirstName;
    static QString selectedUserLastName;
    static QString selectedUserStreet;
    static QString selectedUserStreetNr;
    static QString selectedUserCity;
    static QString selectedUserPlz;
    static QString selectedUserEmail;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool isConnected(); //returns true when DB connection successful

private slots:
    //Previous button
    void on_buttonPrevious_clicked();
    //timeFunction
    void updateTimeLabel();

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
    //global logout func
    void on_buttonLogout_clicked(); //logout button on admin screen

    //PAGE 3 - User screen
    void on_buttonNewOrder_clicked(); //new order for user
    void on_buttonShowPrevOrders_clicked(); //show previous orders for this user
    void on_buttonUserInfo_clicked(); //user info + editing

    //PAGE 4 - New user
    void clearNewUserInput(); //clears all the fields
    void on_buttonClearUserFields_clicked(); //clear fields
    void on_buttonSubmitNewUser_clicked(); //submit new user information


    void on_checkBoxEditUserInfo_stateChanged(int arg1);

    void on_checkBoxSetTime_stateChanged(int arg1);

    void on_tableViewAllMains_clicked(const QModelIndex &index);

    void on_tableViewAllSides_clicked(const QModelIndex &index);

    void on_tableViewAllDrinks_clicked(const QModelIndex &index);

    void on_tableViewAllDesserts_clicked(const QModelIndex &index);

    void on_tableViewItemOptions_clicked(const QModelIndex &index);

    void on_buttonAddToCart_clicked();

    void on_buttonClearCart_clicked();


    void on_buttonToLoginNewUserSuccess_clicked();

    void on_buttonToCheckout_clicked();

    void on_buttonCancelOrder_clicked();

    void on_buttonConfirmOrder_clicked();

    void on_tableViewPrevOrderTimestamps_clicked(const QModelIndex &index);

    void on_tableAllOrdersTimestampAdmin_clicked(const QModelIndex &index);

    void on_buttonEditSelectedUser_clicked();

    void on_tableAllUsersAdmin_clicked(const QModelIndex &index);

    void on_buttonSubmitChangesAdmin_clicked();

    void on_buttonCancelChangesAdmin_clicked();

    void on_buttonDeleteSelectedUser_clicked();

    void on_buttonCancelInfoChangeUser_clicked();

    void on_buttonSubmitInfoChangeUser_clicked();

private:
    Ui::MainWindow *ui;

    Database *db;

    QTimer *countdownConnect;
};



#endif // MAINWINDOW_H
