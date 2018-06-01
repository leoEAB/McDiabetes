#ifndef DATABASE_H
#define DATABASE_H

//mySql includes
#include <QtSql>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

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

private:

    //create Database
    QSqlDatabase db;

};

#endif // DATABASE_H
