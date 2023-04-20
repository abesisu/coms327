#include <string>
#include <vector>

#include "data.h"

class Pokemon {
private:
    int pokemon_id;
    int species_id;
    std::string name;
    int gender;
    int is_shiny;
    int level;
    int hp;
    int max_hp;
    int attack;
    int defense;
    int special_attack;
    int special_defense;
    int speed;
    int base_speed;
    int moves[2];
    int types[2];
public:
    Pokemon(int pokemon_id,
            int species_id,
            std::string name,
            int gender,
            int is_shiny,
            int level,
            int hp,
            int attack,
            int defense,
            int special_attack,
            int special_defense,
            int speed,
            int base_speed,
            int moves[2],
            int types[2]) : moves(), types()
    {
        this->pokemon_id = pokemon_id;
        this->species_id = species_id;
        this->name = name;
        this->gender = gender;
        this->is_shiny = is_shiny;
        this->level = level;
        this->hp = hp;
        this->max_hp = hp;
        this->attack = attack;
        this->defense = defense;
        this->special_attack = special_attack;
        this->special_defense = special_defense;
        this->speed = speed;
        this->base_speed = base_speed;
        this->moves[0] = moves[0];
        this->moves[1] = moves[1];
        this->types[0] = types[0];
        this->types[1] = types[1];
    }
    ~Pokemon() {}
    int get_pokemon_id() { return pokemon_id; }
    std::string get_name() { return name; }  
    int get_gender() { return gender; }
    int get_is_shiny() { return is_shiny; }
    int get_level() { return level; }
    int get_hp() { return hp; }
    void set_hp(int hp) { this->hp = hp; }
    void damage_hp(int damage) 
    { 
        if (this->hp - damage > 0) {
            this->hp -= damage;
        } else {
            this->hp = 0;
        }
    }
    int get_max_hp() { return max_hp; }
    int get_attack() { return attack; }
    int get_defense() { return defense; }
    int get_special_attack() { return special_attack; }
    int get_special_defense() { return special_defense; }
    int get_speed() { return speed; }
    int get_base_speed() { return base_speed; }
    int get_move(int index) { return moves[index]; }
    int get_type(int index) { return types[index]; }
};

Pokemon *generate_pokemon(Data *data, 
                          int pokemon_id, 
                          int gender, 
                          int level, 
                          int is_shiny,
                          int hp_iv,
                          int attack_iv,
                          int defense_iv,
                          int speed_iv,
                          int special_attack_iv,
                          int special_defense_iv);

int calculate_level(int manhattan_distance);

void generate_npc_party(Data *data, Pokemon *party[6], int manhattan_distance);
