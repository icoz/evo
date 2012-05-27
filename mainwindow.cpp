#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //World w;
    QList<AnimalCommand> c;
    c.append(start);
    c << action_move_left << action_move_down << action_move_right
         << action_move_down << end;
    QList<char> cmds = Animal::compile(c);
    QList<char> mems;
    w.addAnimal(cmds,mems);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    w.makeStep();
    ui->label->setPixmap(QPixmap::fromImage(w.getImage()));
}
