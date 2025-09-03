#ifndef SIMONLEDS_H_
#define SIMONLEDS_H_

#include "../GPIOuniv/GPIOuniv.h"

class SimonLeds {
public:
    enum LedColor { RED, GREEN, YELLOW, BLUE, START, PAUSE, NUM_LEDS };

    SimonLeds();
    ~SimonLeds();

    void encenderLed(LedColor color);
    void apagarLed(LedColor color);
    void parpadearLed(LedColor color, int tiempo_ms = 300);
    void apagarTodos();
    void encenderTodos();
    void encenderLedsJuego();
    void parpadearRepetido(LedColor color, int repeticiones, int tiempo_ms);

private:
    static constexpr BBB::GPIOuniv::pinGPIO LED_ROJO     = BBB::GPIOuniv::P8_8;
    static constexpr BBB::GPIOuniv::pinGPIO LED_VERDE    = BBB::GPIOuniv::P8_10;
    static constexpr BBB::GPIOuniv::pinGPIO LED_AMARILLO = BBB::GPIOuniv::P8_12;
    static constexpr BBB::GPIOuniv::pinGPIO LED_AZUL     = BBB::GPIOuniv::P8_16;
    static constexpr BBB::GPIOuniv::pinGPIO LED_PAUSA    = BBB::GPIOuniv::P8_14;
    static constexpr BBB::GPIOuniv::pinGPIO LED_INICIO   = BBB::GPIOuniv::P8_18;

    BBB::GPIOuniv* leds[NUM_LEDS];
};

#endif /* SIMONLEDS_H_ */

