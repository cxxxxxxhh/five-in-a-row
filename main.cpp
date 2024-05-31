#include "mainwindow.h"
#include <QApplication>
#include "start.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   /* MainWindow w;
    w.show();*/
    Start st;
    st.show();

    return a.exec();
}
