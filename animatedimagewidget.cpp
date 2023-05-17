#include "animatedimagewidget.h"
#include <QPainter>

AnimatedImageWidget::AnimatedImageWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    connect(&m_frameTimer, &QTimer::timeout, this, &AnimatedImageWidget::updateFrame);
}

bool AnimatedImageWidget::loadAnimation(const QString &filename){
    m_imageReader.setFileName(filename);
    if (!m_imageReader.canRead())
    {
        return false;
    }

    m_frameTimer.start(m_imageReader.nextImageDelay());
    m_currentFrame = m_imageReader.read();
    update();

    return true;
}

QSize AnimatedImageWidget::sizeHint() const
{
    return m_imageReader.size();
}

void AnimatedImageWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(0, 0, m_currentFrame);
}

void AnimatedImageWidget::updateFrame()
{
    if (m_imageReader.currentImageNumber() == m_imageReader.imageCount()-6)
    {
        m_imageReader.jumpToImage(0);
        qInfo( "C Style Info Message" );
    }
    m_frameTimer.start(m_imageReader.nextImageDelay());
    m_currentFrame = m_imageReader.read();
    update();
}
