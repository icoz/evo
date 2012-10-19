#include "animal.h"
#include <QFile>
#include <QDataStream>
#include <QApplication>

void Animal::onTick()
{
    run();
}

void Animal::run()
{
    int i = 10000;
    Direction dir;
    qApp->processEvents();
    while (i-- > 0) {
        if (cmd_ptr == cmd.size()) {
            cmd_ptr = 0;
        }
        char cur_cmd = cmd.at(cmd_ptr++);
        switch (AnimalCommand(cur_cmd)) {
        //cmd group
        case move_cmd_left:
            (cmd_ptr == 0) ? cmd.size() : cmd_ptr--;
            break;
        case move_cmd_right:
            (cmd_ptr == cmd.size()) ? 0 : cmd_ptr++;
            break;
        case jump_to:
            {
            signed char jmp = cmd.at(cmd_ptr);
            cmd_ptr += jmp;
            if (cmd_ptr < 0) cmd_ptr += cmd.size();
            if (cmd_ptr >= cmd.size()) cmd_ptr -= cmd.size();
            }
            break;
        case jump_to_ifz:
            if (data == 0) {
                signed char jmp = cmd.at(cmd_ptr);
                cmd_ptr += jmp;
                if (cmd_ptr < 0) cmd_ptr += cmd.size();
                if (cmd_ptr >= cmd.size()) cmd_ptr -= cmd.size();
            }
            break;
        case jump_to_ifnz:
            if (data != 0) {
                signed char jmp = cmd.at(cmd_ptr);
                cmd_ptr += jmp;
                if (cmd_ptr < 0) cmd_ptr += cmd.size();
                if (cmd_ptr >= cmd.size()) cmd_ptr -= cmd.size();
            }
            break;
        case restart:
            cmd_ptr = 0; // and searchStart();
        case end:
            searchStart();
            break;
        //sensor group
        case eye_up_distance:
            data = map->getDistance(coord, Up);
            break;
        case eye_down_distance:
            data = map->getDistance(coord, Down);
            break;
        case eye_left_distance:
            data = map->getDistance(coord, Left);
            break;
        case eye_right_distance:
            data = map->getDistance(coord, Right);
            break;
        case touch_up:
            data = map->getTypeNear(coord, Up);
            break;
        case touch_down:
            data = map->getTypeNear(coord, Down);
            break;
        case touch_left:
            data = map->getTypeNear(coord, Left);
            break;
        case touch_right:
            data = map->getTypeNear(coord, Right);
            break;
        //mem group
        case move_mem_left:
            (mem_ptr == 0) ? mem.size() : mem_ptr--;
            break;
        case move_mem_right:
            (mem_ptr == mem.size()) ? 0 : mem_ptr++;
            break;
        case save_to_mem:
            if (mem.size() < mem_ptr)
                for (int i = 0; i < mem_ptr - mem.size(); i++)
                    mem.append(0);
            mem[mem_ptr] = data;
            break;
        case load_from_mem:
            if (mem.size() < mem_ptr)
                for (int i = 0; i < mem_ptr - mem.size(); i++)
                    mem.append(0);
            data = mem[mem_ptr];
            break;
        case add_mem:
            if (mem.size() < mem_ptr)
                for (int i = 0; i < mem_ptr - mem.size(); i++)
                    mem.append(0);
            data += mem[mem_ptr];
            break;
        case sub_mem:
            if (mem.size() < mem_ptr)
                for (int i = 0; i < mem_ptr - mem.size(); i++)
                    mem.append(0);
            data -= mem[mem_ptr];
            break;
        case set_mem_ptr:
            mem_ptr = data;
            mem.reserve(mem_ptr);
            if (mem.size() < mem_ptr)
                for (int i = 0; i < mem_ptr - mem.size(); i++)
                    mem.append(0);
            break;
        //data group
        case data_clear:
            data = 0;
            break;
        case data_inc:
            data++;
            break;
        case data_dec:
            data--;
            if (data < 0) data = 0;
            break;
        //action group
        case action_move_left:
            emit move(Left); return; break;
        case action_move_right:
            emit move(Right); return; break;
        case action_move_up:
            emit move(Up); return; break;
        case action_move_down:
            emit move(Down); return; break;
        case action_eat_left:
            emit eat(Left); return; break;
        case action_eat_right:
            emit eat(Right); return; break;
        case action_eat_up:
            emit eat(Up); return; break;
        case action_eat_down:
            emit eat(Down); return; break;
        case action_wait:
            emit wait(); return; break;
        case action_suicide:
            emit suicide(); return; break;
        case action_split:
            dir = Direction(qrand() % 4);
            emit split(dir);
//            emit split(Right);
//            emit split(Up);
//            emit split(Down);
            return; break;
        case action_split_mutate:
            //emit splitMutate();
            return; break;
        start:
        nop:
        default: //nop,start
            break;
        }
    }
    emit suicide();//kill myself, if i can't decide what to do!
}

void Animal::searchStart()
{
    if (cmd_ptr == cmd.size()) cmd_ptr = 0;
    while ((AnimalCommand(cmd.at(cmd_ptr++)) != start) || (cmd_ptr == cmd.size())) {}
    if (cmd_ptr == cmd.size()) cmd_ptr = 0;
}


QList<char> Animal::compile(QList<AnimalCommand> acmd)
{
    QList<char> out;
    AnimalCommand cmd;
    foreach (cmd,acmd){
        out.append((char) cmd);
    }
    return out;
}

Animal *Animal::loadAnimal(QString filename)
{
    QFile fin(filename);
    if (!fin.open(QIODevice::ReadOnly)) return NULL;
    QDataStream ds(&fin);
    QList<char> cmds2,mems2;
    quint32 cmd_ptr2,mem_ptr2, cmd_cnt, mem_cnt;
    //can't save lists TODO
    ds >> cmd_ptr2 >> mem_ptr2;
    ds >> cmd_cnt; cmds2.reserve(cmd_cnt);//cmds2.size();
    ds >> mem_cnt; mems2.reserve(mem_cnt);//mems.size();
    quint8 c;
    for (quint32 i=0; i<cmd_cnt; i++){
        ds >> c;
        cmds2.append(c);
    }
    for (quint32 i=0; i<mem_cnt; i++){
        ds >> c;
        mems2.append(c);
    }
    fin.close();
    return (new Animal(cmds2,mems2,cmd_ptr2,mem_ptr2));
}

void Animal::saveAnimal(QString filename)
{
    saveAnimal(filename,cmd,mem,cmd_ptr,mem_ptr);
}

void Animal::saveAnimal(QString filename, QList<char> cmds, QList<char> mems, int cmd_start_ptr, int mem_start_ptr)
{
    QFile fout(filename);
    if (!fout.open(QIODevice::WriteOnly)) return;
    QDataStream ds(&fout);
    //ds << cmds << mems << cmd_start_ptr << mem_start_ptr;
    ds << cmd_start_ptr << mem_start_ptr;
    ds << cmds.size();
    ds << mems.size();
    foreach (quint8 c, cmds) ds << c;
    foreach (quint8 c, mems) ds << c;
    fout.close();
}

Animal *Animal::cloneAnimal()
{
    return new Animal(this->cmd,this->mem,0,0);
}
