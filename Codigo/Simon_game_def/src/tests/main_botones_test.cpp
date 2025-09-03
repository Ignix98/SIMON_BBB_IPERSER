/*
#include <iostream>
#include <unistd.h>
#include "SimonBotones.hpp"

int main() {
    BBB::SimonBotones botones;

    std::cout << "=== TEST DE BOTONES ===" << std::endl;
    std::cout << "Pulsa un botón para ver su estado." << std::endl;
    std::cout << "El programa se cierra al pulsar el botón de PAUSA." << std::endl;

    while (true) {
        // Verificar cada botón individualmente
        if (botones.leer_boton(BBB::SimonBotones::ROJO)) {
            std::cout << "Botón ROJO pulsado" << std::endl;
        }
        if (botones.leer_boton(BBB::SimonBotones::VERDE)) {
            std::cout << "Botón VERDE pulsado" << std::endl;
        }
        if (botones.leer_boton(BBB::SimonBotones::AMARILLO)) {
            std::cout << "Botón AMARILLO pulsado" << std::endl;
        }
        if (botones.leer_boton(BBB::SimonBotones::AZUL)) {
            std::cout << "Botón AZUL pulsado" << std::endl;
        }
        if (botones.leer_boton(BBB::SimonBotones::INICIO)) {
            std::cout << "Botón INICIO pulsado" << std::endl;
        }
        if (botones.leer_boton(BBB::SimonBotones::PAUSA)) {
            std::cout << "Botón PAUSA pulsado. Cerrando programa..." << std::endl;
            break;
        }

        usleep(200000);  // 200 ms para evitar saturar la CPU
    }

    return 0;

}
*/
