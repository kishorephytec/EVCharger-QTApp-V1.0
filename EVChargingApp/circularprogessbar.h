#ifndef CIRCULARPROGESSBAR_H
#define CIRCULARPROGESSBAR_H

#include <QWidget>
#include <QPointF>
#include <QPainter>

class CircularProgessBar : public QWidget
{
    Q_OBJECT
public:
    explicit CircularProgessBar(QWidget *parent = 0);
    void setCircleRatio(const qreal &arcRatio=1.0,
                        const qreal &outerCircleRatio=0.8,
                        const qreal &innerCircleRatio = 0.6);
    void setCirclePen(const QPen &outerCirclePen=QPen(Qt::NoPen),
                      const QPen &innerCirclePen=QPen(Qt::NoPen));
    void setCircleBrush(const QBrush &outerCircleBrush=QBrush("#808080"),
                        const QBrush &innerCircleBrush=QBrush("#CBF6FE"));
    void setArcPenBrush(const QBrush &arcPenBrush=QBrush("#008CFE"));
    void setArcPenDashPattern(const QVector<qreal> &dashs,
                              const bool &showDashBaseBrush=false,
                              const QBrush &dashBaseBrush=QBrush("#DCDCDC"));
    void setArcPenGradient(const QConicalGradient &conicalGrad,
                           const bool &isShowGradient=true);
    void setTextProperty(const QFont &font=QFont(), const QColor &color=QColor("#000000"));
    void setText(const QString &text);

    void setProgressValue(const qreal &progressValue);

protected:
    virtual void paintEvent(QPaintEvent  *event);

private:
    qreal currentProgressValue;
    QPointF circleCenter;
    qreal maxRadius;

    qreal arcRatio;
    qreal outerCircleRatio;
    qreal innerCircleRatio;
    QPen outerCirclePen;
    QPen innerCirclePen;
    QBrush outerCircleBrush;
    QBrush innerCircleBrush;
    QString text;
    QColor textColor;
    QFont textFont;
    QPen arcPen;
    bool isShowDashBaseBrush;
    QBrush dashBaseBrush;
    bool isShowGradient;
    QConicalGradient conicalGrad;

    void drawCircleProgressBar(QPainter &painter);

signals:

public slots:
};

#endif // CIRCULARPROGESSBAR_H
