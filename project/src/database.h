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
    Database() {
        db = QSqlDatabase::addDatabase("QMYSQL");
    }

    ~Database();

    //funcNameC "C" means - Custom; Qtsql has same function names
    void setHostnameC(QString host) {
        db.setHostName(host);
    }

    void setPortC(QString port) {
        db.setPort(port.toInt());
    }

    void setUserNameC(QString username) {
        db.setUserName(username);
    }

    void setPasswordC(QString password) {
        db.setPassword(password);
    }

    void setDatabaseNameC(QString databaseName) {
        db.setDatabaseName(databaseName);
    }

    bool openC() {
        return db.open();
    }

    void closeC() {
        db.close();
    }

    bool authorizeUser(QString username, QString password) {

            QSqlQuery queryLogin = QSqlQuery(db);

            queryLogin.prepare("SELECT userName, userPassword FROM users WHERE userName = :userName and userPassword = :userPassword");
            queryLogin.bindValue(":userName", username);
            queryLogin.bindValue(":userPassword", password);
            queryLogin.exec();

            while(queryLogin.next()) {
                if(queryLogin.value(0).toString() == username && queryLogin.value(1).toString() == password){
                    return true;
                }
            }
            return false;

    }

    bool isAdmin(QString username){
        QString type = "a";
        bool finalResult;

        QSqlQuery queryAdmin;

        queryAdmin.prepare("SELECT userName, userType FROM users AS u WHERE u.userName = :userName and u.userType = :userType ");
        queryAdmin.bindValue(":userName", username);
        queryAdmin.bindValue(":userType", type);
        queryAdmin.exec();

        while(queryAdmin.next()) {
            if(queryAdmin.value(0).toString() == username && queryAdmin.value(1).toString() == type){
                finalResult = true;
            } else {
                finalResult = false;
            }
        }

        return finalResult;
    }

    void listAllItems(QTableView *table) {

        QSqlQueryModel *tableViewModel = new QSqlQueryModel;
                                   //ALL MENU ITEMS
        tableViewModel->setQuery("SELECT category, type, name, size, price FROM items");

        tableViewModel->setHeaderData(0, Qt::Horizontal, "Category");
        tableViewModel->setHeaderData(1, Qt::Horizontal, "Type");
        tableViewModel->setHeaderData(2, Qt::Horizontal, "Name");
        tableViewModel->setHeaderData(3, Qt::Horizontal, "Size");
        tableViewModel->setHeaderData(4, Qt::Horizontal, "price");

        table->setModel(tableViewModel);
        table->resizeRowsToContents();
        table->resizeRowsToContents();
        table->resizeColumnToContents(2);
        table->setAlternatingRowColors(true);
    }

    void listAllOrders(QTableView *table) {
        QSqlQueryModel *tableViewModel = new QSqlQueryModel;

                                   //ALL ORDERS
        tableViewModel->setQuery("SELECT piece_name, nn, status FROM main WHERE nn !='0' ORDER BY uid");

        tableViewModel->setHeaderData(0, Qt::Horizontal, "Piece Name");
        tableViewModel->setHeaderData(1, Qt::Horizontal, "NN");
        tableViewModel->setHeaderData(2, Qt::Horizontal, "Status");

        table->setModel(tableViewModel);
        table->resizeColumnsToContents();
        table->setAlternatingRowColors(true);
    }

    void listAllUsers(QTableView *table) {
        QSqlQueryModel *tableViewModel = new QSqlQueryModel;

                                   //ALL USERS
        tableViewModel->setQuery("SELECT  FROM main WHERE nn !='0' ORDER BY uid");

        tableViewModel->setHeaderData(0, Qt::Horizontal, "Piece Name");
        tableViewModel->setHeaderData(1, Qt::Horizontal, "NN");
        tableViewModel->setHeaderData(2, Qt::Horizontal, "Status");

        table->setModel(tableViewModel);
        table->resizeColumnsToContents();
        table->setAlternatingRowColors(true);
    }


private:

    //create Database
    QSqlDatabase db;

};

#endif // DATABASE_H
