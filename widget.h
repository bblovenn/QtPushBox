#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QMessageBox>
#include <QKeyEvent>
#include <QtDebug>
#include "GameMap.h"
#include "Role.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);

    virtual void paintEvent(QPaintEvent* event);//绘图事件函数
    virtual void keyPressEvent(QKeyEvent *event);//键盘按下函数
    void CollisionCheck(int dRow,int dCol); //碰撞检测函数

    ~Widget();

private:
    Ui::Widget *ui;

    GameMap* mGM;
    Role* mRole;
    //游戏更新定时器
    QTimer* mTimer;


};
#endif // WIDGET_H
