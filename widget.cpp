#include "widget.h"
#include<QPainter>
#include<QPixmap>
#include<QCursor>
#include<QMetaEnum>
Widget::Widget(QWidget *parent)
    : QWidget(parent),
    timer(new QTimer(this)),
    menu(new QMenu(this))
{   this->setWindowFlag(Qt::FramelessWindowHint);//去除窗口边框
    this->setAttribute(Qt::WA_TranslucentBackground);//背景透明
    this->installEventFilter(new DragFilter);//安装事件过滤器处理拖拽事件
    connect(timer,&QTimer::timeout,[this](){
   static int index=0;
   auto paths= this->roleActMap.value(this->currentAct);
   this->currentRolePix=paths[index++%paths.size()];//循环播放
   this->update();
}
);
initMenu();
loadRoleActs();
showActAnimation(RoleAct::Normal);
}
Widget::~Widget() {}
void Widget::showActAnimation(RoleAct act)
{
    timer->stop();
    this->currentAct=act;
    timer->start(100);
}
//菜单项处理
void Widget::onMenuTriggered(QAction *action)
{//与枚举类型 RoleAct 关联的元枚举对象 QMetaEnum
    QMetaEnum me =QMetaEnum::fromType<RoleAct>();
    bool m;
    int k=me.keyToValue(action->text().toStdString().c_str(),&m);
    if(!m){   
        return;
    }
    //类型强制转换
    showActAnimation(static_cast<RoleAct>(k));

}
void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(this->currentRolePix.toLocalFile());
    painter.drawPixmap(0,0,180,150,pix);
   
}

void Widget::contextMenuEvent(QContextMenuEvent *event)
{this->menu->popup(QCursor::pos());//菜单出现在鼠标点击的位置

}
void Widget::loadRoleActs()
{
    auto addRes=[this](RoleAct k,QString path,int count){
       QList<QUrl>paths;//储存路径
       char buf[256];//临时缓冲区
       for(int i=0;i<count;++i){
        memset(buf,0,sizeof(buf));
        sprintf_s(buf,path.toStdString().c_str(),i);// 将路径模板中的 %d 替换为当前索引 i
        paths.append(QUrl::fromLocalFile(buf));//将生成的文件路径转换为 QUrl 并添加到列表
       }
       roleActMap.insert(k,paths);
    };
    addRes(RoleAct::Daily, ":/daily/image/daily/%d.png", 10);
    addRes(RoleAct::Happy, ":/happy/image/happy/%d.png", 10);
    addRes(RoleAct::Normal, ":/normal/image/normal/%d.png", 10);
    addRes(RoleAct::Sleep, ":/sleep/image/sleep/%d.png", 9);
}
  
void Widget::initMenu()
{   //初始化菜单
    menu->addAction("Normal");
    menu->addAction("Daily");
    menu->addAction("Happy");
    menu->addAction("Sleep");
    QAction*act=new QAction("Hide");
    connect(act,&QAction::triggered,[this](){
        this->setVisible(false);
    });
    menu->addAction(act);
    connect(this->menu,&QMenu::triggered,this,&Widget::onMenuTriggered);
}
