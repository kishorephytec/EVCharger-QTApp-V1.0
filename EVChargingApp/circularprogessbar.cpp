#include "circularprogessbar.h"
#include <QDebug>

// Constructor for the CircularProgessBar class
CircularProgessBar::CircularProgessBar(QWidget *parent) : QWidget(parent)
{
    // Set the current progress value to 0
    this->currentProgressValue = 0;

    // Set the flags for showing the dash base brush and gradient to false
    this->isShowDashBaseBrush = false;
    this->isShowGradient = false;

    // Initialize the circle ratio, brush, and pen using default values
    setCircleRatio();
    setCircleBrush();
    setCirclePen();

    // Initialize the arc pen brush using default values
    setArcPenBrush();

    // Initialize the text property using default values
    QFont font;
    font.setPointSize(16);
    setTextProperty(font);

    // Resize the widget to 300x300 pixels
    this->resize(600,600);
}

// Set the circle ratio for the circular progress bar
void CircularProgessBar::setCircleRatio(const qreal &arcRatio,
                                        const qreal &outerCircleRatio, const qreal &innerCircleRatio)
{
    // Set the arc ratio
    this->arcRatio = arcRatio;
    // Set the outer circle ratio
    this->outerCircleRatio = outerCircleRatio;
    // Set the inner circle ratio
    this->innerCircleRatio = innerCircleRatio;
}

// Set the pen for the outer and inner circles of the circular progress bar
void CircularProgessBar::setCirclePen(const QPen &outerCirclePen,
                                      const QPen &innerCirclePen)
{
    // Set the pen for the outer circle
    this->outerCirclePen = outerCirclePen;
    // Set the pen for the inner circle
    this->innerCirclePen = innerCirclePen;
}

// Set the brush for the outer and inner circles of the circular progress bar
void CircularProgessBar::setCircleBrush(const QBrush &outerCircleBrush,
                                        const QBrush &innerCircleBrush)
{
    // Set the brush for the outer circle
    this->outerCircleBrush = outerCircleBrush;
    // Set the brush for the inner circle
    this->innerCircleBrush = innerCircleBrush;
}

// Set the brush for the arc pen of the circular progress bar
void CircularProgessBar::setArcPenBrush(const QBrush &arcPenBrush)
{
    // Set the cap style for the arc pen
    this->arcPen.setCapStyle(Qt::FlatCap);
    // Set the brush for the arc pen
    this->arcPen.setBrush(arcPenBrush);
}

// Set the dash pattern for the arc pen of the circular progress bar
void CircularProgessBar::setArcPenDashPattern(const QVector<qreal> &dashs,
                                              const bool &isShowDashBaseBrush, const QBrush &dashBaseBrush)
{
    // Set the dash pattern for the arc pen
    arcPen.setDashPattern(dashs);
    // Set whether to show the dash base brush
    this->isShowDashBaseBrush = isShowDashBaseBrush;
    // Set the dash base brush
    this->dashBaseBrush = dashBaseBrush;
}

// Set the gradient for the arc pen of the circular progress bar
void CircularProgessBar::setArcPenGradient(const QConicalGradient &conicalGrad, const bool &isShowGradient)
{
    // Set the conical gradient for the arc pen
    this->conicalGrad = conicalGrad;
    // Set whether to show the gradient
    this->isShowGradient = isShowGradient;
}

// Set the text property for the circular progress bar
void CircularProgessBar::setTextProperty(const QFont &font, const QColor &color)
{
    // Set the text color
    this->textColor = color;
    // Set the text font
    this->textFont = font;
}

// Set the text for the circular progress bar
void CircularProgessBar::setText(const QString &text)
{
    // Set the text
    this->text = text;
    // Update the circular progress bar
    update();
}

// Set the progress value for the circular progress bar
void CircularProgessBar::setProgressValue(const qreal &progressValue)
{
    // Set the current progress value
    this->currentProgressValue = progressValue;
    // Update the circular progress bar
    update();
}

// Paint event for the circular progress bar
void CircularProgessBar::paintEvent(QPaintEvent *event)
{
    // Call the base class paint event
    QWidget::paintEvent(event);
    // Get the width and height of the widget
    int width = this->width();
    int height = this->height();
    // Determine the center of the widget (the center of the circle)
    circleCenter.setX(width/2.0);
    circleCenter.setY(height/2.0);
    // Determine the maximum radius that can display a circle normally
    int maxDiameter = width > height ? height : width;
    maxRadius = maxDiameter*0.5;
    // Define painter and set properties
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);// Anti-aliasing

    // Draw a circular progress bar
    drawCircleProgressBar(painter);

}

// Draw a circular progress bar
void CircularProgessBar::drawCircleProgressBar(QPainter &painter)
{
    // Calculate the radius of the outer and inner circles
    qreal outerCircleRadius = maxRadius*outerCircleRatio;
    qreal innerCircleRadius = maxRadius*innerCircleRatio;
    // Calculate the width of the annulus
    qreal annulusWidth = outerCircleRadius - innerCircleRadius;
    // Calculate the width of the arc pen
    qreal arcPenWidth = annulusWidth*arcRatio;
    // Calculate the radius of the circle where the arc is located
    qreal arcRadius = annulusWidth/2.0+innerCircleRadius;
    // Rectangle where the arc is located
    QRectF arcRectF(circleCenter.x()-arcRadius,circleCenter.y()-arcRadius,arcRadius*2,arcRadius*2);

    /* Outer circle */
    painter.setPen(outerCirclePen);
    painter.setBrush(outerCircleBrush);
    painter.drawEllipse(circleCenter,outerCircleRadius,outerCircleRadius);
    /* Inner circle */
    painter.setPen(innerCirclePen);
    painter.setBrush(innerCircleBrush);
    painter.drawEllipse(circleCenter,innerCircleRadius,innerCircleRadius);
    /* Arc */
    /* The circular progress bar generally starts at 12 o'clock (90*16) and rotates clockwise (negative value).
     * 57.6=360*16/100; that is, one percentage point corresponds to a value */
    // Set the pen width to adapt to the pen width
    arcPen.setWidthF(arcPenWidth);
    if(isShowDashBaseBrush)// Use dashed base color to draw progress bar background once
    {
        QBrush penBrush = arcPen.brush();// Record the current brush of the arc pen first
        arcPen.setBrush(dashBaseBrush);
        painter.setPen(arcPen);
        painter.drawArc(arcRectF,90*16,-360*16);
        arcPen.setBrush(penBrush);// Restore brush
    }
    if(isShowGradient)
    {
        conicalGrad.setCenter(circleCenter);// Set the center of conical gradient
        arcPen.setBrush(QBrush(conicalGrad));
    }
    painter.setPen(arcPen);
    painter.drawArc(arcRectF,90*16,-(int)(currentProgressValue*57.6));
    /* Text */
    /* The color of text drawn by drawText is determined by pen, but the size of text font cannot be set by pen width. You need to set corresponding font */
    painter.setPen(QPen(textColor));
    painter.setFont(textFont);
    painter.drawText(arcRectF,Qt::AlignCenter,text);
}
