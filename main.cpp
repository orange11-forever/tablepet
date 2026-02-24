#include "widget.h"
#include<QSystemTrayIcon>
#include <QApplication>
#include<QIcon>
int main(int argc, char *argv[])
{ //放到系统托盘
    QApplication a(argc, argv);
    Widget w;
    QSystemTrayIcon sysTray(QIcon(":/daily/image/daily/2.png"),&w);
    QMenu menu;
    //托盘动作
    auto showAct=new QAction("show",&sysTray);
    auto exitAct=new QAction("exit",&sysTray);
    QObject::connect(showAct,&QAction::triggered,[&](){
        w.setVisible(true);
    });
    QObject::connect(exitAct,&QAction::triggered,[&](){
        QApplication::quit();
    });
    menu.addAction(showAct);
    menu.addAction(exitAct);
    //将菜单设置为托盘图标的上下文菜单
    sysTray.setContextMenu(&menu);
    sysTray.show();
    w.show();
    return a.exec();
}
