#ifndef GAMEMODEL_H
#define GAMEMODEL_H
#include<iostream>
using namespace std;

struct chess
{
    int _row,_col,_score;
    friend bool operator < (chess n1,chess n2)
    {
        return n1._score < n2._score;
    }
};

enum GameType
{
    HillClimb,
    MINIMAX,
    Blind
};

enum GameStatus
{
    PLAYING,//还在游戏
    WIN,//有一方赢了
    DEAD//死局了
};

const int kBoardSizeNum = 15;// 棋盘尺寸
const int gap = 40; // 棋盘边缘空隙
const int R = 15; // 棋子半径
const int kMarkSize = 6; // 落子标记边长
const int block= 40; // 格子的大小
const int clickR = 20; // 鼠标点击的模糊距离上限
const int kAIDelay = 700; // AI下棋的思考时间

class GameModel:public chess
{
public:
    GameModel();

public:
    int chessboard[kBoardSizeNum][kBoardSizeNum]; // 存储当前游戏棋盘和棋子的情况,空白为0，白子1，黑子-1
    int scoreMapVec[kBoardSizeNum][kBoardSizeNum]; // 存储各个点位的评分情况，作为AI下棋依据
    bool who_play; // 标示下棋方
    GameType gameType; // 游戏模式
    GameStatus gameStatus; // 游戏状态

    void startGame(GameType type); // 开始游戏
    void calculateScore(chess &node); // 计算评分
    chess best_first(int row,int col);//爬山计分
    chess minimax(int row,int col,int depth,int player);//minimax计分
    chess blind();//盲搜计分

    void actionByPerson(int row, int col); // 人执行下棋
    void actionByAI_HillClimb(int &clickRow, int &clickCol); // 机器执行下棋
    void actionByAI_MiniMax(int &clickRow, int &clickCol);
    void actionByAI_Blind(int &clickRow, int &clickCol);

    void updateGameMap(int row, int col); // 每次落子后更新游戏棋盘
    bool isWin(int row, int col); // 判断游戏是否胜利
    bool istie(); // 判断是否和棋

    int max(int a,int b){if(a>b)return a;else return b;}
    int min(int a,int b){if(a<b)return a;else return b;}
    int abs(int a){if(a<0)return -a;else return a;}
};

#endif // GAMEMODEL_H
