#include "ImGui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imGui/imgui_impl_opengl3.h"
#include <math.h>
#include "ImGui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imGui/imgui_impl_opengl3.h"
#include <math.h>
#include <cstdio>  
#include "Strutture.h"
#include "Utilities.h"

extern int width, height;
extern Game game;
extern std::vector<Curva> nemici;
extern Curva player, nemico;

ImFont* largeTitleFont;
ImFont* mediumFont;

/* verifico la compatibilità della versione IMGUI, creo il
contesto, ottengo l'oggetto IO per configurare opzioni,
imposto il tema che preferisco per l'interfaccia,
infine inizializzo ImGui per GLFW e per OpenGL 3.3*/
void Initialize_IMGUI(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontDefault(); //font di default
    largeTitleFont = io.Fonts->AddFontFromFileTTF("playful-time-star-font/PlayfulTime-BLBB8.ttf", 128.0f);
    mediumFont = io.Fonts->AddFontFromFileTTF("Sour_Gummy/SourGummy-VariableFont_wdth,wght.ttf", 24.0f);
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

void generaGraficamenteNemici() {
    for (int k = 0; k < game.numberOfEnemies; k++) {
        Curva newEnemy = nemico;
        newEnemy.direzioneDestra = k % 2 == 0 ? true : false;
        newEnemy.position = randomPosition(width - 50, height);
        nemici.push_back(newEnemy);
    }
}

void my_interface() {
    ImGui_ImplGlfw_NewFrame();
    ImGui_ImplOpenGL3_NewFrame(); //inizio un nuovo frame per ImGui
    ImGui::NewFrame();


    /* SCHERMATA MENU */

    if (game.isGameStarting == false && game.isGameLosing == false && game.isGameWinning == false) {
        /* dimensioni e posizione della finestra */
        ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(width / 2 - 300, height / 2 - 200), ImGuiCond_Always);
        
        /* finestra del menu*/
        ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
        
        /* titolo del menu */
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.5f, 0.2f, 1.0f)); // Cambia colore del testo
        ImGui::PushFont(largeTitleFont); //per cambiare font (titolo)
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("SPACE GAME").x) / 2); // Centra il titolo
        ImGui::Text("SPACE GAME");
        ImGui::PopFont(); //per tornare al font standard
        ImGui::PopStyleColor();

        /* separatore e spazio */
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();

        /* pulsante start */
        ImGui::PushFont(mediumFont); //per cambiare font (bottoni)
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 300) / 2); // Centra il pulsante
        if (ImGui::Button("Play", ImVec2(300, 50))) {
            generaGraficamenteNemici();
            game.isGameStarting = true;
        }
        /*spazio tra i pulsanti */
        ImGui::Spacing();
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 300) / 2); // Centra il pulsante

        /*pulsanti dei livelli */
        if (ImGui::Button("Level easy", ImVec2(300, 30))) {
            difficoltaPartita(3, 3, 5); //vite dei nemici | numero nemici | vite player
            
        }
        ImGui::Spacing();
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 300) / 2);
        if (ImGui::Button("Level medium", ImVec2(300, 30))) {
            difficoltaPartita(5, 5, 4);
         
        }
        ImGui::Spacing();
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 300) / 2);
        if (ImGui::Button("Level hard", ImVec2(300, 30))) {
            difficoltaPartita(5, 7, 3);
            
        }
        ImGui::Spacing();
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 300) / 2);

        /* pulsante exit */
        if (ImGui::Button("Quit game", ImVec2(300, 50))) {
            game.isGameQuitting = true;
        }
        ImGui::PopFont();

        
        ImGui::End();
    }

    /* svolgimento del gioco */
    else if (game.isGameStarting == true) {
        ImVec2 size(100, 50);
        ImGui::SetNextWindowSize(size, ImGuiCond_Once);
        ImGui::SetNextWindowPos(ImVec2(10, 650), ImGuiCond_Once);
        ImGui::Begin("SCORE", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
        ImGui::Text("%d", game.score);
        ImGui::End();
        ImGui::SetNextWindowSize(size, ImGuiCond_Once);
        ImGui::SetNextWindowPos(ImVec2(10, 710), ImGuiCond_Once);
        ImGui::Begin("HIGH SCORE", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
        ImGui::Text("%d", game.highScore);
        ImGui::End();
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
        ImVec2 text_size = ImGui::CalcTextSize("Vite: 100");
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "LIFE: %d", player.vita);
        draw_list->AddText(ImVec2(width - text_size.x - 10, 10), IM_COL32(255, 255, 255, 255), buffer);
    }
    /* fine del gioco */
    else {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Colore di sfondo nero
        glClear(GL_COLOR_BUFFER_BIT);        // Cancella il buffer colore con il nero

        ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(width / 2 - 300, height / 2 - 200), ImGuiCond_Always);

        //ImGui::SetNextWindowSize(ImVec2(220, 135), ImGuiCond_Always);
        //ImGui::SetNextWindowPos(ImVec2(width / 2 - 100, height / 2 - 50), ImGuiCond_Always);
        ImGui::Begin("Game Over", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
        
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.5f, 0.2f, 1.0f)); // Cambia colore del testo
        ImGui::PushFont(largeTitleFont); //per cambiare font (titolo)

        if (game.isGameWinning == true) {
            ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("YOU WON").x) / 2); // Centra il titolo
            ImGui::Text("YOU WON");
        }
        else if (game.isGameLosing == true) {
            ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("GAME OVER").x) / 2); // Centra il titolo
            ImGui::Text("GAME OVER");
        }
        ImGui::PopFont(); //per tornare al font standard
        ImGui::PopStyleColor();
        

        /* gestione del punteggio */
        ImGui::PushFont(mediumFont); //per cambiare font (bottoni)
        ImGui::Spacing();
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 300) / 2);
        ImGui::Text("Points: %d", game.score);
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 300) / 2);
        ImGui::Text("High Score: %d", game.highScore);

        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 300) / 2);
        if (ImGui::Button("Quit game", ImVec2(300, 50))) {
            game.isGameWinning = false;
            game.isGameLosing = false;
            game.isGameQuitting = true;
        }
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 300) / 2);
        if (ImGui::Button("Back to game", ImVec2(300, 50))) {
            changePlayerColor(player, vec4(0.4f, 0.4f, 0.4f, 1.0f));
            game.isGameStarting = false;
            game.isGameWinning = false;
            game.isGameLosing = false;
            resetGame();
        }
        ImGui::PopFont();

        ImGui::End();
    }
    ImGui::Render();
}


/* chiudo l'integrazione con OpenGL, con GLFW e
distruggo il contesto ImGui*/
void close_GUI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}