#ifndef DATABASE_H
#define DATABASE_H

//mySql includes
#include <QtSql>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

//show tables
#include <QTableView>

//throw messageBox if login is unsuccessful
#include <QMessageBox>

class Database
{
public:
    Database();

    ~Database();

    //funcNameC "C" means - Custom; Qtsql has same function names
    inline void setHostnameC(QString host) {
        db.setHostName(host);
    }

    inline void setPortC(QString port) {
        db.setPort(port.toInt());
    }

    inline void setUserNameC(QString username) {
        db.setUserName(username);
    }

    inline void setPasswordC(QString password) {
        db.setPassword(password);
    }

    inline void setDatabaseNameC(QString databaseName) {
        db.setDatabaseName(databaseName);
    }

    inline bool openC() {
        return db.open();
    }

    inline void closeC() {
        db.close();
    }

//LOGIN FUNCTIONS -------------------------------------------------------------------------
    bool authorizeUser(QString username, QString password);
    bool isAdmin(QString username);
//------------------------------------------------------------------------------------------



//USER FUNCTIONS ---------------------------------------------------------------------
    void listAllOrdersUser(QString username, QTableView *table);
    void showOrderContents(QDateTime orderDate, QTableView *table);
    void listMains(QTableView *table, int hour);
    void listSides(QTableView *table, int hour);
    void listDrinks(QTableView *table, int hour);
    void listDesserts(QTableView *table, int hour);
    void showItemOptions(QString itemName, QTableView *table);
    bool fillCart(QString username, QString itemName, QString itemOption, QTableView *table);
    void clearCart(QString username, QTableView *table);
    void showFinalOrder(QString username, QTableView *table);
    bool completeOrder(QString username);
    QString getStringFromQuery(QString username, QString what);
//---------------------------------------------------------------------------------------------

//ADMIN FUNCTIONALITIES -----------------------------------------------------------------------
    void listAllUsers(QTableView *table);
    void listAllItems(QTableView *table);
    void listAllOrders(QTableView *table);
    bool updateUserData(QString userName,
                             QString firstName,
                             QString lastName,
                             QString street,
                             int streetNr,
                             QString city,
                             QString plz,
                             QString email);
    bool deleteUser(QString username);
    bool deleteSelectedMenuItem(QString name, QString size);
    void addNewItem(QString itemCategory,
                    QString itemType,
                    QString itemName,
                    QString itemSize,
                    double itemPrice);

//---------------------------------------------------------------------------------------------

//NEW USER PAGE -------------------------------------------------------------------------
    bool newUser(QString username, QString password,
                 QString firstName, QString lastName,
                 QString email, QString street,
                 int streetNumber, QString city,
                 QString cityPlz);

//-----------------------------------------------------------------

private:

    //create Database
    QSqlDatabase db;

};

#endif // DATABASE_H
