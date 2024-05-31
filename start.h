#ifndef START_H
#define START_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class Start;
}

class Start : public QDialog
{
    Q_OBJECT

public:
    explicit Start(QWidget *parent = nullptr);
    ~Start();
    void paintEvent(QPaintEvent *);

private slots:
    void on_pushButton_clicked();

private:
    Ui::Start *ui;
    MainWindow *MW;
};

#endif // START_H
