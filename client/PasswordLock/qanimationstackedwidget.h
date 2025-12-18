#ifndef QANIMATIONSTACKEDWIDGET_H
#define QANIMATIONSTACKEDWIDGET_H

#include <QStackedWidget>
#include <QPropertyAnimation>
#include <QPauseAnimation>
#include <QSequentialAnimationGroup>
#include <QDebug>

enum AnimationMode{
    L2R,R2L,U2D,D2U,FromL,FromR,FromU,FromD,Quit2L, Quit2R, Quit2U, Quit2D,AUTOScroll
};

class QAnimationStackedWidget  : public QStackedWidget
{
    Q_OBJECT

public:
    QAnimationStackedWidget(QWidget *parent);
    ~QAnimationStackedWidget();
    //设置动画时长
    void setAnimationDuration(int duration);
    //设置动画模式，模式在enum AnimationMode中枚举
    void setAnimationMode(int mode);
signals:
    //动画完成时发出信号
    void sStackedWidgetAnimationFinished(int indexbefore,int indexcurrent);
public slots:
    //支持除AUTOScroll外的动画
    void StartStackedWidgetAnimation(int indexbefore, int indexafter);
private:
    QPropertyAnimation*					m_AnimationCurrent;  // 当前窗口动画
    QPropertyAnimation*					m_AnimationNext;     // 下一窗口动画
    int            m_AnimationDuration = 1000;       // 持续时间
    int			m_AnimationMode = 0;         // 动画模式
    int			m_lndexBeforeAnimation = 0;  // 原窗口
    int			m_lndexAfterAnimation = 0;   // 目标窗口
    bool			m_IsAnimation = false;
    bool			m_IsGroupAnimation = false;
    QPointF rectL;
    QPointF rectR;
    QPointF rectU;
    QPointF rectD;
    QPointF rect;
    void startAnimationEngine();
private slots:
    void OnAnimationFinished();
};

#endif // QANIMATIONSTACKEDWIDGET_H

