#ifndef GAMEMAP_H
#define GAMEMAP_H

#include <QObject>
#include <QPainter>
#include <QFile>
#include <QDebug>

enum MapInformation{
    Road,    // 道路：可行走的空地
    Wall,    // 墙壁：不可穿过
    Box,     // 箱子：可推动的物体
    Point,   // 目标点：需要推箱子到达的位置
    InPoint  // 箱子已推入目标点
};

class GameMap : public QObject
{
    Q_OBJECT
public:
    explicit GameMap(QObject *parent = nullptr);

    ~GameMap();

    bool InitMap(QString fileName);
    void Clear();
    void Paint(QPainter* _p, QPoint _Pos);  //绘制地图元素的成员函数
    //根据格子类型（路/墙/箱子/目标点），在指定位置画出对应的图片/方块。

    int mRow;
    int mCol;
    int** mPArr;

signals:

};

#endif // GAMEMAP_H
