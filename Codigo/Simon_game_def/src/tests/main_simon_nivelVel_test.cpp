//
//#include "simon_librerias/SimonLeds.h"
//#include "simon_librerias/SimonBotones.hpp"
//#include "simon_librerias/SimonSecuencia.hpp"
//#include "simon_librerias/SimonBuzzer.hpp"
//#include <iostream>
//#include <unistd.h>
//
//using namespace BBB;
///* ----- Selecciona nivel aquí (0-Fácil … 3-Extremo) ----- */
//int  nivel_sel = 3;   // SOLO MODIFICA ESTA LÍNEA
//
///*---------------- CONFIGURACIÓN DE DIFICULTAD ----------------*/
//struct Dificultad {
//    const char* nombre;
//    unsigned int delay_ms;   // duración inicial (ms)
//    unsigned int paso_ms;    // cuánto se reduce por ronda
//    unsigned int minimo_ms;  // límite inferior
//};
//
//constexpr Dificultad niveles[4] = {
//    /* 0: Fácil   */ { "Fácil",    400,  40, 200 },
//    /* 1: Medio   */ { "Medio",    300,  30, 150 },
//    /* 2: Difícil */ { "Difícil",  200,  10, 100 },
//    /* 3: Extremo */ { "Extremo",  100,  10, 60 }
//};
//
///*----------- INTRO: LEDs uno a uno + todos encendidos --------*/
//void secuencia_inicio(SimonLeds& leds, SimonBuzzer& buzzer)
//{
//    leds.apagarTodos();
//    usleep(200000);
//
//    for (int i = 0; i < 4; ++i) {
//        leds.encenderLed(static_cast<SimonLeds::LedColor>(i));
//        buzzer.reproducirColor(static_cast<SimonBuzzer::COLOR>(i));
//        leds.apagarLed(static_cast<SimonLeds::LedColor>(i));
//        usleep(100000);
//    }
//
//    leds.encenderTodos();
//    buzzer.reproducirInicio();
//    usleep(400000);
//    leds.apagarTodos();
//}
//
///*----------- Reproduce la secuencia con el delay actual ------*/
//void reproducirSecuencia(const SimonSecuencia& seq,
//                         SimonLeds& leds,
//                         SimonBuzzer& buzzer,
//                         unsigned int delay_ms)
//{
//    for (int i = 0; i < seq.getNivelActual(); ++i) {
//        int col = seq.getColor(i);
//
//        leds.encenderLed(static_cast<SimonLeds::LedColor>(col));
//        buzzer.reproducirColor(static_cast<SimonBuzzer::COLOR>(col), delay_ms);
//
//        usleep(delay_ms * 1000);
//        leds.apagarLed(static_cast<SimonLeds::LedColor>(col));
//
//        usleep((delay_ms/2) * 1000);          // pausa entre colores
//    }
//}
//
//int main()
//{
//    auto cfg        = niveles[nivel_sel];
//
//    SimonLeds      leds;
//    SimonBotones   botones;
//    SimonBuzzer    buzzer(PWM::P9_14);
//    SimonSecuencia secuencia;
//
//    unsigned int delay_ms = cfg.delay_ms;
//
//    std::cout << "\n=== SIMON (" << cfg.nombre << ") ===\n";
//
//    /* Intro + primer color */
//    secuencia.añadirColor();
//    secuencia_inicio(leds, buzzer);
//
//    while (true) {
//        /* 1) Mostrar la secuencia */
//        reproducirSecuencia(secuencia, leds, buzzer, delay_ms);
//
//        /* 2) El jugador la repite */
//        for (int i = 0; i < secuencia.getNivelActual(); ++i) {
//            int pulsado = -1;
//
//            while (pulsado == -1) {
//                for (int j = 0; j < 4; ++j) {
//                    if (botones.leer_boton(static_cast<SimonBotones::COLOR>(j))) {
//                        pulsado = j;
//                        leds.encenderLed(static_cast<SimonLeds::LedColor>(j));
//                        buzzer.reproducirColor(static_cast<SimonBuzzer::COLOR>(j), 150);
//                        leds.apagarLed(static_cast<SimonLeds::LedColor>(j));
//
//                        while (botones.leer_boton(static_cast<SimonBotones::COLOR>(j)))
//                            usleep(10000);       // espera a que suelte
//                        break;
//                    }
//                }
//                usleep(10000);
//            }
//
//            if (pulsado != secuencia.getColor(i)) {
//                buzzer.reproducirFallo();
//                std::cout << "¡Fallaste! Puntuación: "
//                          << (secuencia.getNivelActual()-1) << '\n';
//                leds.encenderTodos();
//                usleep(800000);
//                leds.apagarTodos();
//                return 0;
//            }
//        }
//
//        /* 3) Acierto: acelerar un poco, añadir color */
//        buzzer.reproducirAcierto();
//        if (delay_ms > cfg.minimo_ms + cfg.paso_ms)
//            delay_ms -= cfg.paso_ms;
//
//        secuencia.añadirColor();
//        usleep(500000);
//    }
//}
