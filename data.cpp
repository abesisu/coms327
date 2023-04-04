#include <fstream>
#include <iostream>
#include "data.h"

std::ifstream *get_file(std::string filename)
{
    std::ifstream *file = new std::ifstream(); // declare new instance so the file persists outside of the function.
    std::string path = "/share/cs327/pokedex/pokedex/data/csv/" + filename;
    std::string home_path;
    if (char *home = std::getenv("HOME")) {
        home_path = std::string(home) + "/" + filename;
    }
    std::string local_path = "/Users/abescheideman/Documents/ISUClasses/Spring23/coms327/pokemon/pokedex/pokedex/data/csv/" + filename;

    file->open(path);
    if (!file->is_open()) {
        file->open(home_path);
    }
    if (!file->is_open() && !home_path.empty()) {
        file->open(local_path);
    }
    if (!file->is_open()) {
        delete file;
        file = 0;
    }

    return file;
}

int Data::get_pokemon(Data &data)
{
    int id;
    std::string identifier;
    int species_id;
    int height;
    int weight;
    int base_experience;
    int order;
    int is_default;
    int got_data = 0;
    std::ifstream *pokemon_file = get_file("pokemon.csv");
    if (!pokemon_file) {
        return got_data;
    }

    std::string line;
    std::getline(*pokemon_file, line);
    while (std::getline(*pokemon_file, line, ',')) {
        id = std::stoi(line);
        std::getline(*pokemon_file, line, ',');
        std::string identifier = line;
        std::getline(*pokemon_file, line, ',');
        species_id = std::stoi(line);
        std::getline(*pokemon_file, line, ',');
        height = std::stoi(line);
        std::getline(*pokemon_file, line, ',');
        weight = std::stoi(line);
        std::getline(*pokemon_file, line, ',');
        base_experience = std::stoi(line);
        std::getline(*pokemon_file, line, ',');
        order = std::stoi(line);
        std::getline(*pokemon_file, line);
        is_default = std::stoi(line);
        Pokemon *p = new Pokemon(id,
                                       identifier,
                                       species_id,
                                       height,
                                       weight,
                                       base_experience,
                                       order,
                                       is_default);
        data.pokemon.push_back(*p);
    }
    got_data = 1;

    return got_data;
}

int Data::get_pokemon_types(Data &data)
{
    return 0;
}

int Data::get_type_names(Data &data)
{
    return 0;
}

int Data::get_pokemon_species(Data &data)
{
 return 0;
}

int Data::get_pokemon_stats(Data &data)
{
 return 0;
}

int Data::get_stats(Data &data)
{
 return 0;
}

int Data::get_pokemon_moves(Data &data)
{
 return 0;
}

int Data::get_moves(Data &data)
{
 return 0;
}

int Data::get_experience(Data &data)
{
 return 0;
}

int Data::get_data(Data &data)
{
    int got_data;
    got_data = get_pokemon(data);
    if (!got_data) {
        std::cerr << "Error: Could not get pokemon data." << std::endl;
    }
    got_data = get_pokemon_types(data);
    if (!got_data) {
        std::cerr << "Error: Could not get pokemon types data." << std::endl;
    }
    got_data = get_type_names(data);
    if (!got_data) {
        std::cerr << "Error: Could not get type names data." << std::endl;
    }
    got_data = get_pokemon_species(data);
    if (!got_data) {
        std::cerr << "Error: Could not get pokemon species data." << std::endl;
    }
    got_data = get_pokemon_stats(data);
    if (!got_data) {
        std::cerr << "Error: Could not get pokemon stats data." << std::endl;
    }
    got_data = get_stats(data);
    if (!got_data) {
        std::cerr << "Error: Could not get stats data." << std::endl;
    }
    got_data = get_pokemon_moves(data);
    if (!got_data) {
        std::cerr << "Error: Could not get pokemon moves data." << std::endl;
    }
    got_data = get_moves(data);
    if (!got_data) {
        std::cerr << "Error: Could not get moves data." << std::endl;
    }
    got_data = get_experience(data);
    if (!got_data) {
        std::cerr << "Error: Could not get experience data." << std::endl;
    }

    return got_data;
}
