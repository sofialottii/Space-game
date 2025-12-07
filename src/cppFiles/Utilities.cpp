#pragma once
#include "lib.h"
#include "strutture.h"
#include <random>

extern int height, width;
extern std::vector<Figura> proiettili, proiettiliNemici;
extern std::vector<Curva> nemici;
extern Figura proiettile, proiettileNemico;
extern Curva player;
extern GLFWwindow* window;
extern Game game;
static int q = 0;

vec2 randomPosition(int width, int height) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> disX(20.0f, static_cast<float>(width)-30.0f);
    q++;
    if (q % 4 == 0) {
        return glm::vec2(disX(gen), height / 2.050f);
    }
    if (q % 3 == 0) {
        return glm::vec2(disX(gen), height / 1.720f);
    }
    if (q % 2 == 0) {
        return glm::vec2(disX(gen), height / 1.400f);
    }
    else {
        return glm::vec2(disX(gen), height / 1.150f);
    }
    
}

void findBB(Figura* fig) {
    int n;
    n = fig->vertices.size();
    float minx = fig->vertices[0].x; // Assumiamo che il primo elemento sia il minimo iniziale
    float miny = fig->vertices[0].y; // Assumiamo che il primo elemento sia il minimo iniziale

    float maxx = fig->vertices[0].x; // Assumiamo che il primo elemento sia il minimo iniziale
    float maxy = fig->vertices[0].y; // Assumiamo che il primo elemento sia il minimo iniziale

    for (int i = 1; i < n; i++) {
        if (fig->vertices[i].x < minx) {
            minx = fig->vertices[i].x;
        }
        if (fig->vertices[i].x > maxx) {
            maxx = fig->vertices[i].x;
        }

        if (fig->vertices[i].y < miny) {
            miny = fig->vertices[i].y;
        }

        if (fig->vertices[i].y > maxy) {
            maxy = fig->vertices[i].y;
        }
    }

    fig->min_BB_obj = vec4(minx, miny, 0.0, 1.0);
    fig->max_BB_obj = vec4(maxx, maxy, 0.0, 1.0);

}

void findBB_Curva(Curva* obj) {
    int n = obj->vertices.size();
    float minx = obj->vertices[0].x;
    float miny = obj->vertices[0].y;
    float maxx = obj->vertices[0].x;
    float maxy = obj->vertices[0].y;
    for (int i = 1; i < n; i++)
    {
        if (obj->vertices[i].x < minx) minx = obj->vertices[i].x;
        if (obj->vertices[i].x > maxx) maxx = obj->vertices[i].x;
        if (obj->vertices[i].y < miny) miny = obj->vertices[i].y;
        if (obj->vertices[i].y > maxy) maxy = obj->vertices[i].y;
    }
    obj->min_BB_obj = vec4(minx, miny, 0.0f, 1.0f);
    obj->max_BB_obj = vec4(maxx, maxy, 0.0f, 1.0f);

}

void updateBB(Figura* fig)
{
    //Aggiorno le coordinate del Bounding Box applicandogli la stessa matrice di modellazione della sua shape
    fig->min_BB = fig->min_BB_obj;
    fig->max_BB = fig->max_BB_obj;
    fig->min_BB = fig->Model * fig->min_BB;
    fig->max_BB = fig->Model * fig->max_BB;
}

void updateBB_Curva(Curva* obj) {
    obj->min_BB = obj->min_BB_obj;
    obj->max_BB = obj->max_BB_obj;
    obj->min_BB = obj->Model * obj->min_BB;
    obj->max_BB = obj->Model * obj->max_BB;
}

void calculateBoundingCircle(Curva* obj) {
    glm::vec2 center = (obj->min_BB + obj->max_BB) * 0.5f;
    float radius = glm::distance(obj->min_BB, obj->max_BB) * 0.5f;
    obj->position = center;
    obj->radius = radius;
}

bool checkCollision(Figura obj1, Curva obj2) {
    updateBB(&obj1);
    updateBB_Curva(&obj2);

    calculateBoundingCircle(&obj2);

    glm::vec2 obj1_center = (obj1.min_BB + obj1.max_BB) * 0.5f;
    glm::vec2 obj2_center = obj2.position;

    float distance = glm::distance(obj1_center, obj2_center);
    return distance <= obj2.radius;
}


