#include "SimonLeds.h"
#include <unistd.h>

SimonLeds::SimonLeds() {
    leds[RED]     = new BBB::GPIOuniv(LED_ROJO);
    leds[GREEN]   = new BBB::GPIOuniv(LED_VERDE);
    leds[BLUE]    = new BBB::GPIOuniv(LED_AZUL);
    leds[YELLOW]  = new BBB::GPIOuniv(LED_AMARILLO);
    leds[PAUSE]   = new BBB::GPIOuniv(LED_PAUSA);
    leds[START]   = new BBB::GPIOuniv(LED_INICIO);

    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i]->setDirection(BBB::GPIOuniv::OUTPUT);  // Corregido: OUTPUT
    }
}

SimonLeds::~SimonLeds() {
    for (int i = 0; i < NUM_LEDS; i++) {
        delete leds[i];
    }
}

void SimonLeds::encenderLed(LedColor color) {
    leds[color]->setValue(BBB::GPIOuniv::HIGH);
}

void SimonLeds::apagarLed(LedColor color) {
    leds[color]->setValue(BBB::GPIOuniv::LOW);
}

void SimonLeds::parpadearLed(LedColor color, int tiempo_ms) {
    encenderLed(color);
    usleep(tiempo_ms * 1000);
    apagarLed(color);
}

void SimonLeds::apagarTodos() {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i]->setValue(BBB::GPIOuniv::LOW);
    }
}

void SimonLeds::encenderTodos() {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i]->setValue(BBB::GPIOuniv::HIGH);
    }
}

void SimonLeds::encenderLedsJuego() {
    for (int i = 0; i < NUM_LEDS; i++) {
        if (i != START && i != PAUSE) {
            leds[i]->setValue(BBB::GPIOuniv::HIGH);
        }
    }
}

void SimonLeds::parpadearRepetido(LedColor color, int repeticiones, int tiempo_ms) {
    for (int i = 0; i < repeticiones; i++) {
        encenderLed(color);
        usleep(tiempo_ms * 1000);
        apagarLed(color);
        usleep(tiempo_ms * 1000);
    }
}
