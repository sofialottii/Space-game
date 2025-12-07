#pragma once
#pragma warning(disable:4996)

#include "strutture.h"
#include "Utilities.h"
#include "init_geometrie.h"
#include "gestione_curve.h"
#define PI 3.14159265358979323
#define NUM_RIGHE_FILE 1000

void INIT_TRIANGLE(Figura* fig) {
    fig->vertices.push_back(vec3(-0.5f, -0.5f, 0.0f));
    fig->vertices.push_back(vec3(0.5f, -0.5f, 0.0f));
    fig->vertices.push_back(vec3(0.0f, 0.5f, 0.0f));

    fig->colors.push_back(vec4(1.0, 0.0, 0.0, 1.0));
    fig->colors.push_back(vec4(0.0, 1.0, 0.0, 1.0));
    fig->colors.push_back(vec4(0.0, 0.0, 1.0, 1.0));

    fig->nv = fig->vertices.size();
    fig->render = GL_TRIANGLE_FAN;
}

void INIT_CIRCLE(float cx, float cy, float raggiox, float raggioy, Figura* fig, float r, float g, float b) {

    int i;
    float stepA = (2 * PI) / fig->nTriangles;
    float t, xx, yy;

    fig->vertices.push_back(vec3(cx, cy, 0.0));

    fig->colors.push_back(vec4(r, g, b, 1.0));

    for (i = 0; i <= fig->nTriangles; i++)
    {
        t = (float)i * stepA;
        xx = cx + raggiox * cos(t);
        yy = cy + raggioy * sin(t);
        fig->vertices.push_back(vec3(xx, yy, 0.0));
        //Colore 
        fig->colors.push_back(vec4(0.0, 0.0, 0.0, 0.0));
    }
    findBB(fig);

    fig->vertices.push_back(vec3(fig->min_BB_obj.x, fig->min_BB_obj.y, 0.0));
    fig->colors.push_back(vec4(0.0, 0.0, 0.0, 0.0));
    fig->vertices.push_back(vec3(fig->max_BB_obj.x, fig->min_BB_obj.y, 0.0));
    fig->colors.push_back(vec4(0.0, 0.0, 0.0, 0.0));
    fig->vertices.push_back(vec3(fig->max_BB_obj.x, fig->max_BB_obj.y, 0.0));
    fig->colors.push_back(vec4(0.0, 0.0, 0.0, 0.0));
    fig->vertices.push_back(vec3(fig->min_BB_obj.x, fig->max_BB_obj.y, 0.0));
    fig->colors.push_back(vec4(0.0, 0.0, 0.0, 0.0));


    fig->nv = fig->vertices.size();
    fig->render = GL_TRIANGLE_FAN;
}


void INIT_HEART(float cx, float cy, float raggiox, float raggioy, Figura* fig)
{


    int i;
    float t, xx, yy;
    float stepA = (2 * PI) / fig->nTriangles;
    fig->vertices.push_back(vec3(cx, cy, 0.0));

    fig->colors.push_back(vec4(1.0, 1.0, 1.0, 1.0));

    for (i = 0; i <= fig->nTriangles; i++)
    {
        t = (float)i * stepA;
        xx = cx + raggiox * (16 * pow(sin(t), 3));
        yy = cy + raggioy * ((13 * cos(t) - 5 * cos(2 * t) - 2 * cos(3 * t) - cos(4 * t)));
        fig->vertices.push_back(vec3(xx, yy, 0.0));
        fig->colors.push_back(vec4(1.0, 0.0, 0.0, 1.0));
    }

    findBB(fig);
    fig->vertices.push_back(vec3(fig->min_BB_obj.x, fig->min_BB_obj.y, 0.0));
    fig->colors.push_back(vec4(1.0, 0.0, 0.0, 1.0));
    fig->vertices.push_back(vec3(fig->max_BB_obj.x, fig->min_BB_obj.y, 0.0));
    fig->colors.push_back(vec4(1.0, 0.0, 0.0, 1.0));
    fig->vertices.push_back(vec3(fig->max_BB_obj.x, fig->max_BB_obj.y, 0.0));
    fig->colors.push_back(vec4(1.0, 0.0, 0.0, 1.0));
    fig->vertices.push_back(vec3(fig->min_BB_obj.x, fig->max_BB_obj.y, 0.0));
    fig->colors.push_back(vec4(1.0, 0.0, 0.0, 1.0));

    fig->nv = fig->vertices.size();
    fig->render = GL_TRIANGLE_FAN;

    fig->nv = fig->vertices.size();
    fig->render = GL_TRIANGLE_FAN;

}

