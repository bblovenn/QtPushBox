#include "Role.h"

Role::Role(QObject *parent) : QObject(parent)
{
    // 原来的错误出生点（硬编码在墙上）
    // mRow = 1;
    // mCol = 0;
    mRow = 1;
    mCol = 1;
    mImg = QImage("://people.png");
    UpdatePaintPos();
}

// 新增：带参数的构造函数，从 GameMap 获取正确的出生点
Role::Role(int startRow, int startCol, QObject *parent) : QObject(parent)
{
    mRow = startRow;
    mCol = startCol;
    mImg = QImage("://people.png");
    UpdatePaintPos();
}

//单独更新绘制坐标的函数
void Role::UpdatePaintPos()
{
    if (mImg.isNull()) return;
    mPaintPos = QPoint(mCol * mImg.width(), mRow * mImg.height());

    //显示位置
    //mPaintPos = QPoint(mCol * mImg.width(), mRow * mImg.height());
    //逻辑坐标 (mRow, mCol)          像素坐标 (mPaintPos)
    //┌─────────────────┐            ┌─────────────────────────┐
    //│   (0,0) (0,1)   │            │   (0,0)   (32,0)        │
    //│   (1,0) (1,1)   │   转换      │   (0,32)  (32,32)       │
    //│   (2,0) (2,1)   │  ──────→   │   (0,64)  (32,64)       │
    //└─────────────────┘            └─────────────────────────┘
    //   行×列 网格                      像素位置 (假设 32×32)

    // 计算示例：mRow=1, mCol=2, img=32×32
    //mPaintPos.x() = 2 * 32 = 64
    //mPaintPos.y() = 1 * 32 = 32

}


void Role::Move(int dRow, int dCol)
{
    mRow += dRow;
    mCol += dCol;

    UpdatePaintPos();
}

void Role::Paint(QPainter *p, QPoint pos)
{
    if (mImg.isNull() || !p) return;
    p->drawImage(mPaintPos + pos,mImg);
}

