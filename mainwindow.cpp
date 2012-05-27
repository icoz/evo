#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //World w;
    QList<AnimalCommand> c,c2,c3;
    c << start
      << action_move_left
      << action_eat_down
      << action_eat_up
      << action_eat_left
      << action_eat_right
      << action_move_down
      << action_eat_down
      << action_eat_up
      << action_eat_left
      << action_eat_right
      << action_move_right
      << action_eat_down
      << action_eat_up
      << action_eat_left
      << action_eat_right
      << action_move_down
      << action_eat_down
      << action_eat_up
      << action_eat_left
      << action_eat_right
      << end;
    c2 << start
      << action_eat_left
      << action_move_left
      << action_eat_down
      << action_move_down
      << end;
    c3 << start
       << action_eat_right
       << action_move_right
       << action_eat_up
       << action_move_up
      << end;
    QList<char> cmds = Animal::compile(c);
    QList<char> cmds2 = Animal::compile(c2);
    QList<char> cmds3 = Animal::compile(c3);
    QList<char> mems;
    for (int i=0; i < 50; i++){
        w.addAnimal(cmds,mems);
        w.addAnimal(cmds2,mems);
        w.addAnimal(cmds3,mems);
    }
    tmr.setInterval(50);
    connect(&tmr, SIGNAL(timeout()), SLOT(onTimerTimeout()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onTimerTimeout()
{
    //qDebug("time!");
    w.makeStep();
    ui->label->setPixmap(QPixmap::fromImage(w.getImage()));
}

void MainWindow::on_btnStart_clicked()
{
    tmr.start();
}

void MainWindow::on_btnStop_clicked()
{
    tmr.stop();
}
