#include <iostream>
#include <cmath>
#include "main.hpp"

static const unsigned int TRACKING_TURNS = 3;

Monster::Monster(unsigned int x, unsigned int y, unsigned int ch, 
    const TCODColor &color, std::string name, bool blocks, bool canDie,
    unsigned int fovRadius, bool canExplore, double maxHp, 
    double hp, double mp, double atk, double defense, unsigned int spd) :
    
    Entity(x, y, ch, color, name, blocks, canDie, fovRadius, canExplore, maxHp, 
    hp, mp, atk, defense, spd) {
}

void Monster::update() {
    if (canDie && isDead()) {
        return;
    }
    
    engine.map->computeFOV(*this);
    
    if (canSee(engine.player->x, engine.player->y)) {
        moveCount = TRACKING_TURNS;
    }
    else {
        --moveCount;
    }
    
    if (moveCount > 0) {
        moveOrAttack(engine.player->x, engine.player->y);
    }
}

void Monster::moveOrAttack(unsigned int target_x, unsigned int target_y) {
    int dx = target_x - x;
    int dy = target_y - y;
    int step_dx = ( dx > 0 ? 1 : -1 );
    int step_dy = ( dy > 0 ? 1 : -1 );
    double distance = sqrt(dx*dx + dy*dy);
    
    if (distance >= 1.5) {
        dx = (int)(round(dx / distance));
        dy = (int)(round(dy / distance));
    
        if ( engine.map->canWalk(x + dx, y + dy) ) {
            x += dx;
            y += dy;
        }
        else if (engine.map->canWalk(x + step_dx, y)) {
            x += step_dx;
        }
        else if (engine.map->canWalk(x, y + step_dy)) {
            y += step_dy;
        }
    }
    else {
        attack(*(engine.player));
    }
}

void Monster::die() {
    Entity::die();
    std::cout << "You killed " << name << ".\n" << std::endl;
}