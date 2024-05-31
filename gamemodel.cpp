#include <utility>
#include <time.h>
#include "gamemodel.h"
#include<queue>
#include<stdlib.h>
#include<iostream>
#include<cstdlib>
#include<ctime>

using namespace std;
GameModel::GameModel()
{}

void GameModel::startGame(GameType type)
{
    gameType = type;
    for(int i=0;i<kBoardSizeNum;i++)
    {
        for(int j=0;j<kBoardSizeNum;j++)
        {
            chessboard[i][j]=0;
        }
    }
    for(int i=0;i<kBoardSizeNum;i++)
    {
        for(int j=0;j<kBoardSizeNum;j++)
        {
            scoreMapVec[i][j]=0;
        }
    }
    who_play = true;// 己方下为true,对方下位false


}

void GameModel::updateGameMap(int row, int col)
{
    if (who_play)
        chessboard[row][col] = 1;
    else
        chessboard[row][col] = -1;

    who_play = !who_play;//切换
}

void GameModel::actionByPerson(int row, int col)
{
    updateGameMap(row, col);
}

void GameModel::actionByAI_HillClimb(int &clickRow, int &clickCol)
{
    chess n=this->best_first(clickRow,clickCol);
    clickRow = n._row; // 记录落子点，其实修改了
    clickCol = n._col;
    updateGameMap(clickRow, clickCol);
}

void GameModel::actionByAI_MiniMax(int &clickRow, int &clickCol)
{
    chess n=this->minimax(clickRow,clickCol,7,-1);
    clickRow = n._row; // 记录落子点，其实修改了
    clickCol = n._col;
    updateGameMap(clickRow, clickCol);
}

void GameModel::actionByAI_Blind(int &clickRow, int &clickCol){
    chess n=this->blind();
    clickRow = n._row; // 记录落子点，其实修改了
    clickCol = n._col;
    updateGameMap(clickRow, clickCol);
}

