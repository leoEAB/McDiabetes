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
        tableViewModel->setQuery("SELECT category, type, name, size, price FROM items ORDER BY name");

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

        tableViewModel->setQuery("select UD.userName, email, firstName, lastName, street, streetNr, city from userInfo as UI RIGHT OUTER JOIN userData as UD ON (UI.userName = UD.userName)");

        table->setModel(tableViewModel);
        table->resizeColumnToContents(1);
        table->resizeColumnToContents(5);
        table->setAlternatingRowColors(true);
    }

    void listMains(QTableView *table) {
        QSqlQuery queryListMains;
        QString foodCategory = "Main";

        queryListMains.prepare("SELECT DISTINCT name FROM items WHERE items.type = :category ");
        queryListMains.bindValue(":category", foodCategory);
        queryListMains.exec();

        QSqlQueryModel *tableViewModel = new QSqlQueryModel;
        tableViewModel->setQuery(queryListMains);

        tableViewModel->setHeaderData(0, Qt::Horizontal, "Name");

        table->setModel(tableViewModel);
        table->resizeColumnsToContents();
        table->setAlternatingRowColors(true);

    }

    void listSides(QTableView *table) {
        QSqlQuery queryListSides;
        QString foodCategory = "Side";

        queryListSides.prepare("SELECT DISTINCT name FROM items WHERE items.type = :category ");
        queryListSides.bindValue(":category", foodCategory);
        queryListSides.exec();

        QSqlQueryModel *tableViewModel = new QSqlQueryModel;
        tableViewModel->setQuery(queryListSides);

        table->setModel(tableViewModel);
        table->resizeColumnsToContents();
        table->setAlternatingRowColors(true);

    }

    void listDrinks(QTableView *table) {
        QSqlQuery queryListDrinks;
        QString foodCategory = "Beverage";

        queryListDrinks.prepare("SELECT DISTINCT name FROM items WHERE items.type = :category ");
        queryListDrinks.bindValue(":category", foodCategory);
        queryListDrinks.exec();

        QSqlQueryModel *tableViewModel = new QSqlQueryModel;
        tableViewModel->setQuery(queryListDrinks);

        table->setModel(tableViewModel);
        table->resizeColumnsToContents();
        table->setAlternatingRowColors(true);

    }

    void listDesserts(QTableView *table) {
        QSqlQuery queryListDesserts;
        QString foodCategory = "Dessert";

        queryListDesserts.prepare("SELECT DISTINCT name FROM items WHERE items.type = :category ");
        queryListDesserts.bindValue(":category", foodCategory);
        queryListDesserts.exec();

        QSqlQueryModel *tableViewModel = new QSqlQueryModel;
        tableViewModel->setQuery(queryListDesserts);

        table->setModel(tableViewModel);
        table->resizeColumnsToContents();
        table->setAlternatingRowColors(true);

    }

    void showItemOptions(QString itemName, QTableView *table) {
        QSqlQuery queryItemOptions;

        queryItemOptions.prepare("SELECT size, price FROM items WHERE items.name = :itemName");
        queryItemOptions.bindValue(":itemName", itemName);
        queryItemOptions.exec();

        QSqlQueryModel *tableViewModel = new QSqlQueryModel;
        tableViewModel->setQuery(queryItemOptions);

        table->setModel(tableViewModel);
        table->resizeColumnsToContents();
        table->setAlternatingRowColors(true);
    }

    void fillCart(QString itemName, QString itemOption, QTableView *table) {

        /*delete cart at first*/


        QSqlQuery queryAddItem;

        queryAddItem.prepare("INSERT INTO userCart(type, name, size, price) SELECT type, name, size, price FROM items where items.name = :itemName and items.size = :itemSize");
        queryAddItem.bindValue(":itemName", itemName);
        queryAddItem.bindValue(":itemSize", itemOption);
        queryAddItem.exec();

        QSqlQuery listCart;
        listCart.prepare("SELECT type, name, size, price FROM userCart");
        listCart.exec();

        QSqlQueryModel *tableViewModel = new QSqlQueryModel;
        tableViewModel->setQuery(listCart);

        table->setModel(tableViewModel);
        table->resizeColumnsToContents();
        table->setAlternatingRowColors(true);
    }

    void clearCart(QTableView *table) {
        QSqlQuery setSafeUpdates;

        setSafeUpdates.prepare("SET SQL_SAFE_UPDATES = 0");
        setSafeUpdates.exec();

        QSqlQuery clearCartQuery;
        clearCartQuery.prepare("delete from usercart");
        clearCartQuery.exec();

        QSqlQueryModel *tableViewModel = new QSqlQueryModel;
        tableViewModel->setQuery(clearCartQuery);

        table->setModel(tableViewModel);
    }



private:

    //create Database
    QSqlDatabase db;

};

#endif // DATABASE_H
