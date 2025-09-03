#include "SimonBotones.hpp"
#include <iostream>
#include <unistd.h>

namespace BBB {

SimonBotones::SimonBotones() {
    inButton[0] = new GPIOuniv((GPIOuniv::pinGPIO)BOTON_ROJO);
    inButton[1] = new GPIOuniv((GPIOuniv::pinGPIO)BOTON_VERDE);
    inButton[2] = new GPIOuniv((GPIOuniv::pinGPIO)BOTON_AMARILLO);
    inButton[3] = new GPIOuniv((GPIOuniv::pinGPIO)BOTON_AZUL);
    inButton[4] = new GPIOuniv((GPIOuniv::pinGPIO)BOTON_INICIO);
    inButton[5] = new GPIOuniv((GPIOuniv::pinGPIO)BOTON_PAUSA);

    for (int i = 0; i < 6; ++i) {
        inButton[i]->setDirection(GPIOuniv::INPUT);
    }
}

void SimonBotones::configurar_interrupcion_inicio(CallbackType funcion) {
    inButton[INICIO]->setActiveHigh();
    inButton[INICIO]->setEdgeType(GPIOuniv::RISING);
    inButton[INICIO]->setDebounceTime(10);
    inButton[INICIO]->waitForEdge(funcion);
}

void SimonBotones::configurar_interrupcion_pausa(CallbackType funcion) {
    inButton[PAUSA]->setActiveHigh();
    inButton[PAUSA]->setEdgeType(GPIOuniv::RISING);
    inButton[PAUSA]->setDebounceTime(10);
    inButton[PAUSA]->waitForEdge(funcion);
}

bool SimonBotones::leer_boton(COLOR color) {
    return inButton[color]->getValue() == GPIOuniv::HIGH;
}

std::vector<bool> SimonBotones::leer_estado(bool mostrar) {
    std::vector<bool> estado(4, false);
    for (int i = 0; i < 4; ++i) {
        estado[i] = leer_boton((COLOR)i);
    }

    if (mostrar) {
        std::cout << "Estado botones: (";
        for (int i = 0; i < 4; ++i)
            std::cout << estado[i] << (i < 3 ? ", " : "");
        std::cout << ")" << std::endl;
    }

    return estado;
}

bool SimonBotones::obtener_y_reiniciar_bandera(bool& bandera) {
    bool valor = bandera;
    if (bandera) bandera = false;
    return valor;
}

}
