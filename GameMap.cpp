#include "GameMap.h"

GameMap::GameMap(QObject *parent) : QObject(parent)
{
    mRow = 0;  //无行
    mCol = 0;  //无列
    mPArr = nullptr; //初始化空指针
    mPlayerRow = 1; // 默认玩家出生行
    mPlayerCol = 1; // 默认玩家出生列
}

GameMap::~GameMap(){
    Clear();
}

bool GameMap::InitMap(QString fileName)
{
//    //判断是否可以打开文件
//    QFile file(fileName);
//    if(!file.open(QIODevice::ReadOnly)){
//        qDebug()<<"地图文件打开失败";
//        return false;
//    }
//    QByteArray arrAll = file.readAll(); //读取文件
//    arrAll.replace("\r\n","\n");  //统一换行符,减少不同平台带来的文件读取影响
//    QList<QByteArray> lineList = arrAll.split('\n'); //按 "\n" 分割成多行，每行是一个 QByteArray
//    mRow = lineList.size();  //列表长度就是行数
//    mPArr = new int*[mRow];  //为二维 int 数组分配「行指针数组」（后续还需为每行分配列空间）

//    for(int i = 0;i < mRow;i++){
//        QList<QByteArray> colList = lineList[i].split(',');//按逗号分割当前行，提取每个单元格的数据
//        mCol = colList.size(); //取当前行的列数
//        mPArr[i] = new int[mCol]; //为二维数组的第 i 行分配内存空间（大小为列数）

//        for(int j=0;j<mCol;j++){
//            mPArr[i][j] = colList[j].toInt(); //将字节数组转换为整数，并存储到二维数组对应位置
//            // 作用：将第 i 行、第 j 列的字节数组（如 "42"）解析为 int 整数，并赋值给二维数组对应位置
//        }
//    }
//}

    // 1. 空文件名检查
        if (fileName.isEmpty()) {
            qDebug() << "地图文件名不能为空";
            return false;
        }

        // 2. 打开文件（调整return位置，让调试信息输出）
        QFile file(fileName);
        if(!file.open(QIODevice::ReadOnly)){
            qDebug()<<"地图文件打开失败：" << file.errorString() << "，路径：" << fileName;
            return false; // 移到调试信息后
        }

        // 3. 读取文件并处理换行符
        QByteArray arrAll = file.readAll();
        arrAll.replace("\r\n","\n");
        QList<QByteArray> lineList = arrAll.split('\n');

        // 4. 过滤空行（避免地图文件最后一行空行导致的错误）
        QList<QByteArray> validLines;
        for (const QByteArray& line : lineList) {
            QByteArray trimmed = line.trimmed();
            if (!trimmed.isEmpty() && !trimmed.startsWith('#')) { // 跳过注释行和空行
                validLines.append(trimmed);
            }
        }
        if (validLines.isEmpty()) {
            qDebug() << "地图文件无有效内容";
            file.close();
            return false;
        }

        // 5. 初始化二维数组
        mRow = validLines.size();
        mPArr = new int*[mRow];
        for(int i = 0;i < mRow;i++){
            QList<QByteArray> colList = validLines[i].split(',');
            mCol = colList.size();
            mPArr[i] = new int[mCol];

            for(int j=0;j<mCol;j++){
                mPArr[i][j] = colList[j].toInt();
            }
        }

        // 6. 扫描玩家位置（值为4）
        mPlayerRow = 1;
        mPlayerCol = 1;
        bool foundPlayer = false;
        for(int i = 0;i < mRow && !foundPlayer;i++){
            for(int j = 0;j < mCol && !foundPlayer;j++){
                if(mPArr[i][j] == 4){
                    mPlayerRow = i;
                    mPlayerCol = j;
                    mPArr[i][j] = Road; // 将玩家位置替换为道路
                    foundPlayer = true;
                }
            }
        }

        file.close(); // 关闭文件
        return true; // 补充正常执行的返回值
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

void GameMap::Paint(QPainter *p, QPoint Pos)
{

    // 空指针检查，避免崩溃
    if (mPArr == nullptr || mRow == 0 || mCol == 0) return;

    for(int i = 0;i<mRow;i++){
        for(int j=0;j<mCol;j++){
            QString imgName;
            switch(mPArr[i][j]){
                case Road:
                    imgName = "://road.png";
                    break;
                case Wall:
                    imgName = "://wall.png";
                    break;
                case Box:
                    imgName = "://box.png";
                    break;
                case Point:
                    imgName = "://point.png";
                    break;
                case InPoint:
                    imgName = "://Inpoint.png";
                    break;
            }
            QImage img(imgName);
            p->drawImage( QRect(
                               Pos.x() + j * img.width(),   // ① 绘制区域左上角 X 坐标：基准位置 + 列索引 × 图片宽度

                               //┌────────────────────────────────────────┐
                               //│  _Pos.x()                              │
                               //│    ↓                                   │
                               //│    ●────────┬────────┬────────┬──────→ │
                               //│             │        │        │        │
                               //│           j=0      j=1      j=2      j=3
                               //│             │        │        │        │
                               //│           0×w      1×w      2×w      3×w
                               //│             │        │        │        │
                               //│          X=base   X=base+w X=base+2w  │
                               //└────────────────────────────────────────┘

                               // 示例：Pos.x()=100, img.width()=32, j=2
                               // X = 100 + 2×32 = 164

                               Pos.y() + i * img.height(),  // ② 绘制区域左上角 Y 坐标：基准位置 + 行索引 × 图片高度

                               //┌────────────────────────────────────────┐
                               //│  _Pos.y()                              │
                               //│    ↓                                   │
                               //│    ●                                   │ i=0
                               //│    │                                   │
                               //│    │ 32px                              │ i=1 → Y = base + 1×32
                               //│    │                                   │
                               //│    ●                                   │ i=2 → Y = base + 2×32
                               //│    │                                   │
                               //│    │ 32px                              │
                               //│    │                                   │
                               //│    ●                                   │ i=3 → Y = base + 3×32
                               //│    ▼                                   │
                               //└────────────────────────────────────────┘

                               // 示例：Pos.y()=50, img.height()=32, i=1
                               // Y = 50 + 1×32 = 82

                               img.width(),                   // ③ 绘制区域宽度 = 图片原始宽度
                               img.height()                   // ④ 绘制区域高度 = 图片原始高度
                           ),
                           img  );
                           //p->drawImage(_Pos,img);
        }
    }
}
