#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "GameModel.h"
#include "ui_mainwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);// 监听鼠标移动情况，方便落子
    void mouseReleaseEvent(QMouseEvent *event);// 实际落子

private:
    Ui::MainWindow *ui;
    GameModel *game; // 游戏指针
    GameType game_type; // 存储游戏类型
    int clickPosRow, clickPosCol; // 存储将点击的位置
    void startGame();
    void checkGame(int y, int x);

private slots:
    void chessOneByPerson(); // 人执行
    void chessHillClimb(); // AI下棋
    void chessMiniMax();
    void chessBlind();
    void gameHillClimb();
    void gameMiniMax();
    void gameBlind();
};


#endif // MAINWINDOW_H
