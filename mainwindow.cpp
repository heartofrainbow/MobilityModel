#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "randomwalk.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_8_triggered()
{
    RandomWalk *randomwalk_ui = new RandomWalk;
    randomwalk_ui->show();

}