void GameModel::calculateScore(chess& node)// 最关键的计算评分函数
{   // 统计玩家或者电脑连成的子
    int personNum = 0; // 玩家连成子的个数
    int botNum = 0; // AI连成子的个数
    int emptyNum = 0; // 各方向空白位的个数
    int row=node._row;
    int col=node._col;

    if(row<0||col<0||row>=kBoardSizeNum||col>=kBoardSizeNum){
        srand((unsigned)time(NULL));
        node._row = rand()%(kBoardSizeNum-1)+1;
        srand((unsigned)time(NULL));
        node._col = rand()%(kBoardSizeNum-1)+1;
    }//避免AI无子可下
    row=node._row;
    col=node._col;
            // 空白点就算
            if (row > 0 && col > 0 &&
                chessboard[row][col] == 0)
            {
                // 遍历周围八个方向
                for (int y = -1; y <= 1; y++)
                    for (int x = -1; x <= 1; x++)
                    {
                        // 重置
                        personNum = 0;
                        botNum = 0;
                        emptyNum = 0;

                        // 原坐标不算
                        if (!(y == 0 && x == 0))
                        {
                            // 每个方向延伸4个子

                            // 对玩家白子评分（正反两个方向）
                            for (int i = 1; i <= 4; i++)
                            {
                                if (row + i * y > 0 && row + i * y < kBoardSizeNum &&
                                    col + i * x > 0 && col + i * x < kBoardSizeNum &&
                                    chessboard[row + i * y][col + i * x] == 1) // 玩家的子
                                {
                                    personNum++;
                                }
                                else if (row + i * y > 0 && row + i * y < kBoardSizeNum &&
                                         col + i * x > 0 && col + i * x < kBoardSizeNum &&
                                         chessboard[row + i * y][col + i * x] == 0) // 空白位
                                {
                                    emptyNum++;
                                    break;
                                }
                                else            // 出边界
                                    break;
                            }

                            for (int i = 1; i <= 4; i++)
                            {
                                if (row - i * y > 0 && row - i * y < kBoardSizeNum &&
                                    col - i * x > 0 && col - i * x < kBoardSizeNum &&
                                    chessboard[row - i * y][col - i * x] == 1) // 玩家的子
                                {
                                    personNum++;
                                }
                                else if (row - i * y > 0 && row - i * y < kBoardSizeNum &&
                                         col - i * x > 0 && col - i * x < kBoardSizeNum &&
                                         chessboard[row - i * y][col - i * x] == 0) // 空白位
                                {
                                    emptyNum++;
                                    break;
                                }
                                else            // 出边界
                                    break;
                            }

                            if (personNum == 1)
                                scoreMapVec[row][col] += 10;
                            else if (personNum == 2)
                            {
                                if (emptyNum == 1)
                                    scoreMapVec[row][col] += 30;
                                else if (emptyNum == 2)
                                    scoreMapVec[row][col] += 40;
                            }
                            else if (personNum == 3)
                            {

                                if (emptyNum == 1)
                                    scoreMapVec[row][col] += 60;
                                else if (emptyNum == 2)
                                    scoreMapVec[row][col] += 110;
                            }
                            else if (personNum == 4)
                                scoreMapVec[row][col] += 10100;

                            // 进行一次清空
                            emptyNum = 0;

                            // 对AI黑子评分
                            for (int i = 1; i <= 4; i++)
                            {
                                if (row + i * y > 0 && row + i * y < kBoardSizeNum &&
                                    col + i * x > 0 && col + i * x < kBoardSizeNum &&
                                    chessboard[row + i * y][col + i * x] == -1) // 玩家的子
                                {
                                    botNum++;
                                }
                                else if (row + i * y > 0 && row + i * y < kBoardSizeNum &&
                                         col + i * x > 0 && col + i * x < kBoardSizeNum &&
                                         chessboard[row +i * y][col + i * x] == 0) // 空白位
                                {
                                    emptyNum++;
                                    break;
                                }
                                else            // 出边界
                                    break;
                            }

                            for (int i = 1; i <= 4; i++)
                            {
                                if (row - i * y > 0 && row - i * y < kBoardSizeNum &&
                                    col - i * x > 0 && col - i * x < kBoardSizeNum &&
                                    chessboard[row - i * y][col - i * x] == -1) // AI的子
                                {
                                    botNum++;
                                }
                                else if (row - i * y > 0 && row - i * y < kBoardSizeNum &&
                                         col - i * x > 0 && col - i * x < kBoardSizeNum &&
                                         chessboard[row - i * y][col - i * x] == 0) // 空白位
                                {
                                    emptyNum++;
                                    break;
                                }
                                else            // 出边界
                                    break;
                            }

                            if (botNum == 0)
                                scoreMapVec[row][col] +=5;
                            else if (botNum == 1)
                                scoreMapVec[row][col] += 10;
                            else if (botNum == 2)
                            {
                                if (emptyNum == 1)
                                    scoreMapVec[row][col] +=25;
                                else if (emptyNum == 2)
                                    scoreMapVec[row][col] += 50;
                            }
                            else if (botNum == 3)
                            {
                                if (emptyNum == 1)
                                    scoreMapVec[row][col] += 55;
                                else if (emptyNum == 2)
                                    scoreMapVec[row][col] += 100;
                            }
                            else if (botNum >= 4)
                                scoreMapVec[row][col] +=10000;

                        }
                    }
              node._score= scoreMapVec[row][col];
              //cout<<node._score;
              return;
            }
           node._score= 0;
}

