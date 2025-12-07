#pragma once
#include <glad/glad.h>
#include "ShaderMaker.h"
#include "strutture.h"

extern unsigned int programId, programIdS, programIdBul, programIdNemici;

/* configuro gli shader perché il mio progetto possa avere animazioni
di sfondo e movimento degli oggetti. */

void INIT_SHADER(void){
    GLenum ErrorCheckValue = glGetError();

    /* init shader per gli oggetti principali */

    char* vertexShader = (char*)"vertex_shader.glsl";
    char* fragmentShader = (char*)"fragmentShaderN.glsl";

    /* funzione che crea un program shader completo a partire dallo shader per la gestione dei vertici
    e quello per la gestione dei pixel. ProgramId sarà un numero che verrà associato ad un oggetto per poi renderizzarlo*/
    
    programId = ShaderMaker::createProgram(vertexShader, fragmentShader);

    /* init shader per i nemici */

    fragmentShader = (char*)"fragmentShaderNemici.glsl";

    programIdNemici = ShaderMaker::createProgram(vertexShader, fragmentShader);
    
    /* init shader per la renderizzazione dei proiettili */

    fragmentShader = (char*)"fragmentShaderBul.glsl";

    programIdBul = ShaderMaker::createProgram(vertexShader, fragmentShader);

    /* init shader per la renderizzazione del background */

    fragmentShader = (char*)"fragment_shader.glsl";     

    programIdS = ShaderMaker::createProgram(vertexShader, fragmentShader); //questo identificativo è per lo sfondo
   

}

/* alla funzione passo in input un puntatore all'indirizzo ricevuto, in modo
dat poter modificare direttamente i campi della struttura originale */

void INIT_VAO(Figura* fig)
{
    /* genero un nuovo VAO e ottengo un identificatore univoco */
    glGenVertexArrays(1, &fig->VAO);

    glBindVertexArray(fig->VAO); //ora lego il VAO appena creato al contesto di rendering corrente

    /* creo il vbo dei VERTICI e lo rendo attivo */

    glGenBuffers(1, &fig->VBO_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_vertices);

    /* riempio il VBO dei vertici (carico i dati). argomenti:
    1. il tipo di buffer a cui la funzione fa riferimento. GL_ARRAY_BUFFER è tipico dei vertici
    2. quantità di memoria da allocare per il buffer (numero di vertici * la dimensione di cascun vertice)
    3. è il puntatore ai dati dei vertici. Copio i dati di fig->vertices nel buffer di OpenGL
    4. con lo static diciamo che i dati non cambieranno frequentemente */
    glBufferData(GL_ARRAY_BUFFER, fig->vertices.size() * sizeof(vec3), fig->vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0); //abilito l'attributo

    /* creo il vbo dei COLORI e lo rendo attivo */

    glGenBuffers(1, &fig->VBO_colors);
    glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_colors);

    /* riempio il VBO dei colori. Stesso discorso di quando ho
    riempito il VBO dei vertici */
    glBufferData(GL_ARRAY_BUFFER, fig->colors.size() * sizeof(vec4), fig->colors.data(), GL_STATIC_DRAW);
    
    //Adesso carico il VBO dei coloris nel layer 2
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1); //abilito l'attributo

}

/* prima era statico, ora è dinamico */

void INIT_VAO_DYNAMIC(Figura* fig)
{
    /* esattamente come nel caso precedente */

    glGenVertexArrays(1, &fig->VAO);
    glBindVertexArray(fig->VAO);

    glGenBuffers(1, &fig->VBO_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_vertices);

    /* ora è dinamico, non statico */
    glBufferData(GL_ARRAY_BUFFER, fig->vertices.size() * sizeof(vec3), fig->vertices.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0); //abilito l'attributo


    glGenBuffers(1, &fig->VBO_colors);
    glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_colors);

    glBufferData(GL_ARRAY_BUFFER, fig->colors.size() * sizeof(vec4), fig->colors.data(), GL_DYNAMIC_DRAW);

    //Adesso carico il VBO dei coloris nel layer 2
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

}

void INIT_VAO_DYNAMIC_NULL(Figura* fig)
{

    glGenVertexArrays(1, &fig->VAO);
    glBindVertexArray(fig->VAO);

    glGenBuffers(1, &fig->VBO_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_vertices);

    /* la funzione cambia qui */
    glBufferData(GL_ARRAY_BUFFER, fig->nTriangles * sizeof(vec3), NULL, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    /*creo, attivo, riempio VBO COLORI */

    glGenBuffers(1, &fig->VBO_colors);
    glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_colors);
    glBufferData(GL_ARRAY_BUFFER, fig->colors.size() * sizeof(vec4), NULL, GL_DYNAMIC_DRAW);

    //Adesso carico il VBO dei coloris nel layer 2
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

}

void UPDATE_VAO_DYNAMIC(Figura* fig)
{


    glBindVertexArray(fig->VAO);
    //Rendo attivo il VBO della geometria dei vertices
    glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_vertices);


    //glBufferSubData copia le posizioni dei vertici aggiornate da fig->vertices nell'oggetto vertex buffer object. 
    // Questo consente aggiornamenti efficienti senza dover ricreare l'intero VBO ogni volta che i dati cambiano.

    glBufferSubData(GL_ARRAY_BUFFER, 0, fig->vertices.size() * sizeof(vec3), fig->vertices.data());

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);



    glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_colors);
    glBufferSubData(GL_ARRAY_BUFFER, 0, fig->colors.size() * sizeof(vec4), fig->colors.data());

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

}


