#pragma once
#include "lib.h"
#include "ShaderMaker.h"
#include "strutture.h" 
#include "inizializzazioni.h"
#include "init_geometrie.h"
#include "gestione_interazioni.h"
#include "gestione_curve.h"
#include "Utilities.h"


extern float r, g, b;
extern unsigned int MatProjS, MatModelS, MatProj, MatModel, vec_resS, loc_time, MatProjNemico, MatModelNemico;
extern mat4 Projection;
extern vec2 resolution;
extern int  height, width;
extern float deltaTime;
extern vector<float> timerFig;
extern vector<Figura> Scena;

extern Figura background, proiettile, proiettileNemico, bonus;
extern std::vector<Figura> proiettili, proiettiliNemici;
extern Curva nemico, player;
extern std::vector<Curva> nemici;
extern Game game;

float deltaBonus = 0;


void renderingBg(float currentFrame, int frame) {
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    glUseProgram(Scena[0].progamId);
    Scena[0].Model = mat4(1.0);
    Scena[0].Model = scale(Scena[0].Model, background.scale);

    glUniformMatrix4fv(MatProjS, 1, GL_FALSE, value_ptr(Projection));
    glUniformMatrix4fv(MatModelS, 1, GL_FALSE, value_ptr(Scena[0].Model));
    glUniform2fv(vec_resS, 1, value_ptr(resolution));
    glUniform1f(loc_time, currentFrame);
    
    //Questa istruzione "lega" o "attiva" il Vertex Array Object (VAO) di ogni struttura di tipo Figura memorizzata nel vector Scena .
    glBindVertexArray(Scena[0].VAO);
    //Fa il rendering della struttura i-esima
    glDrawArrays(Scena[0].render, 0, Scena[0].nv);
}

void renderingPlayer() {
    glUseProgram(player.programId);
    player.Model = mat4(1.0);
    player.Model = translate(player.Model, vec3(player.position.x, player.position.y, 0.0));
    player.Model = scale(player.Model, player.scale);
    glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(player.Model));
    glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(Projection));
    glBindVertexArray(player.VAO);
    glDrawArrays(player.render, 0, player.nv);
}

void renderingNemico() {
    static float deltadelta = 0.0f;
    for (auto itN = nemici.begin(); itN != nemici.end(); ) {
        auto& nemico = *itN;
        glUseProgram(nemico.programId);
        nemico.Model = mat4(1.0);
        nemico.Model = translate(nemico.Model, vec3(nemico.position.x, nemico.position.y, 0.0));
        float angoloInRadianti = glm::radians(53.0f);
        nemico.Model = rotate(nemico.Model, angoloInRadianti, vec3(0.0, 0.0, 1.0));
        nemico.Model = scale(nemico.Model, nemico.scale);
        if (nemico.isAlive == false) {
            nemico.Model = translate(nemico.Model, vec3(100000.0f, 1000000.0f, 0.0f)); // Fuori dalla vista
        }
        glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(nemico.Model));
        glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(Projection));
        glBindVertexArray(nemico.VAO);
        glDrawArrays(nemico.render, 0, nemico.nv);
        bool nemicoMorto = false;

        findBB_Curva(&nemico);
        //findBB(&proiettile);

        if (deltadelta >= game.SpeedEnemyBullets && nemico.isAlive == true) {
            Figura newBullet = proiettileNemico;
            newBullet.position = nemico.position;
            proiettiliNemici.push_back(newBullet);
            deltadelta = 0.0f;
        }
        deltadelta += deltaTime;


        ++itN;

    }
}

void rendering(float currentFrame, int frame) {

    int i;

    renderingBg(currentFrame, frame);

    /* disegno il player */

    renderingPlayer();

    /* disegno i nemici */

    aggiornaNemico(deltaTime);
    
    renderingNemico(); //qui avviene anche la creazione di nuovi proiettili nemici

    /* disegno i proiettili dei nemici */

    aggiornaProiettileNemico(deltaTime);

    for (auto it = proiettiliNemici.begin(); it != proiettiliNemici.end(); ) {
        auto& proiettileNemico = *it;
        glUseProgram(proiettileNemico.progamId);
        proiettileNemico.Model = mat4(1.0);
        proiettileNemico.Model = translate(proiettileNemico.Model, vec3(proiettileNemico.position.x, proiettileNemico.position.y, 0.0));
        proiettileNemico.Model = scale(proiettileNemico.Model, proiettileNemico.scale);
        glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(proiettileNemico.Model));
        glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(Projection));
        glBindVertexArray(proiettileNemico.VAO);
        glDrawArrays(proiettileNemico.render, 0, proiettileNemico.nv);

        bool proiettileNemicoMorto = false;

        findBB(&proiettileNemico);

        /* controlli */
        findBB_Curva(&player);
        if (checkCollision(proiettileNemico, player)) {
            player.vita = player.vita - 1;
            it = proiettiliNemici.erase(it);
            proiettileNemicoMorto = true;


            vec4 hitColor = vec4(0.8f, 0.0f, 0.0f, 1.0f); // Colore rosso
            if (player.vita == 1) {
                changePlayerColor(player, hitColor);
            }

        }


        /* gestire se vita nemico = 0 */
        if (player.vita <= 0) {
            player.isAlive = false;
        }

        if (!proiettileNemicoMorto) {
            ++it;
        }

    }
    
    /* disegno il proiettile del player */

    aggiornaProiettile(deltaTime);

    for (auto it = proiettili.begin(); it != proiettili.end(); ) {
        auto& proiettile = *it;
        glUseProgram(proiettile.progamId);
        proiettile.Model = mat4(1.0);
        proiettile.Model = translate(proiettile.Model, vec3(proiettile.position.x, proiettile.position.y, 0.0));
        proiettile.Model = scale(proiettile.Model, proiettile.scale);
        glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(proiettile.Model));
        glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(Projection));
        glBindVertexArray(proiettile.VAO);
        glDrawArrays(proiettile.render, 0, proiettile.nv);

        bool proiettileMorto = false;

        findBB(&proiettile);
        
        
        for (int k = 0; k < game.numberOfEnemies; k++) {
            if (nemici[k].isAlive) {
                findBB_Curva(&nemici[k]);
                if (checkCollision(proiettile, nemici[k])) {
                    nemici[k].vita = nemici[k].vita - 1;
                    game.score += 10;
                    it = proiettili.erase(it);
                    proiettileMorto = true;
                }
                /* gestire se vita nemico = 0 */
                if (nemici[k].vita <= 0) {
                    nemici[k].isAlive = false;
                }
            }
        }
        
        if (!proiettileMorto) {
            ++it;
        }
    }


    /* infine gestisco il bonus */
    if (deltaBonus >= 3.0f && bonus.isalive == false) {
        
        glUseProgram(bonus.progamId);
        bonus.Model = mat4(1.0f);
        bonus.Model = translate(bonus.Model, vec3(bonus.position.x, bonus.position.y, 0.0));
        bonus.Model = scale(bonus.Model, bonus.scale);
        glUniformMatrix4fv(MatModel, 1, GL_FALSE, value_ptr(bonus.Model));
        glUniformMatrix4fv(MatProj, 1, GL_FALSE, value_ptr(Projection));
        glBindVertexArray(bonus.VAO);
        glDrawArrays(bonus.render, 0, bonus.nv);
        bonus.isalive = true;

    }

    deltaBonus += deltaTime;


}