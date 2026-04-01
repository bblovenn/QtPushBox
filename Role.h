#ifndef ROLE_H
#define ROLE_H

#include <QObject>
#include <QPoint>
#include <QImage>
#include <QPainter>

class Role : public QObject
{
    Q_OBJECT
public:
    explicit Role(QObject *parent = nullptr);

    int mRow;
    int mCol;
\
    // 作用：记录绘制时的屏幕像素坐标
    // 类型：QPoint = (x, y) 二维点
    // 与 mRow/mCol 的关系：
    // mPaintPos.x() = 起点 X + mCol * 瓦片宽度
    // mPaintPos.y() = 起点 Y + mRow * 瓦片高度
    //┌─────────────────────────────────┐
    //│          屏幕/Widget             │
    //│                                 │
    //│   mPaintPos → ● (100, 50)       │
    //│                ┌──────┐         │
    //│                │ 元素  │ 32×32  │
    //│                └──────┘         │
    //│                                 │
    //│   mRow=1, mCol=2 (逻辑坐标)     │
    //└─────────────────────────────────┘
    QPoint mPaintPos;
    QImage mImg;

    // 作用：移动元素位置
    // 参数：
    //   dRow: 行方向偏移量（+1 向下，-1 向上）
    //   dCol: 列方向偏移量（+1 向右，-1 向左）
    //移动方向示意：
    //         dRow=-1
    //           ↑
    //           │
    //dCol=-1 ←  ●  → dCol=+1
    //           │
    //           ↓
    //         dRow=+1

    // 示例调用：
    //Move(0, 1);   // 向右移动 1 格
    //Move(-1, 0);  // 向上移动 1 格
    //Move(1, 1);   // 向右下移动 1 格
    void Move(int dRow,int dCol);

    // 作用：绘制元素到画布
    // 参数：
    //   p:   画笔指针（负责绘制操作）
    //   pos: 绘制基准位置（通常是地图起点）
    //人在二维数组里的x,y和显示的x,y坐标轴是反过来的
    void Paint(QPainter* p,QPoint pos);

    //绘制流程：
    //┌─────────────────────────────────────────┐
    //│  1. 接收画笔 p 和基准位置 pos              │
    //│  2. 根据 mRow/mCol 计算像素坐标            │
    //│  3. 调用 p->drawImage() 绘制 mImg         │
    //│                                         │
    //│  pos → ●                                │
    //│         ┌─────┬─────┬─────┐             │
    //│         │     │     │     │             │
    //│         ├─────┼─────┼─────┤             │
    //│         │     │ ●   │     │ ← mRow=1    │
    //│         │     │mCol=1   │     │   mCol=1│
    //│         └─────┴─────┴─────┘             │
    //│               绘制 mImg                  │
    //└─────────────────────────────────────────┘

    //单独更新绘制坐标的函数
    void UpdatePaintPos();

signals:

};

#endif // ROLE_H
