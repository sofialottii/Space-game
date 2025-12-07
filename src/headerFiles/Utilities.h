#pragma once
#include "strutture.h"
#include "lib.h"
vec2 randomPosition(int width, int height);

void findBB(Figura* fig);
void updateBB(Figura* fig);
void findBB_Curva(Curva* obj);
void updateBB_Curva(Curva* obj);

bool checkCollision(Figura obj1, Curva obj2);
void changePlayerColor(Curva& player, vec4 newColor);

void updateRotation(Curva curva, float cursorX, float cursorY);
void aggiornaProiettile(float deltaTime);
void aggiornaNemico(float deltaTime);
void aggiornaProiettileNemico(float deltaTime);

void difficoltaPartita(int enemyLife, int numberOfEnemies, int playerLife);
void resetGame();