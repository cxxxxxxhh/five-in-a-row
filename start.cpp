#include "start.h"
#include "ui_start.h"
#include <QPainter>
#include <QProgressDialog>
#include <QPushButton>
#include "mainwindow.h"

Start::Start(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Start)
{
    ui->setupUi(this);
}

Start::~Start()
{
    delete ui;
}

void Start::paintEvent(QPaintEvent *)
{
    QPixmap background(":/start.jpg");
    QPainter painter(this);
    painter.drawPixmap(0,0,background);
}

void Start::on_pushButton_clicked()
{
    QProgressDialog pd(tr("Loading"),tr("Cancle"),0,50000,this);
       pd.setWindowTitle(tr("Game"));
       pd.setWindowModality(Qt::WindowModal);
       pd.show();
       for(int i = 0;i < 50000;++i)
       {
           pd.setValue(i);
           QCoreApplication::processEvents();
           if(pd.wasCanceled()) break;
       }
       pd.setValue(50000);
       accept();
       MW = new MainWindow;
       MW->show();
}
