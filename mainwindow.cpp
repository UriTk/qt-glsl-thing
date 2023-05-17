#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "customopenglwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    CustomOpenGLWidget *openGLWidget = new CustomOpenGLWidget(this);
    openGLWidget->setGeometry(0, 0, 800, 600);
}

MainWindow::~MainWindow()
{
    delete ui;
}


