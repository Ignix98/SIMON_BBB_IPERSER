#ifndef SIMON_BUZZER_HPP_
#define SIMON_BUZZER_HPP_

#include "PWMuniv.h"
#include "notas.h"
#include <vector>

namespace BBB {

struct Nota {
    double frecuencia;
    int duracion_ms;
};

class SimonBuzzer {
public:
    enum COLOR { ROJO = 0, VERDE = 1, AMARILLO = 2, AZUL = 3 };
    bool keepPlaying = false;

    SimonBuzzer(PWM::pinPWM pin);
    ~SimonBuzzer();



    void reproducirNota(const Nota& nota, float duty = 0.5);
    void reproducirMelodia(const std::vector<Nota>& melodia, float duty = 0.5);

    void reproducirInicio();
    void reproducirFallo();
    void reproducirAcierto();
    void reproducirSilencio();
    void reproducirVictoria();
    void reproducirColor(COLOR color, int duracion_ms = 200);
    void reproducirConfirmacion(int duracion_ms = 200);
    void reproducirMelodias(const std::vector<std::vector<Nota>>& melodias, float duty);
    void reproducirMelodiasIntro();
    void reproducirFaseExtra();

    // Melodias
    void reproducirZeldaLullaby();
    void reproducirZeldaMainTheme();


private:
    PWM pwm;
};

} // namespace BBB

#endif
