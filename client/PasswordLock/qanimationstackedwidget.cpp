#include "QAnimationStackedWidget.h"



QAnimationStackedWidget::QAnimationStackedWidget(QWidget *parent)
    : QStackedWidget(parent)
{
    m_AnimationCurrent = new QPropertyAnimation(this,"pos"); // 动画对象，当前页
    m_AnimationNext = new QPropertyAnimation(this, "pos");  // 下一页
    connect(m_AnimationNext, &QPropertyAnimation::finished, this, &QAnimationStackedWidget::OnAnimationFinished);  // 动画完成时调用OnAnimationFinished
    connect(m_AnimationCurrent, &QPropertyAnimation::finished, this, &QAnimationStackedWidget::OnAnimationFinished);// 动画完成时调用OnAnimationFinished
}

QAnimationStackedWidget::~QAnimationStackedWidget()
{
    if (m_AnimationCurrent) { m_AnimationCurrent->deleteLater(); m_AnimationCurrent = Q_NULLPTR; }
    if (m_AnimationNext) { m_AnimationNext->deleteLater(); m_AnimationNext = Q_NULLPTR; }
}

void QAnimationStackedWidget::OnAnimationFinished()
{
    m_IsAnimation = false;
    qDebug()<<"end draw";
    widget(m_lndexBeforeAnimation)->hide();
    widget(m_lndexAfterAnimation)->show();
    widget(m_lndexAfterAnimation)->raise();
    setCurrentIndex(m_lndexAfterAnimation);
    emit sStackedWidgetAnimationFinished(m_lndexBeforeAnimation, m_lndexAfterAnimation);
}

void QAnimationStackedWidget::StartStackedWidgetAnimation(int indexbefore, int indexafter)
{
    if (m_IsAnimation) { return; }
    if (indexbefore >= count() || indexafter >= count() || indexbefore < 0 || indexafter < 0 || indexafter == indexbefore)
    {
        return;
    }
    m_lndexBeforeAnimation = indexbefore;
    m_lndexAfterAnimation = indexafter;
    m_AnimationCurrent->setDuration(m_AnimationDuration);
    m_AnimationNext->setDuration(m_AnimationDuration);
    m_IsAnimation = true;
    startAnimationEngine();
}

void QAnimationStackedWidget::startAnimationEngine()
{
    m_AnimationCurrent->setTargetObject(widget(m_lndexBeforeAnimation));
    m_AnimationNext->setTargetObject(widget(m_lndexAfterAnimation));
    widget(m_lndexBeforeAnimation)->show();
    widget(m_lndexAfterAnimation)->show();
    switch (m_AnimationMode)
    {
    case R2L:
        m_AnimationCurrent->setStartValue(rect);
        m_AnimationCurrent->setEndValue(rectL);
        m_AnimationNext->setStartValue(rectR);
        m_AnimationNext->setEndValue(rect);
        m_AnimationCurrent->start();
        m_AnimationNext->start();
        break;
    case L2R:
        m_AnimationCurrent->setStartValue(rect);
        m_AnimationCurrent->setEndValue(rectR);
        m_AnimationNext->setStartValue(rectL);
        m_AnimationNext->setEndValue(rect);
        m_AnimationCurrent->start();
        m_AnimationNext->start();
        break;
    case U2D:
        m_AnimationCurrent->setStartValue(rect);
        m_AnimationCurrent->setEndValue(rectD);
        m_AnimationNext->setStartValue(rectU);
        m_AnimationNext->setEndValue(rect);
        m_AnimationCurrent->start();
        m_AnimationNext->start();
        break;
    case D2U:
        m_AnimationCurrent->setStartValue(rect);
        m_AnimationCurrent->setEndValue(rectU);
        m_AnimationNext->setStartValue(rectD);
        m_AnimationNext->setEndValue(rect);
        m_AnimationCurrent->start();
        m_AnimationNext->start();
        break;
    case FromD:
        m_AnimationNext->setStartValue(rectD);
        m_AnimationNext->setEndValue(rect);
        m_AnimationNext->start();
        widget(m_lndexAfterAnimation)->raise();
        break;
    case FromU:
        m_AnimationNext->setStartValue(rectU);
        m_AnimationNext->setEndValue(rect);
        m_AnimationNext->start();
        widget(m_lndexAfterAnimation)->raise();
        break;
    case FromL:
        m_AnimationNext->setStartValue(rectL);
        m_AnimationNext->setEndValue(rect);
        m_AnimationNext->start();
        widget(m_lndexAfterAnimation)->raise();
        break;
    case FromR:
        m_AnimationNext->setStartValue(rectR);
        m_AnimationNext->setEndValue(rect);
        m_AnimationNext->start();
        widget(m_lndexAfterAnimation)->raise();
        break;
    case Quit2D:
        m_AnimationCurrent->setStartValue(rect);
        m_AnimationCurrent->setEndValue(rectD);
        m_AnimationCurrent->start();
        m_AnimationNext->setStartValue(rect);
        m_AnimationNext->setEndValue(rect);
        m_AnimationNext->start();
        widget(m_lndexBeforeAnimation)->raise();
        break;
    case Quit2U:
        m_AnimationCurrent->setStartValue(rect);
        m_AnimationCurrent->setEndValue(rectU);
        m_AnimationCurrent->start();
        m_AnimationNext->setStartValue(rect);
        m_AnimationNext->setEndValue(rect);
        m_AnimationNext->start();
        widget(m_lndexBeforeAnimation)->raise();
        break;
    case Quit2L:
        m_AnimationCurrent->setStartValue(rect);
        m_AnimationCurrent->setEndValue(rectL);
        m_AnimationCurrent->start();
        m_AnimationNext->setStartValue(rect);
        m_AnimationNext->setEndValue(rect);
        m_AnimationNext->start();
        widget(m_lndexBeforeAnimation)->raise();
        break;
    case Quit2R:
        m_AnimationCurrent->setStartValue(rect);
        m_AnimationCurrent->setEndValue(rectR);
        m_AnimationCurrent->start();
        m_AnimationNext->setStartValue(rect);
        m_AnimationNext->setEndValue(rect);
        m_AnimationNext->start();
        widget(m_lndexBeforeAnimation)->raise();
        break;
    default:
        break;
    }

}

void QAnimationStackedWidget::setAnimationMode(int mode)
{
    m_AnimationMode = mode;
    rectL = QPointF(-width(), 0);
    rectR = QPointF(width(), 0);
    rectU = QPointF(0, -height());
    rectD = QPointF(0, height());
    rect = QPointF(0, 0);
}

void QAnimationStackedWidget::setAnimationDuration(int duration)
{
    m_AnimationDuration=duration;
}

