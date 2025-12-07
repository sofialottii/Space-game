#pragma once

#include "strutture.h"
void INIT_TRIANGLE(Figura* fig);
void INIT_CIRCLE(float cx, float cy, float raggiox, float raggioy, Figura* fig, float r, float g, float b);
void INIT_BUTTERFLY(float cx, float cy, float raggiox, float raggioy, Figura* fig);
void INIT_HEART(float cx, float cy, float raggiox, float raggioy, Figura* fig);
void INIT_PIANO(Figura* fig);

void INIT_PLAYER(Curva* curva);
void INIT_PROIETTILE(Figura* fig);
void INIT_NEMICO(Curva* curva);
