#include <QPainter>
#include <QTimer>
#include <QMouseEvent>
#include <QMessageBox>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <math.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gamemodel.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(800, 800);// 设置棋盘大小
    setMouseTracking(true);

    // 添加菜单
    QMenu *gameMenu = menuBar()->addMenu(tr("WUZIQI")); // menuBar默认是存在的，直接加菜单就可以了

    QAction *actionPVE_Blind = new QAction("Pattern <Blind>", this);
    connect(actionPVE_Blind, SIGNAL(triggered()), this, SLOT(gameBlind()));
    gameMenu->addAction(actionPVE_Blind);

    QAction *actionPVE_HillClimb = new QAction("Pattern <HillClimb>", this);
    connect(actionPVE_HillClimb, SIGNAL(triggered()), this, SLOT(gameHillClimb()));
    gameMenu->addAction(actionPVE_HillClimb);

    QAction *actionPVE_MiniMax = new QAction("Pattern <MiniMax>", this);
    connect(actionPVE_MiniMax, SIGNAL(triggered()), this, SLOT(gameMiniMax()));
    gameMenu->addAction(actionPVE_MiniMax);

    // 开始游戏
    startGame();
}

MainWindow::~MainWindow()
{
    if (game)
    {
        delete game;
    }
}

void MainWindow::startGame()
{
    game = new GameModel;
    gameBlind();// 初始化游戏模型
}

void MainWindow::gameHillClimb()
{
    game_type = HillClimb;
    game->gameStatus = PLAYING;
    game->startGame(game_type);
    update();
}

void MainWindow::gameMiniMax()
{
    game_type = MINIMAX;
    game->gameStatus = PLAYING;
    game->startGame(game_type);
    update();
}

