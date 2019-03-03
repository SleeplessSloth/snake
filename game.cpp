#include <chrono>
#include <random>
#include "game.hpp"

#include <ncurses.h>

void Game::tick()
{
    const static std::chrono::milliseconds TICK_DURATION(500);
    auto last_tick = std::chrono::high_resolution_clock::now();
    Facing input;

    while(true)
    {
        // true if the time of the next tick(last tick + tick duration) is in the past
        if((last_tick + TICK_DURATION) < std::chrono::high_resolution_clock::now())
        {
            update();
            last_tick = std::chrono::high_resolution_clock::now();
        }


        if((input = ui.get_input()) != Facing::null) player->facing = input;
    }
}

void Game::update()
{
    point player_head = player->get();
    if(field->get(player_head) == Object::food)
    {
        field->set(player_head, Object::empty);
        place_food();
        player->lengthen();
    }
    
    player->move(field->field_size);
    ui.display(field, player);
}

void Game::place_food()
{
    // pseudo-random number generator
    static std::mt19937 rng;
    // generate a seed from a random device in the system
    rng.seed(std::random_device()());
    // make a class to distribute the random numbers
    static std::uniform_int_distribution<std::mt19937::result_type> dist(0, field->field_size - 1);

    field->set({(int) dist(rng), (int) dist(rng)}, Object::food);
}

Game::Game(int pField_size)
{
    field = new Field(pField_size);
    player = new Player();

    place_food();
    tick();
}

Game::~Game()
{
    delete field;
    delete player;
}