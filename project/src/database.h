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

    bool authorizeUser(QString username, QString password) {

            QSqlQuery queryUser;
            QSqlQuery queryPassword;

            queryUser.prepare(QString("select userName from users where userName = :userName"));
            queryUser.bindValue(":userName", username);

            queryPassword.prepare(QString("select userPassword from users where userPassword = :userPassword"));
            queryPassword.bindValue(":userPassword", password);

            if(queryPassword.exec() && queryUser.exec()) {
                return true;
            }
            return false;
    }

    bool isAdmin(QString username){
        /*
        QSqlQuery queryAdmin;
        queryAdmin.prepare();
        queryAdmin.bindValue();
        */
        return false;
    }

    void listAllItems(QTableView *table) {

        QSqlQueryModel *tableViewModel = new QSqlQueryModel;
                                   //ALL MENU ITEMS
        tableViewModel->setQuery("SELECT piece_name, nn, status FROM main WHERE nn !='0' ORDER BY uid");

        tableViewModel->setHeaderData(0, Qt::Horizontal, "Piece Name");
        tableViewModel->setHeaderData(1, Qt::Horizontal, "NN");
        tableViewModel->setHeaderData(2, Qt::Horizontal, "Status");

        table->setModel(tableViewModel);
        table->resizeColumnsToContents();
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
        tableViewModel->setQuery("SELECT piece_name, nn, status FROM main WHERE nn !='0' ORDER BY uid");

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
