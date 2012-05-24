#include "animal.h"



void Animal::onTick()
{
    run();
}

void Animal::run()
{
    while () {
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
            signed char jmp = cmd.at(cmd_ptr);
            cmd_ptr += jmp;
            if (cmd_ptr < 0) cmd_ptr += cmd.size();
            if (cmd_ptr >= cmd.size()) cmd_ptr -= cmd.size();
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
            data = map->getDistance(coord, up);
            break;
        case eye_down_distance:
            data = map->getDistance(coord, down);
            break;
        case eye_left_distance:
            data = map->getDistance(coord, left);
            break;
        case eye_right_distance:
            data = map->getDistance(coord, right);
            break;
        case touch_up:
            data = map->getTypeNear(coord, up);
            break;
        case touch_down:
            data = map->getTypeNear(coord, down);
            break;
        case touch_left:
            data = map->getTypeNear(coord, left);
            break;
        case touch_right:
            data = map->getTypeNear(coord, right);
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
            emit move(left); return; break;
        case action_move_right:
            emit move(right); return; break;
        case action_move_up:
            emit move(up); return; break;
        case action_move_down:
            emit move(down); return; break;
        case action_eat_left:
            emit eat(left); return; break;
        case action_eat_right:
            emit eat(right); return; break;
        case action_eat_up:
            emit eat(up); return; break;
        case action_eat_down:
            emit eat(down); return; break;
        case action_wait:
            emit wait(); return; break;
        case action_suicide:
            emit suicide(); return; break;
        case action_split:
            emit split(); return; break;
        case action_split_mutate:
            emit splitMutate(); return; break;
        default: //nop,start
            break;
        }
    }
}

void Animal::searchStart()
{
    if (cmd_ptr == cmd.size()) cmd_ptr = 0;
    while ((AnimalCommand(cmd.at(cmd_ptr++)) != start) || (cmd_ptr == cmd.size())) {}
    if (cmd_ptr == cmd.size()) cmd_ptr = 0;
}
