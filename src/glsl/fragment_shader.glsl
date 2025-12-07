#version 330 core
in vec4 ourColor;
out vec4 FragColor;  
uniform vec2 resolution;
uniform float time;
vec3 col;
float smooth_;
const vec3 skyColor = vec3(0.0, 0.0, 0.0);


// Funzione hash semplice per generare un valore pseudo-casuale
float random(vec2 n) {
    return fract(sin(dot(n, vec2(12.9898, 78.233))) );
}


void main()
{
    // Coordinate normalizzate del frammento
    vec2 normalizedCoords = 2.0 * gl_FragCoord.xy / resolution.xy - 1.0;

    // colore iniziale delle stelle
    vec3 starColor = vec3(0.2, 0.0, 0.40);

    // definisco e inizializzo i parametri delle stelle
    int numberOfStars = 250;
    float starDensity = 0.4;
    float starScale = 0.005;


    for (int i = 0; i < numberOfStars; i++)
    {
        //scelta della direzione: direzioni casuali orizzontali
        //vec2 circleCenter = vec2(random(vec2(float(i), time * 0.0001)) * 2.0 - 1.0,
        //    random(vec2(time * 0.0001, float(i))) * 2.0 - 1.0);


        //direzioni completamente casuali
        vec2 circleCenter = vec2(random(vec2(float(i), time * 0.0003)) * 2.0 - 1.0,
                         random(vec2(time * 0.0003, float(i))) * 2.0 - 1.0);
        

        // Stesso verso
        //vec2 initialPosition = vec2(random(vec2(float(i), 0.0)) * 2.0 - 1.0,  // Posizione iniziale orizzontale casuale
        //                    random(vec2(0.0, float(i))) * 2.0 - 1.0); // Posizione iniziale verticale casuale
        // Spostamento orizzontale in funzione del tempo
        //float shiftX = mod(initialPosition.x + time * 0.1, 2.0) - 1.0; // "2.0" corrisponde all'intervallo [-1, 1] in coordinate normalizzate
        // Usa `shiftX` come coordinata x aggiornata per il movimento unidirezionale
        //vec2 circleCenter = vec2(shiftX, initialPosition.y);


        // Distanza del frammento dal centro del cerchio
        float distanceToCenter = length(normalizedCoords - circleCenter);

        // Densit  della stella in questo punto
        float starHeight = smoothstep(starScale, 0.0, distanceToCenter);

        // Aggiungi la densit  della stella al colore finale
        starColor += vec3(starDensity) * starHeight;
    }

    // Calcola il colore finale del frammento, interpolando tra cielo e stelle
    FragColor = vec4(mix(skyColor, starColor, starColor.b), 1.0);
}