#include "customopenglwidget.h"

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

// Global vars for shader program, vertex buffer, UV buffer, and VAO
QOpenGLShaderProgram *shaderProgram;
QOpenGLBuffer *vertexBuffer;
QOpenGLBuffer *uvBuffer;
QOpenGLVertexArrayObject *vao;

// Constructor
CustomOpenGLWidget::CustomOpenGLWidget(QWidget *parent) :
    QOpenGLWidget(parent), texture1(nullptr), texture2(nullptr)
{
    movie1 = new QMovie("C:/Users/Uri/Documents/fuckingdumbshit/2.gif");
    movie2 = new QMovie("C:/Users/Uri/Documents/fuckingdumbshit/1.gif");

    connect(movie1, &QMovie::frameChanged, this, &CustomOpenGLWidget::updateAnimation);
    connect(movie2, &QMovie::frameChanged, this, &CustomOpenGLWidget::updateAnimation);

    movie1->start();
    movie2->start();
}

// Destructor
CustomOpenGLWidget::~CustomOpenGLWidget() {
    makeCurrent();
    delete texture1;
    delete texture2;
    doneCurrent();
}

// Draw a texture USING COOL OPENGL!!!
void CustomOpenGLWidget::drawTexture(QOpenGLTexture *texture) {

    // Debug. Check if texture, VAO, and shaderProgram are valid.
    // This shouldn't actually ever be triggered unless something is fucked up with prepareVertexBuffer() or prepareShaderProgram()
    if (!texture || !vao || !shaderProgram) {
        qWarning() << "Invalid texture/VAO/shaderProgram";
        return;
    }

    // Bind the shader program, VAO, and texture
    shaderProgram->bind();
    vao->bind();
    texture->bind();

    // Set the texture sampler uniform value
    shaderProgram->setUniformValue("textureSampler", 0);

    // Draw the quad (two triangles) using the bound VAO. It's OpenGL shit
    glDrawArrays(GL_TRIANGLES, 0, 6); // Change to GL_TRIANGLES and use 6 vertices

    // Release the texture, VAO, and shader program
    texture->release();
    vao->release();
    shaderProgram->release();
}

// Initialize OpenGL state
void CustomOpenGLWidget::initializeGL() {
    // Initialize OpenGL functions
    initializeOpenGLFunctions();

    // Enable blending and set the blend function (for transparency)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Prepare the shader program and vertex buffer
    prepareShaderProgram();
    prepareVertexBuffer();
}

// Render the OpenGL scene
void CustomOpenGLWidget::paintGL() {
    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // Update texture1 & texture2
    texture1 = new QOpenGLTexture(movie1->currentPixmap().toImage());
    texture2 = new QOpenGLTexture(movie2->currentPixmap().toImage());

    // Draw both textures on top of each other. This determines layer order.
    drawTexture(texture1);
    drawTexture(texture2);

    update();
}

// These next two functions are OpenGL shit.
// Prepare the shader program
void CustomOpenGLWidget::prepareShaderProgram() {
    // Define the vertex and fragment shader source code.
    // These are written in GLSL and could be loaded from a separate file if desired.
    const char *vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec3 vertexPosition;
        layout(location = 1) in vec2 vertexUV;
        out vec2 UV;
        void main() {
            gl_Position = vec4(vertexPosition, 1.0);
            UV = vertexUV;
        }
    )";
    const char *fragmentShaderSource = R"(
        #version 330 core
        in vec2 UV;
        out vec4 color;
        uniform sampler2D textureSampler;
        void main() {
            color = texture(textureSampler, UV);
        }
    )";

    // Create and compile the shader program
    shaderProgram = new QOpenGLShaderProgram(this);
    shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    shaderProgram->link();
}
// Prepare the vertex buffer and VAO
void CustomOpenGLWidget::prepareVertexBuffer() {
    // Define vertex positions (a simple quad)
    static const GLfloat vertexData[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,

        1.0f, -1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f
    };

    // Define UV coordinates
    static const GLfloat uvData[] = {
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,

        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f
    };

    // Create and bind the vertex buffer
    vertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vertexBuffer->create();
    vertexBuffer->bind();
    vertexBuffer->allocate(vertexData, sizeof(vertexData));

    // Create and bind the UV buffer
    uvBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    uvBuffer->create();
    uvBuffer->bind();
    uvBuffer->allocate(uvData, sizeof(uvData));

    // Create and bind the VAO
    vao = new QOpenGLVertexArrayObject(this);
    vao->create();
    vao->bind();

    // Bind the vertex buffer and set the attribute buffer for vertex positions
    vertexBuffer->bind();
    shaderProgram->enableAttributeArray(0);
    shaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3);

    // Bind the UV buffer and set the attribute buffer for UV coordinates
    uvBuffer->bind();
    shaderProgram->enableAttributeArray(1);
    shaderProgram->setAttributeBuffer(1, GL_FLOAT, 0, 2);

    // Release the VAO
    vao->release();
}

// QOpenGLWidget overridden protected function,
void CustomOpenGLWidget::resizeGL(int width, int height) {
    // Set the viewport to the given width and height
    glViewport(0, 0, width, height);
}

void CustomOpenGLWidget::updateAnimation() {
    update();
}