bool GameModel::isWin(int row, int col)
{
    // 横竖斜四种大情况，每种情况都根据当前落子往后遍历5个棋子，有一种符合就算赢
    // 水平方向
    for (int i = 0; i < 5; i++)
    {
        // 往左5个，往右匹配4个子，20种情况
        if (col - i > 0 &&
            col - i + 4 < kBoardSizeNum &&
            chessboard[row][col - i] == chessboard[row][col - i + 1] &&
            chessboard[row][col - i] == chessboard[row][col - i + 2] &&
            chessboard[row][col - i] == chessboard[row][col - i + 3] &&
            chessboard[row][col - i] == chessboard[row][col - i + 4])
            return true;
    }

    // 竖直方向(上下延伸4个)
    for (int i = 0; i < 5; i++)
    {
        if (row - i > 0 &&
            row - i + 4 < kBoardSizeNum &&
            chessboard[row - i][col] == chessboard[row - i + 1][col] &&
            chessboard[row - i][col] == chessboard[row - i + 2][col] &&
            chessboard[row - i][col] == chessboard[row - i + 3][col] &&
            chessboard[row - i][col] == chessboard[row - i + 4][col])
            return true;
    }

    // 左斜方向
    for (int i = 0; i < 5; i++)
    {
        if (row + i < kBoardSizeNum &&
            row + i - 4 > 0 &&
            col - i > 0 &&
            col - i + 4 < kBoardSizeNum &&
            chessboard[row + i][col - i] == chessboard[row + i - 1][col - i + 1] &&
            chessboard[row + i][col - i] == chessboard[row + i - 2][col - i + 2] &&
            chessboard[row + i][col - i] == chessboard[row + i - 3][col - i + 3] &&
            chessboard[row + i][col - i] == chessboard[row + i - 4][col - i + 4])
            return true;
    }

    // 右斜方向
    for (int i = 0; i < 5; i++)
    {
        if (row - i > 0 &&
            row - i + 4 < kBoardSizeNum &&
            col - i > 0 &&
            col - i + 4 < kBoardSizeNum &&
            chessboard[row - i][col - i] == chessboard[row - i + 1][col - i + 1] &&
            chessboard[row - i][col - i] == chessboard[row - i + 2][col - i + 2] &&
            chessboard[row - i][col - i] == chessboard[row - i + 3][col - i + 3] &&
            chessboard[row - i][col - i] == chessboard[row - i + 4][col - i + 4])
            return true;
    }

    return false;
}

bool GameModel::istie()
{
    // 所有空格全部填满
    for (int i = 1; i < kBoardSizeNum; i++)
        for (int j = 1; j < kBoardSizeNum; j++)
        {
            if (!(chessboard[i][j] == 1 || chessboard[i][j] == -1))
                return false;
        }
    return true;
}

chess GameModel::best_first(int row,int col){//爬山
    priority_queue<chess>  Que;   //从小到大的优先级队列
    chess node;
    node._row=row;node._col=col;
    node._score=0;
    Que.push(node);
    int direction_row[8]={-1,-1,-1,0,+1,+1,+1,0};
    int direction_col[8]={-1,0,+1,+1,+1,0,-1,-1};

    while (!Que.empty())
    {
        chess now = Que.top();
        Que.pop();
        cout<<now._score<<" ";
        int rec_row=now._row;int rec_col=now._col;

        for(int i=0;i<kBoardSizeNum;i++)
        {
            for(int j=0;j<kBoardSizeNum;j++)
            {
                scoreMapVec[i][j]=0;
            }
        }

        for(int i=0;i<8;i++){
            chess child;
            child._row=now._row+direction_row[i];
            child._col=now._col+direction_col[i];
            this->calculateScore(child);
            if(child._score>now._score){
                rec_row=child._row;rec_col=child._col;
            }
            Que.push(child);
        }

        if(rec_row==now._row&&rec_col==now._col)return now;
    }
}