/* per le curve di Hermite, funzione statica*/

void INIT_VAO_Curva(Curva* fig)
{
    glGenVertexArrays(1, &fig->VAO);
    glBindVertexArray(fig->VAO);

    glGenBuffers(1, &fig->VBO_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_vertices);
    glBufferData(GL_ARRAY_BUFFER, fig->nv * sizeof(vec3), fig->vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &fig->VBO_colors);
    glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_colors);
    glBufferData(GL_ARRAY_BUFFER, fig->nv * sizeof(vec4), fig->colors.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &fig->VBO_CP);
    glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_CP);
    glBufferData(GL_ARRAY_BUFFER, fig->ncp * sizeof(vec3), fig->CP.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &fig->VBO_CP_colors);
    glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_CP_colors);
    glBufferData(GL_ARRAY_BUFFER, fig->ncp * sizeof(vec4), fig->colCP.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(3);

    glGenBuffers(1, &fig->VBO_TG);
    glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_TG);
    glBufferData(GL_ARRAY_BUFFER, fig->ntg * sizeof(vec3), fig->tg.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(4);

    glGenBuffers(1, &fig->VBO_TG_colors);
    glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_TG_colors);
    glBufferData(GL_ARRAY_BUFFER, fig->ntg * sizeof(vec4), fig->col_tg.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(5);
}

/* inizializzazione dei buffer e dei VAO necessari per la curva di Hermite.
Vertici, punti di controllo e vettori tangenti. Uso:
-vec3 per quanto riguarda le posizioni
-vec4 per quanto riguarda i colori (RGBA)
Scelgo GL_DYNAMIC_DRAW perché i dati cambieranno spesso. */

void INIT_VAO_DYNAMIC_Curva(Curva* fig){

    /* genero e collego un VAO, per gestire lo stato dei buffer */
    glGenVertexArrays(1, &fig->VAO);
    glBindVertexArray(fig->VAO);

    /* creo un vbo (VERTICI), lo attivo, gli lascio lo spazio allocato per nv vertici */
    glGenBuffers(1, &fig->VBO_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_vertices);
    glBufferData(GL_ARRAY_BUFFER, fig->nv * sizeof(vec3), NULL, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0); //assegno i dati all'attributo 0 per i vertici

    /* creo un VBO per i colori associati ai vertici (vec4 = RGBA) */
    glGenBuffers(1, &fig->VBO_colors);
    glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_colors);
    glBufferData(GL_ARRAY_BUFFER, fig->nv * sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
    
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1); //assegno i dati all'attributo 1 per i colori


    /* creo un VBO per i punti di controllo (inizio e fine della curva) */
    glGenBuffers(1, &fig->VBO_CP);
    glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_CP);
    glBufferData(GL_ARRAY_BUFFER, fig->ncp * sizeof(vec3), NULL, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(2);

    /* creo un VBO per i colori dei punti di controllo */
    glGenBuffers(1, &fig->VBO_CP_colors);
    glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_CP_colors);
    glBufferData(GL_ARRAY_BUFFER, fig->ncp * sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
    
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(3);

    
    /* creo un VBO per i vettori tangenti */
    glGenBuffers(1, &fig->VBO_TG);
    glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_TG);
    glBufferData(GL_ARRAY_BUFFER, fig->ntg * sizeof(vec3), NULL, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(4);

    /* creo un VBO per i colori dei vettori tangenti*/
    glGenBuffers(1, &fig->VBO_TG_colors);
    glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_TG_colors);
    glBufferData(GL_ARRAY_BUFFER, fig->ntg * sizeof(vec4), NULL, GL_DYNAMIC_DRAW);
    
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(5);


}

/* aggiorno i dati dei buffer della curva */

void UPDATE_VAO_Curva(Curva* fig){

    glBindVertexArray(fig->VAO);
    
    /* aggiorno i vertici della curva */
    glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_vertices);
    glBufferSubData(GL_ARRAY_BUFFER, 0, fig->vertices.size() * sizeof(vec3), fig->vertices.data());

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    /* aggiorno i colori dei vertici */
    glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_colors);
    glBufferSubData(GL_ARRAY_BUFFER, 0, fig->colors.size() * sizeof(vec4), fig->colors.data());
    
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);


    /* aggiorno i punti di controllo*/
    glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_CP);
    glBufferSubData(GL_ARRAY_BUFFER, 0, fig->CP.size() * sizeof(vec3), fig->CP.data());

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(2);

    /* aggiorno i colori dei punti di controllo */
    glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_CP_colors);
    glBufferSubData(GL_ARRAY_BUFFER, 0, fig->colCP.size() * sizeof(vec4), fig->colCP.data());
    
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(3);


    /* aggiorno i vettori tangenti */
    glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_TG);
    glBufferSubData(GL_ARRAY_BUFFER, 0, fig->tg.size() * sizeof(vec3), fig->tg.data());

    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(4);

    /* aggirono i colori dei vettori tangenti*/
    glBindBuffer(GL_ARRAY_BUFFER, fig->VBO_TG_colors);
    glBufferSubData(GL_ARRAY_BUFFER, 0, fig->col_tg.size() * sizeof(vec4), fig->col_tg.data());
    
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(5);

    //nota: teoricamente posso ogliere tutti i glVertex e glEnable perché
    //questa funzione aggiorna solo i dati, non la struttura
}