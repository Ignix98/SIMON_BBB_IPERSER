#ifndef SIMON_BOTONES_HPP_
#define SIMON_BOTONES_HPP_

#include "GPIOuniv.h"
#include <vector>

namespace BBB {

class SimonBotones {
public:
    enum COLOR { ROJO = 0, VERDE = 1, AMARILLO = 2, AZUL = 3, INICIO = 4, PAUSA = 5 };

private:
    enum BUTTON_GPIO {
        BOTON_ROJO     = GPIOuniv::P8_7,
        BOTON_VERDE    = GPIOuniv::P8_9,
        BOTON_AMARILLO = GPIOuniv::P8_11,
        BOTON_AZUL     = GPIOuniv::P8_15,
        BOTON_INICIO   = GPIOuniv::P8_17,
        BOTON_PAUSA    = GPIOuniv::P8_26
    };

    GPIOuniv* inButton[6];

public:
    SimonBotones();
    bool leer_boton(COLOR color);
    std::vector<bool> leer_estado(bool mostrar = false);
    void configurar_interrupcion_inicio(CallbackType funcion);
    void configurar_interrupcion_pausa(CallbackType funcion);
    bool obtener_y_reiniciar_bandera(bool& bandera);
};

} // namespace BBB

#endif