chess GameModel::minimax(int row, int col, int depth, int player){
    if(depth==0){
        // 清空评分数组
        for(int i=0;i<kBoardSizeNum;i++)
        {
            for(int j=0;j<kBoardSizeNum;j++)
            {
                scoreMapVec[i][j]=0;
            }
        }
        chess node;node._row=row;node._col=col;
        chessboard[row][col]=0;
        this->calculateScore(node);
        node._score=-node._score;
        //node._score=-node._score*5;
        //this->countblank(node);
        cout<<node._score<<" ";
        return node;
    }
    cout<<"*";
    int direction_row[8]={-1,-1,-1,0,+1,+1,+1,0};
    int direction_col[8]={-1,0,+1,+1,+1,0,-1,-1};
    int bestvalue;int rec; 
    chess child;

    do{srand((unsigned)time(0));//为了防止继续找并没有更多合适的点，所以初始化一个
        child._row=rand() % (kBoardSizeNum-1)+1;
        srand((unsigned)time(0));
        child._col=rand() % (kBoardSizeNum-1)+1;}
    while(chessboard[child._row][child._col]!=0);

    std::vector<std::pair<int, int > > Points;
    chess imit;
    if(player==1){
        rec=1000000;
        for (int i = 1; i < 8; i++)
            {
                if (row+direction_row[i]>0&&row+direction_row[i]<kBoardSizeNum
                        &&col+direction_col[i]&&col+direction_col[i]<kBoardSizeNum
                        &&chessboard[row+direction_row[i]][col+direction_col[i]]==0){
                    chessboard[row+direction_row[i]][col+direction_col[i]]=1;
                    bestvalue=minimax(row+direction_row[i],col+direction_col[i],depth-1,-player)._score;
                    bestvalue=min(bestvalue,rec);
                    if(rec!=bestvalue){
                        child._row=row+direction_row[i];child._col=col+direction_col[i];
                        child._score=bestvalue;
                    }
                    chessboard[row+direction_row[i]][col+direction_col[i]]=0;
                    if ( bestvalue> rec) //<         // 找最大的数和坐标
                    {
                        Points.clear();
                        rec=bestvalue;
                        Points.push_back(std::make_pair(row+direction_row[i],col+direction_col[i]));
                    }
                    else if (bestvalue==rec)     // 如果有多个最大的数，都存起来
                        Points.push_back(std::make_pair(row+direction_row[i],col+direction_col[i]));
                }
                else continue;
            }

    }

    else if(player==-1){
        rec=-1000000;
        for (int i = 1; i < 8; i++)
            {
                if (row+direction_row[i]>0&&row+direction_row[i]<kBoardSizeNum
                        &&col+direction_col[i]&&col+direction_col[i]<kBoardSizeNum
                        &&chessboard[row+direction_row[i]][col+direction_col[i]]==0){
                    chessboard[row+direction_row[i]][col+direction_col[i]]=-1;
                    bestvalue=minimax(row+direction_row[i],col+direction_col[i],depth-1,-player)._score;
                    bestvalue=max(bestvalue,rec);
                    if(rec!=bestvalue){
                        child._row=row+direction_row[i];child._col=col+direction_col[i];
                        child._score=bestvalue;
                    }
                    chessboard[row+direction_row[i]][col+direction_col[i]]=0;
                    if ( bestvalue>rec)
                    {
                        Points.clear();
                        rec=bestvalue;
                        Points.push_back(std::make_pair(row+direction_row[i],col+direction_col[i]));
                    }
                    else if (bestvalue==rec)
                        Points.push_back(std::make_pair(row+direction_row[i],col+direction_col[i]));
                }
                else continue;
            }

    }
    if(Points.size()!=0){//务必判断如此
    srand((unsigned)time(0));
    int index = rand() % Points.size();
    std::pair<int, int> pointPair = Points.at(index);
    child._row=pointPair.first;child._col=pointPair.second;}
    child._score=-rec;
    return child;
}

chess GameModel::blind(){//盲搜
    // 清空评分数组
    for(int i=0;i<kBoardSizeNum;i++)
    {
        for(int j=0;j<kBoardSizeNum;j++)
        {
            scoreMapVec[i][j]=0;
        }
    }

    int maxScore=0;
    std::vector<std::pair<int, int> > maxPoints;//如果有相同分数的解就都存起来然后随便找一个

    for (int row = 1; row < kBoardSizeNum; row++)
        for (int col = 1; col < kBoardSizeNum; col++){
            chess node;
            node._row=row;node._col=col;
            this->calculateScore(node);
            if(maxScore<scoreMapVec[row][col]&&chessboard[row][col] == 0){
                maxPoints.clear();
                maxScore = scoreMapVec[row][col];
                maxPoints.push_back(std::make_pair(row, col));
            }
            else if (scoreMapVec[row][col] == maxScore)     // 如果有多个最大的数，都存起来
                maxPoints.push_back(std::make_pair(row, col));
        }
    chess aim;
    aim._score=maxScore;
    srand((unsigned)time(0));
    int index = rand() % maxPoints.size();
    std::pair<int, int> pointPair = maxPoints.at(index);
    aim._row = pointPair.first; // 记录落子点，其实修改了
    aim._col = pointPair.second;
    return aim;
}
