#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    mGM = new GameMap(this);

    // 加载地图后才能获取玩家出生位置
    //可选：弹出文件对话框让用户手动选择地图文件
    //QString fileName = QFileDialog::getOpenFileName(this,"打开地图","./","*.txt");
    if(!mGM->InitMap(":/1.txt")){
        QMessageBox::warning(this,"警告","文件打开失败");
    }

    // 根据地图文件中的玩家位置创建角色（注释掉原来的错误出生点）
    // mRole = new Role(this);  // 原来的错误方式
    mRole = new Role(mGM->mPlayerRow, mGM->mPlayerCol, this);

    //定时调用更新函数
    mTimer = new QTimer(this);
    mTimer->start(100);

    connect(mTimer,&QTimer::timeout,[=]()
    {
        this->update();
    });

    //固定窗口大小
    setFixedSize(1024,768);

}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // 画背景
    QImage bgImg(":/road.png");
    if (!bgImg.isNull()) {
        painter.drawImage(QRect(0,0,1024,768), bgImg);
    }
    else {
        qDebug() << "背景图片加载失败";
    }
    // 画地图
    if (mGM) {
         mGM->Paint(&painter, QPoint(10,200));
    }
    // 画人物
    if (mRole) {
        mRole->Paint(&painter, QPoint(10,200));
    }
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    // 空指针保护
    if (!mGM || !mRole) return;
    switch(event->key())
    {
    case Qt::Key_W:
    case Qt::Key_Up:
    {
        CollisionCheck(-1,0);
        break;
    }

    case Qt::Key_S:
    case Qt::Key_Down:
    {
        CollisionCheck(1,0);
        break;
    }

    case Qt::Key_A:
    case Qt::Key_Left:
    {
        CollisionCheck(0,-1);
        break;
    }

    case Qt::Key_D:
    case Qt::Key_Right:
    {
        CollisionCheck(0,1);
        break;
    }
    }
}

void Widget::CollisionCheck(int dRow, int dCol)
{
    //空指针保护
    if (!mGM || mGM->mPArr == nullptr || mGM->mRow == 0 || mGM->mCol == 0) return;

    //mRole:指向玩家/角色对象的指针
    int newRow = mRole->mRow + dRow;
    int newCol = mRole->mCol + dCol;

    // 新增：打印移动信息，方便调试
    qDebug() << "尝试移动：从(" << mRole->mRow << "," << mRole->mCol << ")到(" << newRow << "," << newCol << ")，地形值：" << mGM->mPArr[newRow][newCol];

    //边界检查（核心：避免数组越界）
    if (newRow < 0 || newRow >= mGM->mRow || newCol < 0 || newCol >= mGM->mCol) {
       qDebug() << "超出地图边界";
           return;
    }

//    //判断前面的是不是墙
//    if(mGM->mPArr[newRow+dRow][newCol+dCol] == Wall){
//        qDebug()<<"前方的道路无法通行";
//        return;
//    }
//    //判断前面的是不是路
//    else if(mGM->mPArr[newRow+dRow][newCol+dCol] == Road){
//        mGM->mPArr[newRow+dRow][newCol+dCol] = Box;
//        mGM->mPArr[newRow][newCol] = Road;
//    }
//    //判断是否到达目标点
//    else if(mGM->mPArr[newRow+dRow][newCol+dCol] == Point){
//        mGM->mPArr[newRow+dRow][newCol+dCol] = InPoint;
//        mGM->mPArr[newRow][newCol] = Road;
//    }
//    else{
//        return;
//    }

    //碰撞检测
    int cellType = mGM->mPArr[newRow][newCol];
        switch (cellType) {
            case Wall: // 撞墙，无法移动
                qDebug() << "前方是墙，无法移动";
                return;

            case Road: // 空地，直接移动
                mRole->Move(dRow, dCol);
                break;

            case Point: // 目标点，直接移动
                mRole->Move(dRow, dCol);
                break;

            case Box: // 箱子，检查是否能推动
            case InPoint: { // 箱子在目标点上
                // 计算箱子被推动后的位置
                int boxNewRow = newRow + dRow;
                int boxNewCol = newCol + dCol;
                // 箱子边界检查
                if (boxNewRow < 0 || boxNewRow >= mGM->mRow || boxNewCol < 0 || boxNewCol >= mGM->mCol) {
                    qDebug() << "箱子超出边界，无法推动";
                    return;
                }
                // 箱子前方是否是可推动的位置（路/目标点）
                int boxCellType = mGM->mPArr[boxNewRow][boxNewCol];
                if (boxCellType == Road) {
                    // 推动箱子到空地
                    mGM->mPArr[boxNewRow][boxNewCol] = Box;
                    mGM->mPArr[newRow][newCol] = (cellType == InPoint) ? Point : Road;
                    mRole->Move(dRow, dCol);
                } else if (boxCellType == Point) {
                    // 推动箱子到目标点
                    mGM->mPArr[boxNewRow][boxNewCol] = InPoint;
                    mGM->mPArr[newRow][newCol] = (cellType == InPoint) ? Point : Road;
                    mRole->Move(dRow, dCol);
                } else {
                    qDebug() << "箱子前方有障碍物，无法推动";
                    return;
                }
                break;
            }

            default:
                qDebug() << "未知地形，无法移动";
                return;
        }
    qDebug() << "人物当前位置：(" << mRole->mRow << "," << mRole->mCol << ")";
}

Widget::~Widget()
{
    delete ui;
}

