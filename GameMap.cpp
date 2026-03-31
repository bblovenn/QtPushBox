#include "GameMap.h"

GameMap::GameMap(QObject *parent) : QObject(parent)
{
    mRow = 0;  //无行
    mCol = 0;  //无列
    mPArr = nullptr; //初始化空指针
}

GameMap::~GameMap(){
    Clear();
}

bool GameMap::InitMap(QString fileName)
{

}

void GameMap::Clear()
{
    if(mPArr != nullptr && mRow>0){ //确定数组是否真实存在
        for(int i=0;i<mRow;i++){
            delete[] mPArr[i];
            mPArr[i] = nullptr;
        }
        delete[] mPArr;
        mPArr = nullptr;//主指针置空
    }
    mRow = 0;
    mCol = 0;//行与列置空
}

void GameMap::Paint(QPainter *_p, QPoint _Pos)
{

}
