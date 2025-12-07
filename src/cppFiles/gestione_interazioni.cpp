#pragma once
#include "lib.h"
#include "strutture.h"
#include "gestione_interazioni.h"

extern GLFWwindow* window;
 
extern bool isMouseDown;
extern float r, g, b;
extern double mousex,mousey;
extern int height, width;
extern Curva player;
extern Figura proiettile;
extern vector<Figura> proiettili;


/* modo di GLFW per gestire gli eventi del cursore del mouse, permettendo di
rilevare e di eseguire azioni specifiche di conseguenza. */

void cursor_position_callback(
    GLFWwindow* window, double xpos, double ypos) {

    /* salvo le coordinate in cui si trova il cursore del mouse */

    mousex = xpos;
    mousey = height-ypos;
    
}


/* modo di GLFW per gestire gli eventi del mouse, permettendo di rilevare
 e di eseguire azioni specifiche di conseguenza. */

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    double xpos, ypos;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){ //pulsante sinistro del mouse premuto
        /* glfwGetCursorPos è un'utilità fornita dalla libreria GLFW che permette di recuperare la
        posizione attuale del cursore del mouse nella finestra openGL*/
      
        glfwGetCursorPos(window, &xpos, &ypos);
        mousex = xpos;
        mousey = height-ypos;
    }
}


/* modo di GLFW per gestire gli eventi della tastiera, permettendo di rilevare
quando un tasto viene premuto, e di eseguire azioni specifiche. */

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    float leftLimit = player.scale.x * 2.0f;
    float rightLimit = float(width) - player.scale.x * 3.5f;
    float upperLimit = float(width) - player.scale.y * 5.0f;
    float underLimit = player.scale.y * 2.0f;
    
    /* gestisco gli eventi tramite un switch case. */
    switch (key) {

    case GLFW_KEY_D:
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            if (player.position.x < rightLimit) {
                player.position.x += 15.0f;
            }
        }
        break;

    case GLFW_KEY_A:
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            if (player.position.x > leftLimit) {
                player.position.x -= 15.0f;
            }
        }
        break;

    case GLFW_KEY_W:
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            if (player.position.y < upperLimit) {
                player.position.y += 15.0f;
            }
        }
        break;

    case GLFW_KEY_S:
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            if (player.position.y > underLimit) {
                player.position.y -= 15.0f;
            }
        }
        break;

    case GLFW_KEY_SPACE:
        if (action == GLFW_PRESS) {
            Figura newBullet = proiettile;
            newBullet.position = player.position;
            proiettili.push_back(newBullet);
        }
        break;

    case GLFW_KEY_ESCAPE:
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        break;



    default:
        break;
    }

}

/* modo di GLFW per gestire quando la finestra viene ridimensionata. */

void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
    width = w;
    height = h;
}

