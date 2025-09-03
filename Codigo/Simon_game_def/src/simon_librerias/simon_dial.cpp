
#include "simon_dial.hpp"
#include <cmath>
#include <unistd.h>

namespace BBB {

SimonDial::SimonDial(int adcChannel, PWM::pinPWM pwmPin)
    : adc(adcChannel), pwm(pwmPin)
{
    pwm.setPolarity(PWM::ACTIVE_HIGH);
    pwm.setPeriod(period);
    pwm.run();
}

SimonDial::~SimonDial() {
    pwm.stop();
}

unsigned int SimonDial::calcularDuty(int angle) {
    return sval_min + (angle * (sval_max - sval_min)) / 180;
}

int SimonDial::calcularNivel(int angle) {
    if (angle < 45) return 3;
    if (angle < 90) return 2;
    if (angle < 135) return 1;
    return 0;
}

void SimonDial::actualizar() {
    int raw = adc.readAnalog();
    int angulo_deseado = (raw * 180) / v_pot_max;
    int delta = angulo_deseado - angulo;

    if (std::abs(delta) > velocidadMaximaGrados) {
        angulo += (delta > 0 ? velocidadMaximaGrados : -velocidadMaximaGrados);
    } else {
        angulo = angulo_deseado;
    }

    nivel = calcularNivel(angulo);
    unsigned int duty = calcularDuty(angulo);
    pwm.setDutyCycle(duty);
}

void SimonDial::setRango(int in_min, int in_max) {
	sval_min = in_min;
	sval_max = in_max;
}

int SimonDial::getAngulo() const { return angulo; }
int SimonDial::getNivel() const { return nivel; }

} // namespace BBB
