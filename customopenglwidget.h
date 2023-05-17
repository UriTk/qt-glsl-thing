#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QMovie>

class CustomOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    CustomOpenGLWidget(QWidget *parent = nullptr);
    ~CustomOpenGLWidget();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

private slots:
    void updateAnimation();

private:
    QOpenGLTexture *texture1;
    QOpenGLTexture *texture2;
    QMovie *movie1;
    QMovie *movie2;
    void prepareShaderProgram();
    void prepareVertexBuffer();
    void drawTexture(QOpenGLTexture *texture);
};
