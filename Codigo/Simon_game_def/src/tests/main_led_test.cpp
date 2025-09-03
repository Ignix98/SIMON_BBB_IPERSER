/*
#include <iostream>
#include <unistd.h>
#include "simon_librerias/SimonLeds.h"

int main() {
    std::cout << "==== TEST LEDS ====" << std::endl;

    SimonLeds leds;

    // 1. Encender LEDs uno a uno
    std::cout << "Encendiendo LEDs uno a uno..." << std::endl;
    for (int i = 0; i < SimonLeds::NUM_LEDS; i++) {
        leds.encenderLed(static_cast<SimonLeds::LedColor>(i));
        usleep(500000);  // 0.5 segundos
        leds.apagarLed(static_cast<SimonLeds::LedColor>(i));
        usleep(200000);
    }

    // 2. Encender todos los LEDs del juego (sin inicio ni pausa)
    std::cout << "Encendiendo LEDs del juego (rojo, verde, azul, amarillo)..." << std::endl;
    leds.encenderLedsJuego();
    usleep(1000000);
    leds.apagarTodos();

    // 3. Encender TODOS los LEDs (incluye inicio y pausa)
    std::cout << "Encendiendo TODOS los LEDs..." << std::endl;
    leds.encenderTodos();
    usleep(1000000);
    leds.apagarTodos();

    // 4. Parpadear repetidamente el LED rojo
    std::cout << "Parpadeando LED ROJO 3 veces..." << std::endl;
    leds.parpadearRepetido(SimonLeds::RED, 3, 300);

    std::cout << "Parpadeando LED INICIO 3 veces..." << std::endl;
    leds.parpadearRepetido(SimonLeds::START, 3, 300);



    // 5. Apagar todos por seguridad
    std::cout << "Apagando todos los LEDs..." << std::endl;
    leds.apagarTodos();

    std::cout << "==== TEST COMPLETADO ====" << std::endl;
    return 0;
}
*/

