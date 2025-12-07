#version 330 core

layout (location = 0) in vec3 aPos;   // la posizione del vertice ha pos 0
layout (location = 1) in vec4 aColor; // la variabile del colore ha pos 1
  
out vec4 ourColor; // colore da passare al fragment

uniform mat4 Projection;  //VARIABILE DI TIPO uniform
uniform mat4 Model;
void main()
{
//Trasformazione dei vertici dalle coordinate nel sistema di riferimento dell'oggetto (apos), al sistema di riferimento del mondo (premoltiplicazione 
// per Model) e successivamente proiettate nel cubo di centro l'origine e lato lungo 2, con x,y,z che variano tra -1 ed 1- (premoltiplicazione 
//per la matrice Projection)
    gl_Position = Projection*Model*vec4(aPos, 1.0);
    ourColor = aColor; // set ourColor to the input color we got from the vertex data
}  