void updateRotation(Curva curva, float cursorX, float cursorY) {
    // Calcola la differenza tra la posizione della figura e la posizione del cursore
    float dx = cursorX - curva.position.x;
    float dy = cursorY - curva.position.y;

    // Usa atan2 per calcolare l'angolo in radianti
    curva.angle = glm::atan(dy, dx);  // Angolo in radianti

    // Crea la matrice di rotazione
    curva.Model = glm::rotate(glm::mat4(1.0f), curva.angle, glm::vec3(0.0f, 0.0f, 1.0f));  // Rotazione attorno all'asse Z
}

void aggiornaProiettile(float deltaTime) {
    for (auto it = proiettili.begin(); it != proiettili.end();) {
        auto& actualProiettile = *it;
        actualProiettile.position.y += 300.0f * deltaTime;
        float timeFactor = sin(glfwGetTime() * 5.0f);
        actualProiettile.scale = vec3(30.0f, 30.0f, 1.0f);
        //actualProiettile.scale = vec3(30.0f + 10.0f * timeFactor, 30.0f + 10.0f * timeFactor, 1.0f);
        if (actualProiettile.position.y > height) {
            it = proiettili.erase(it);
        }
        else {
            ++it;
        }
    }
}


void aggiornaNemico(float deltaTime) {
    // Calcolo del nuovo movimento pseudocasuale
    for (auto it = nemici.begin(); it != nemici.end();) {
        auto& nemico = *it;
        if (nemico.direzioneDestra == true) {
            nemico.position.x += nemico.velocita * deltaTime;
            if (nemico.position.x >= (width - 20.0f)) {
                nemico.direzioneDestra = false;
            }
        }
        else {
            nemico.position.x -= nemico.velocita * deltaTime;
            if (nemico.position.x <= (20.0f)) {
                nemico.direzioneDestra = true;
            }
        }
        it++;


    }
}

void aggiornaProiettileNemico(float deltaTime) {
    for (auto it = proiettiliNemici.begin(); it != proiettiliNemici.end();) {
        auto& proiettile = *it;
        proiettile.position.y -= 300.0f * deltaTime;
        float timeFactor = sin(glfwGetTime() * 5.0f);
        proiettile.scale = vec3(30.0f, 30.0f, 1.0f);
        //proiettile.scale = vec3(30.0f + 10.0f * timeFactor, 30.0f + 10.0f * timeFactor, 1.0f);
        if (proiettile.position.y < 0 || proiettile.position.y > height) {
            it = proiettiliNemici.erase(it);
        }
        else {
            ++it;
        }
    }
}

/* cambio il colore del player */
void changePlayerColor(Curva& player, vec4 newColor) {
    // Aggiorna tutti i colori con il nuovo colore
    for (auto& color : player.colors) {
        color = newColor;
    }

    // Ricarica il VBO dei colori
    glBindBuffer(GL_ARRAY_BUFFER, player.VBO_colors); // Assicurati che VBO_color sia l'ID del buffer per i colori
    glBufferSubData(GL_ARRAY_BUFFER, 0, player.colors.size() * sizeof(vec4), player.colors.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void difficoltaPartita(int enemyLife, int numberOfEnemies, int playerLife) {
    /* inizializzo l'high score */
    FILE* file;
    errno_t err = fopen_s(&file, "highScore.txt", "r");
    if (err != 0) {
        perror("Impossibile aprire il file");

    }
    else {
        fscanf_s(file, "%d", &game.highScore);
        fclose(file);
    }

    /* inizializzo il gioco di default */
    game.score = 0;
    game.enemyLife = enemyLife;
    game.isGameStarting = false;
    game.numberOfEnemies = numberOfEnemies;
    game.playerLife = playerLife;
    game.SpeedEnemyBullets = 4.0f; //più è bassa, più è veloce
    player.vita = playerLife;
}


void resetGame(){
    
    for (auto it = nemici.begin(); it != nemici.end();) {
        it = nemici.erase(it);
    }
    for (auto it = proiettili.begin(); it != proiettili.end();) {
        it = proiettili.erase(it);
    }
    for (auto it = proiettiliNemici.begin(); it != proiettiliNemici.end();) {
        it = proiettiliNemici.erase(it);
    }
    
    difficoltaPartita(3, 3, 5); //torniamo in easy mode
    
    /* resetto caratteristiche player*/
    player.position.x = width / 2.0f;
    player.position.y = height / 20.0f;
    player.isAlive = true;
    player.vita = game.playerLife;

}
