#include "animal.h"
#include <QFile>
#include <QDataStream>
#include <QTextStream>

void Animal::onTick()
{
    run();
}

void Animal::run()
{
    int i = MAX_STEPS;
    Direction dir;
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
            checkMemSize(mem_ptr, true);
            mem[mem_ptr] = data;
            break;
        case load_from_mem:
            checkMemSize(mem_ptr, true);
            data = mem[mem_ptr];
            break;
        case add_mem:
            checkMemSize(mem_ptr, true);
            data += mem[mem_ptr];
            break;
        case sub_mem:
            checkMemSize(mem_ptr, true);
            data -= mem[mem_ptr];
            break;
        case set_mem_ptr:
            mem_ptr = data;
            mem.reserve(mem_ptr);
            checkMemSize(mem_ptr, true);
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
            //if (data < 0) data = 0;
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
        case start:
        case nop:
        default: //nop,start
            break;
        }
    }
    emit suicide();//kill myself, if i can't decide what to do!
}

void Animal::searchStart()
{
    if (cmd.size() == 0){
        cmd.append(char(action_suicide));
        cmd_ptr = 0;
        return;
    }
    if (cmd_ptr == cmd.size()) cmd_ptr = 0;
    //while ((AnimalCommand(cmd.at(cmd_ptr++)) != start) || (cmd_ptr == cmd.size())) {}
    //while ((cmd_ptr != cmd.size()) || (AnimalCommand(cmd.at(cmd_ptr++)) != start)) {}
    cmd_ptr = cmd.indexOf(start);
    if (cmd_ptr == -1) cmd_ptr = 0;
}

inline bool Animal::checkMemSize(int mem_size, bool expand)
{
    if (expand){
        if (mem.size() < mem_size+1){
            mem.reserve(mem_size+1);
            int count = mem_size - mem.size() + 1;
            for (int i = 0; i < count; i++)
                mem.append(0);
        }
        return true;
    }
    else
        return (mem.size() < mem_size+1);
}


QList<quint8> Animal::compile(QList<AnimalCommand> acmd)
{
    QList<quint8> out;
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
    QList<quint8> cmds2,mems2;
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
    if (cmds2.size() == 0) return NULL;
    return (new Animal(cmds2,mems2,cmd_ptr2,mem_ptr2));
}

void Animal::saveAnimal(QString filename, bool with_code)
{
    saveAnimal(filename,cmd,mem,cmd_ptr,mem_ptr, with_code);
}

void Animal::saveAnimal(QString filename, QList<quint8> cmds, QList<quint8> mems, int cmd_start_ptr, int mem_start_ptr, bool with_code)
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
    if (with_code){
        QFile fout(filename.append(".code"));
        if (!fout.open(QIODevice::WriteOnly)) return;
        QTextStream ts(&fout);
        QString code_line;
        int cmd_ptr_2=0;
        while (cmd_ptr_2 != cmds.size()) {
            char cur_cmd = cmds.at(cmd_ptr_2++);
            switch (AnimalCommand(cur_cmd)) {
            case move_cmd_left:
                code_line = "move_cmd_left";
                break;
            case move_cmd_right:
                code_line = "move_cmd_right";
                break;
            case jump_to:
                {
                    signed char jmp = cmds.at(cmd_ptr_2++);
                    code_line = "jump " + QString::number(jmp);
                }
                break;
            case jump_to_ifz:
                {
                    signed char jmp = cmds.at(cmd_ptr_2++);
                    code_line = "jump " + QString::number(jmp) + " if zero";
                }
                break;
            case jump_to_ifnz:
                {
                    signed char jmp = cmds.at(cmd_ptr_2++);
                    code_line = "jump " + QString::number(jmp) + " if not zero";
                }
                break;
            case restart:
                code_line = "restart";
            case end:
                code_line = "end";
                break;
            //sensor group
            case eye_up_distance:
                code_line = "eye_up_distance";
                break;
            case eye_down_distance:
                code_line = "eye_down_distance";
                break;
            case eye_left_distance:
                code_line = "eye_left_distance";
                break;
            case eye_right_distance:
                code_line = "eye_right_distance";
                break;
            case touch_up:
                code_line = "touch_up";
                break;
            case touch_down:
                code_line = "touch_down";
                break;
            case touch_left:
                code_line = "touch_left";
                break;
            case touch_right:
                code_line = "touch_right";
                break;
            //mem group
            case move_mem_left:
                code_line = "move_mem_left";
                break;
            case move_mem_right:
                code_line = "move_mem_right";
                break;
            case save_to_mem:
                code_line = "save_to_mem";
                break;
            case load_from_mem:
                code_line = "load_from_mem";
                break;
            case add_mem:
                code_line = "add_mem";
                break;
            case sub_mem:
                code_line = "sub_mem";
                break;
            case set_mem_ptr:
                code_line = "set_mem_ptr";
                break;
            //data group
            case data_clear:
                code_line = "data_clear";
                break;
            case data_inc:
                code_line = "data_inc";
                break;
            case data_dec:
                code_line = "data_dec";
                break;
            //action group
            case action_move_left:
                code_line = "action_move_left";
                break;
            case action_move_right:
                code_line = "action_move_right";
                break;
            case action_move_up:
                code_line = "action_move_up";
                break;
            case action_move_down:
                code_line = "action_move_down";
                break;
            case action_eat_left:
                code_line = "action_eat_left";
                break;
            case action_eat_right:
                code_line = "action_eat_right";
                break;
            case action_eat_up:
                code_line = "action_eat_up";
                break;
            case action_eat_down:
                code_line = "action_eat_down";
                break;
            case action_wait:
                code_line = "action_wait";
                break;
            case action_suicide:
                code_line = "action_suicide";
                break;
            case action_split:
                code_line = "action_split";
                break;
            case action_split_mutate:
                code_line = "action_split_mutate";
                break;
            case start:
                code_line = "start";
                break;
            case nop:
                code_line = "nop";
                break;
            default: //nop,start
                code_line = "unknown opcode";
                break;
            }
            ts << code_line << endl;
        }
        fout.close();
    }
}

Animal *Animal::cloneAnimal()
{
    return new Animal(this->cmd,this->mem,0,0);
}