void MainWindow::gameBlind()
{
    game_type = Blind;
    game->gameStatus = PLAYING;
    game->startGame(game_type);
    update();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);// 绘制棋盘
    painter.setRenderHint(QPainter::Antialiasing, true); // 抗锯齿

    QPixmap background(":/map.jpg");
    painter.drawPixmap(0,0,background);

    QBrush brush;
    brush.setColor(QColor("#FFE4B5"));//设置背景颜色
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    painter.drawRect(40,40,600,600);//背景框大小

    QPen pen = painter.pen();
    pen.setColor(QColor("#D2B48C"));//设置边框颜色
    pen.setWidth(15);//设置边框宽度
    painter.setPen(pen);

    pen.setColor(Qt::black);//绘制棋盘
    pen.setWidth(2);
    painter.setPen(pen);
    for(int i=0;i<=15;i++)
    {
        painter.drawLine(40+i*40,40,40+i*40,640);
        painter.drawLine(40,40+i*40,640,40+i*40);
    }

    QBrush brushh;
    brushh.setStyle(Qt::SolidPattern);// 绘制落子标记(防止鼠标出框越界)
    if (clickPosRow > 0 && clickPosRow < kBoardSizeNum && clickPosCol > 0 && clickPosCol < kBoardSizeNum && game->chessboard[clickPosRow][clickPosCol] == 0)
    {
        if (game->who_play)
            brush.setColor(Qt::black);
        else
            brush.setColor(Qt::white);
        painter.setBrush(brush);
        painter.drawRect(gap + block* clickPosCol - kMarkSize / 2, gap + block* clickPosRow - kMarkSize / 2, kMarkSize, kMarkSize);
    }

    brush.setColor(Qt::black);//绘制五个黑点
    painter.setBrush(brush);
    painter.drawRect(155,155,10,10);
    painter.drawRect(475,155,10,10);
    painter.drawRect(155,475,10,10);
    painter.drawRect(475,475,10,10);
    painter.drawRect(315,315,10,10);

    // 绘制棋子
    for (int i = 0; i < kBoardSizeNum; i++)
        for (int j = 0; j < kBoardSizeNum; j++)
        {
            if (game->chessboard[i][j] == -1)//默认-1为白棋子
            {
                brush.setColor(Qt::white);
                painter.setPen(Qt::NoPen);
                painter.setBrush(brush);
                painter.drawEllipse(gap + block * j - R, gap + block * i - R, R*2, R*2);
            }
            else if (game->chessboard[i][j] == 1)
            {
                brush.setColor(Qt::black);
                painter.setBrush(brush);
                painter.drawEllipse(gap + block * j - R, gap + block * i - R, R*2, R*2);
            }
        }

    // 判断输赢
    if (clickPosRow > 0 && clickPosRow < kBoardSizeNum && clickPosCol > 0 && clickPosCol < kBoardSizeNum && (game->chessboard[clickPosRow][clickPosCol] == 1 || game->chessboard[clickPosRow][clickPosCol] == -1))
    {
        if (game->isWin(clickPosRow, clickPosCol) && game->gameStatus == PLAYING)
        {
            game->gameStatus = WIN;
            QString str;
            if (game->chessboard[clickPosRow][clickPosCol] == -1)
                str = "white player";
            else if (game->chessboard[clickPosRow][clickPosCol] == 1)
                str = "black player";
            QMessageBox::StandardButton btnValue = QMessageBox::information(this, "congratulations", str + "win!");
            if (btnValue == QMessageBox::Ok)// 重置游戏状态，否则容易死循环
            {
                game->startGame(game_type);
                game->gameStatus = PLAYING;
            }
        }
    }

    if (game->istie())//是否平局
    {
        QMessageBox::StandardButton btnValue = QMessageBox::information(this, "oh", "it's a tie!");
        if (btnValue == QMessageBox::Ok)
        {
            game->startGame(game_type);
            game->gameStatus = PLAYING;
        }

    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    int x = event->x();
    int y = event->y();//通过鼠标确定落子位置

    // 棋盘边缘不能落子
    if (x >= gap + block / 2 && x < size().width() - gap && y >= gap + block/ 2 && y < size().height()- gap)
    {
        clickPosRow = -1; //进行初始化
        clickPosCol = -1;

        int col = x / block;//求出是要下在哪一个格子
        int row = y / block;
        int leftTopPosX = gap + block* col;//求要落子的点的坐标
        int leftTopPosY = gap + block* row;

        int len_a = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY));
        int len_b = sqrt((x - leftTopPosX - block) * (x - leftTopPosX - block) + (y - leftTopPosY) * (y - leftTopPosY));
        int len_c = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY - block) * (y - leftTopPosY - block));
        int len_d = sqrt((x - leftTopPosX - block) * (x - leftTopPosX -block) + (y - leftTopPosY - block) * (y - leftTopPosY - block));

        if (len_a < clickR)//左上
        {
            clickPosRow = row;
            clickPosCol = col;
        }
        if (len_b < clickR)//右上
        {
            clickPosRow = row;
            clickPosCol = col + 1;
        }
        if (len_c < clickR)//左下
        {
            clickPosRow = row + 1;
            clickPosCol = col;
        }
        if (len_d < clickR)//右下
        {
            clickPosRow = row + 1;
            clickPosCol = col + 1;
        }
    }
    update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    // 人下棋，并且不能抢机器的棋
    if ((!(game_type == HillClimb && !game->who_play)) &&(!(game_type == MINIMAX && !game->who_play)) &&(!(game_type == Blind && !game->who_play)) )
    {
        chessOneByPerson();
        // 如果是人机模式，需要调用AI下棋
        if (game->gameType == HillClimb && !game->who_play)
        {
            // 用定时器做一个延迟
            QTimer::singleShot(kAIDelay, this, SLOT(chessHillClimb()));
        }
        else if (game->gameType == MINIMAX && !game->who_play)
        {
            QTimer::singleShot(kAIDelay, this, SLOT(chessMiniMax()));
        }
        else if (game->gameType == Blind && !game->who_play)
        {
            QTimer::singleShot(kAIDelay, this, SLOT(chessBlind()));
        }
    }

}

void MainWindow::chessOneByPerson()// 根据当前存储的坐标下子,只有有效点击才下子，并且该处没有子
{
    if (clickPosRow != -1 && clickPosCol != -1 && game->chessboard[clickPosRow][clickPosCol] == 0)
    {
        game->actionByPerson(clickPosRow, clickPosCol);
        update();// 重绘
    }
}

void MainWindow::chessHillClimb()
{
    game->actionByAI_HillClimb(clickPosRow, clickPosCol);
    update();
}

void MainWindow::chessMiniMax()
{
    game->actionByAI_MiniMax(clickPosRow, clickPosCol);
    update();
}

void MainWindow::chessBlind()
{
    game->actionByAI_Blind(clickPosRow, clickPosCol);
    update();
}



