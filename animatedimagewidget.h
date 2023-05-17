#ifndef ANIMATEDIMAGEWIDGET_H
#define ANIMATEDIMAGEWIDGET_H
#include <QOpenGLWidget>
#include <QImageReader>
#include <QTimer>

class AnimatedImageWidget : public QOpenGLWidget
{
public:
    AnimatedImageWidget(QWidget *parent = nullptr);
    bool loadAnimation(const QString &filename);
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void updateFrame();

private:
    QImageReader m_imageReader;
    QImage m_currentFrame;
    QTimer m_frameTimer;
};

#endif // ANIMATEDIMAGEWIDGET_H
