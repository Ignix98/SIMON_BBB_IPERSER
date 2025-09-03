
#ifndef SIMON_DIAL_HPP_
#define SIMON_DIAL_HPP_

#include "ADC.h"
#include "PWMuniv.h"
#include <iostream>

namespace BBB {

class SimonDial {
public:
    SimonDial(int adcChannel, PWM::pinPWM pwmPin);
    ~SimonDial();

    void actualizar();               // Lee ADC y mueve servo
    int getAngulo() const;           // 0–180
    int getNivel() const;            // 1–4
    int velocidadMaximaGrados = 30;  // Límite de cambio por actualización
    void setRango(int in_min, int in_max);

private:
    ADC adc;
    PWM pwm;
    int angulo = 0;
    int nivel = 1;

    // Parámetros SG90
    const int period = 20000000;         // 20 ms
    unsigned int sval_min = 550000;
    unsigned int sval_max = 2400000;
    const unsigned int v_pot_max = 4095;

    unsigned int calcularDuty(int angle);
    int calcularNivel(int angle);
};

} // namespace BBB

#endif
