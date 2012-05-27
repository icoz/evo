#include "animal.h"



void Animal::onTick()
{
    run();
}

void Animal::run()
{
    int i = 10000;
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
            mem[mem_ptr] = data;
            break;
        case load_from_mem:
            data = mem[mem_ptr];
            break;
        case add_mem:
            data += mem[mem_ptr];
            break;
        case sub_mem:
            data -= mem[mem_ptr];
            break;
        case set_mem_ptr:
            mem_ptr = data;
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
            //emit split();
            return; break;
        case action_split_mutate:
            //emit splitMutate();
            return; break;
        default: //nop,start
            break;
        }
    }
    emit wait();
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
