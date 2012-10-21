#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //World w;
    //qsrand()
    for (int i=0; i < 500; i++){
        //generateAndSaveAnimal();
        //w.addAnimal(Animal::loadAnimal("rnd.ani"));
        w.addAnimal(generateAnimal());
    }
    //loadAnimals();
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
    ui->lblCount->setText(QString("Animal count %1").arg(w.getAnimalCount()));
}

void MainWindow::on_btnStart_clicked()
{
    tmr.start();
}

void MainWindow::on_btnStop_clicked()
{
    tmr.stop();
    qDebug("MainWindow: timer is stopped.");
}

void MainWindow::createAndSaveTestAnimals()
{
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
    c3 << start // spiral
       << load_from_mem //load from var1
       << move_mem_right //save var2
       << data_dec
       << save_to_mem

       << move_mem_right //save to var3
       << data_dec
       << save_to_mem
           //right
               << action_move_right
               << data_dec
           << jump_to_ifnz
           << AnimalCommand(-3)
           //up
           << load_from_mem
           << data_dec
           << save_to_mem
               << action_move_up
               << data_dec
           << jump_to_ifnz
           << AnimalCommand(-3)
           //left
           << load_from_mem
           << data_dec
           << save_to_mem
               << action_move_left
               << data_dec
           << jump_to_ifnz
           << AnimalCommand(-3)
           //down
           << load_from_mem
           << data_dec
           << save_to_mem
               << action_move_down
               << data_dec
           << jump_to_ifnz
           << AnimalCommand(-3)
           //dec var2
           << move_mem_left
           << load_from_mem
           << data_dec
           << save_to_mem
           << jump_to_ifnz
           << AnimalCommand(-33)
           << move_mem_left // restore var1
//           << load_from_mem
       << end;
    QList<quint8> cmds = Animal::compile(c);
    QList<quint8> cmds2 = Animal::compile(c2);
    QList<quint8> cmds3 = Animal::compile(c3);
    QList<quint8> mems;
    Animal::saveAnimal("c1.ani",cmds,mems);
    Animal::saveAnimal("c2.ani",cmds2,mems);
    mems << 10 << 0 << 0;
    Animal::saveAnimal("c3.ani",cmds3,mems);
}

void MainWindow::loadAnimals()
{
    for (int i=0; i < 50; i++){
        w.addAnimal(Animal::loadAnimal("c1.ani"));
        w.addAnimal(Animal::loadAnimal("c2.ani"));
        w.addAnimal(Animal::loadAnimal("c3.ani"));
    }
}

void MainWindow::generateAndSaveAnimal()
{
    QList<AnimalCommand> cmd;
    quint8 i, maxCmd, maxMem;
    maxCmd = qrand()%1500 + 1;
    maxMem = qrand()%1500 + 1;
    QList<quint8> mems;
    for (i=0; i< maxCmd; i++){
        cmd << AnimalCommand(qrand() % MAX_ANIMAL_COMMAND);
        cmd << AnimalCommand(qrand() % sizeof(quint8));
    }
    for (i=0; i< maxMem; i++)
        mems << char(qrand() % sizeof(quint8));
    QList<quint8> cmds = Animal::compile(cmd);
    Animal::saveAnimal("rnd.ani",cmds,mems);
}

Animal *MainWindow::generateAnimal()
{
    QList<AnimalCommand> cmd;
    quint8 i, maxCmd, maxMem;
    maxCmd = qrand()%1500 + 1;
    maxMem = qrand()%1500 + 1;
    QList<quint8> mems;
    for (i=0; i< maxCmd; i++){
        cmd << AnimalCommand(qrand() % MAX_ANIMAL_COMMAND);
        cmd << AnimalCommand(qrand() % sizeof(quint8));
    }
    for (i=0; i< maxMem; i++)
        mems << char(qrand() % sizeof(quint8));
    QList<quint8> cmds = Animal::compile(cmd);
    return (new Animal(cmds,mems));
}
