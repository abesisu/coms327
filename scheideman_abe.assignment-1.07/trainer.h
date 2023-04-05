#ifndef TRAINER_H
#define TRAINER_H

#include "util.h"

typedef enum trainer_type {
    pc_e,
    hiker_e,
    rival_e,
    pacer_e,
    wanderer_e,
    sentry_e,
    explorer_e,
    swimmer_e,
} trainer_type_e;

class trainer {
    protected:
        trainer_type_e type;
        int next_turn;
        int seq_num;
        coordinate_t pos;
        coordinate_t dir;
    public:
        explicit trainer(trainer_type_e t) : pos(), dir()
        {
            type = t;
            next_turn = 0;
            seq_num = 0;
            pos.x = 0;
            pos.y = 0;
        }
        explicit trainer(trainer *t) : pos(), dir()
        {
            type = t->type;
            next_turn = t->next_turn;
            seq_num = t->seq_num;
            pos = t->pos;
            dir = t->dir;
        }
        ~trainer() {}
        
        trainer_type_e get_type() { return type; }
        void set_next_turn(int next) { next_turn = next; }
        int get_next_turn() const { return next_turn; }
        int get_seq_num() const { return seq_num; }
        void set_pos(coordinate_t p) { pos = p; }
        void set_pos_y(int y) { pos.y = y; }
        void set_pos_x(int x) { pos.x = x; }
        coordinate_t get_pos() { return pos; }
        void set_dir_y(int y) { dir.y = y; }
        void set_dir_x(int x) { dir.x = x; }
        coordinate_t get_dir() { return dir; }
};

class pc : public trainer {
    public:
        pc() : trainer(pc_e) { seq_num = 0; };
        explicit pc(trainer *t) : trainer(t) {}
        ~pc() {}
};

class npc : public trainer {
    public:
        explicit npc(trainer *t) : trainer(t)
        { 
            switch (t->get_type())
            {
                case hiker_e:
                    seq_num = 1;
                    break;
                case rival_e:
                    seq_num = 2;
                    break;
                case pacer_e:
                    seq_num = 3;
                    break;
                case wanderer_e:
                    seq_num = 4;
                    break;
                case sentry_e:
                    seq_num = 5;
                    break;
                case explorer_e:
                    seq_num = 6;
                    break;
                case swimmer_e:
                    seq_num = 7;
                    break;
                default:
                    seq_num = 0;
                    break;
            }
        }
        ~npc() {}
};

#endif