void INIT_PIANO(Figura* fig)
{
    fig->vertices.clear();

    fig->vertices.push_back(vec3(-0.5, -0.5, 0.0));
    fig->colors.push_back(vec4(0.0, 0.0, 0.0, 1.0));
    fig->vertices.push_back(vec3(0.5, -0.5, 0.0));
    fig->colors.push_back(vec4(1.0, 0.0, 0.0, 1.0));
    fig->vertices.push_back(vec3(0.5, 0.5, 0.0));
    fig->colors.push_back(vec4(1.0, 0.0, 0.0, 1.0));
    fig->vertices.push_back(vec3(-0.5, 0.5, 0.0));
    fig->colors.push_back(vec4(1.0, 0.0, 0.0, 1.0));

    fig->nv = fig->vertices.size();
    fig->render = GL_TRIANGLE_FAN;

}

void INIT_PLAYER(Curva* curva){

    float* t;
    float step_t;
    int i;
    struct Dati {
        float x;
        float y;
        float z;
    };

    FILE* file;
    errno_t err = fopen_s(&file, "player1.txt", "r");
    if (err != 0) {
        perror("Impossibile aprire il file");
        
    } else {
        /* vettore per memorizzare i dati */
        struct Dati dati[NUM_RIGHE_FILE];

        int riga = 0;
        while (fscanf_s(file, "%f %f %f", &dati[riga].x, &dati[riga].y, &dati[riga].z) == 3) {

            riga++; //l'indice della riga viene aumentato

            if (riga >= NUM_RIGHE_FILE) {
                printf("Array riempito. Ci sono troppe righe nel file.\n");
                break;
            }
        }

        fclose(file);

        for (int i = 0; i < riga; i++) {
            curva->CP.push_back(vec3(dati[i].x, dati[i].y, dati[i].z));
            curva->colCP.push_back(vec4(0.0, 0.0, 0.0, 1.0));
            curva->Derivata.push_back(vec3(0.0, 0.0, 0.0));
        }
        curva->ncp = curva->CP.size();

        t = new float[curva->CP.size()];
        step_t = 1.0 / (curva->CP.size() - 1);
        for (i = 0; i < curva->CP.size(); i++)
            t[i] = (float)i * step_t;

        t[curva->CP.size()] = 1.0;
        vec3 centro = vec3(0.0, 0.0, 0.0);
        CostruisciHermitePlayer(t, curva, centro);
    
        curva->nv = curva->vertices.size();
        curva->render = GL_TRIANGLE_FAN;

     }

    
}

void INIT_NEMICO(Curva* curva) {
    float* t;
    float step_t;
    int i;
    struct Dati {
        float x;
        float y;
        float z;
    };

    FILE* file;

    errno_t err = fopen_s(&file, "nemico.txt", "r");
    if (err != 0) {
        perror("Impossibile aprire il file");

    }
    else {
        /* vettore per memorizzare i dati */
        struct Dati dati[NUM_RIGHE_FILE];

        int riga = 0;
        while (fscanf_s(file, "%f %f %f", &dati[riga].x, &dati[riga].y, &dati[riga].z) == 3) {

            riga++; //l'indice della riga viene aumentato

            if (riga >= NUM_RIGHE_FILE) {
                printf("Array riempito. Ci sono troppe righe nel file.\n");
                break;
            }
        }

        fclose(file);

        for (int i = 0; i < riga; i++) {
            curva->CP.push_back(vec3(dati[i].x, dati[i].y, dati[i].z));
            curva->colCP.push_back(vec4(0.0, 0.0, 0.0, 1.0));
            curva->Derivata.push_back(vec3(0.0, 0.0, 0.0));
        }
        curva->ncp = curva->CP.size();

        t = new float[curva->CP.size()];
        step_t = 1.0 / (curva->CP.size() - 1);
        for (i = 0; i < curva->CP.size(); i++)
            t[i] = (float)i * step_t;

        t[curva->CP.size()] = 1.0;
        vec3 centro = vec3(0.0, 0.0, 0.0);
        CostruisciHermiteNemico(t, curva, centro);

        curva->nv = curva->vertices.size();
        curva->render = GL_TRIANGLE_FAN;

    }

}



