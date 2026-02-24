#ifndef WIDGET_H
#define WIDGET_H
#include<QUrl>
#include <QWidget>
#include<QMap>
#include<QList>
#include<QTimer>
#include<QEvent>
#include<QMouseEvent>
#include<QContextMenuEvent>
#include<QMenu>
class QPaintEvent;
namespace Act{
Q_NAMESPACE
//四种状态
enum RoleAct{
    Daily,
    Happy,
    Normal,
    Sleep
};
Q_ENUM_NS(RoleAct);

}
using namespace Act;
class Widget : public QWidget
{
    Q_OBJECT

public:    
    Widget(QWidget *parent = nullptr);
    ~Widget();
public:
void showActAnimation(RoleAct act);//展示动作
public:
void onMenuTriggered(QAction*action);//菜单
protected:
//重写画图事件和菜单事件
    void paintEvent(QPaintEvent*event)override;
    void contextMenuEvent(QContextMenuEvent*event)override;
protected:
    void loadRoleActs();//加载动作
    void initMenu();//初始化菜单
private:
QMap<RoleAct,QList<QUrl>> roleActMap;
QTimer*timer;
RoleAct currentAct;
QUrl currentRolePix;
QMenu*menu;
};
//拖拽以及点击事件处理
class DragFilter:public QObject{
public:
    bool eventFilter(QObject*obj,QEvent*event){
        auto w =dynamic_cast<QWidget*>(obj);
        if(!w){
            return false;
        }
        if(event->type()==QEvent::MouseButtonPress){
            auto e=dynamic_cast<QMouseEvent*>(event);
            if(e){
                pos=e->pos();
            }
        }
        else if(event->type()==QEvent::MouseMove)
        {
            auto e =dynamic_cast<QMouseEvent*>(event);
            if(e){
                if(e->buttons()&Qt::MouseButton::LeftButton){
                    w->move(e->globalPosition().toPoint()-pos);
                }
            }
        }
        return QObject::eventFilter(obj,event);
    }
private:
    QPoint pos;
};
#endif // WIDGET_H
