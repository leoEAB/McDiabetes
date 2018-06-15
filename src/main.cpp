#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QTimer>

#include "database.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap(":/images/img/logo.png"));
    splash->show();

    MainWindow w;
    w.setWindowIcon(QIcon(":images/img/logoSmallest.png"));

    QTimer::singleShot(2500, splash, SLOT(close()));
    QTimer::singleShot(2500, &w, SLOT(show()));

    return a.exec();
}
