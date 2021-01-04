#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->widget_2->setType(1);
    ui->widget_3->setType(2);
    ui->widget_4->setType(3);
}

MainWindow::~MainWindow()
{
    delete ui;
}

