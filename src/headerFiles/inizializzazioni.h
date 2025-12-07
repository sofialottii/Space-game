#pragma once


void INIT_SHADER(void);

void INIT_VAO(Figura* fig);
void INIT_VAO_DYNAMIC(Figura* fig);
void INIT_VAO_DYNAMIC_NULL(Figura* fig);
void UPDATE_VAO_DYNAMIC(Figura* fig);

/* curve di Hermite */
void INIT_VAO_Curva(Curva* fig);
void INIT_VAO_DYNAMIC_Curva(Curva* fig);
void UPDATE_VAO_Curva(Curva* fig);