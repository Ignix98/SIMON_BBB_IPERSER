//#include "simon_librerias/SimonLeds.h"
//#include "simon_librerias/SimonBotones.hpp"
//#include "simon_librerias/SimonSecuencia.hpp"
//#include "simon_librerias/SimonBuzzer.hpp"
//#include <iostream>
//#include <unistd.h>
//#include <chrono>
//
//using namespace BBB;
//
///* -------- Tablas de configuración (ver arriba) -------- */
//struct VelocidadCfg { const char* nombre; unsigned delay_ms, step_ms, minimo_ms; };
//struct TiempoCfg    { const char* nombre; unsigned btn_ms; };
//
//constexpr VelocidadCfg vel_niveles[4] = {
//    { "Lenta",   400,  40, 200 },
//    { "Media",   300,  30, 150 },
//    { "Rápida",  200,  10, 100 },
//    { "Turbo",   100,  10, 60 }
//};
//
//constexpr TiempoCfg time_niveles[4] = {
//    { "Holgado",  3000 },
//    { "Normal",   2500 },
//    { "Estricto", 1800 },
//    { "Relámpago",1200 }
//};
//
///* ------------------- Intro visual -------------------- */
//void intro(SimonLeds& leds, SimonBuzzer& buzzer)
//{
//    leds.apagarTodos(); usleep(200000);
//    for (int i = 0; i < 4; ++i) {
//        leds.encenderLed(static_cast<SimonLeds::LedColor>(i));
//        buzzer.reproducirColor(static_cast<SimonBuzzer::COLOR>(i));
//        leds.apagarLed(static_cast<SimonLeds::LedColor>(i));
//        usleep(100000);
//    }
//    leds.encenderTodos();
//    buzzer.reproducirInicio();
//    usleep(400000);
//    leds.apagarTodos();
//}
//
///* --------- Mostrar secuencia con velocidad actual ---- */
//void playSeq(const SimonSecuencia& seq,
//             SimonLeds& leds,
//             SimonBuzzer& buzzer,
//             unsigned int delay_ms)
//{
//    for (int i = 0; i < seq.getNivelActual(); ++i) {
//        int col = seq.getColor(i);
//        leds.encenderLed(static_cast<SimonLeds::LedColor>(col));
//        buzzer.reproducirColor(static_cast<SimonBuzzer::COLOR>(col), delay_ms);
//        usleep(delay_ms * 1000);
//        leds.apagarLed(static_cast<SimonLeds::LedColor>(col));
//        usleep((delay_ms/2) * 1000);
//    }
//}
//
///* ====================  MAIN  ========================= */
//int main()
//{
//    /*► Selección de niveles (0-3) ◄*/
//    int vel_sel  = 1;   // velocidad de la secuencia
//    int time_sel = 1;   // tiempo límite por botón
//
//    auto velCfg  = vel_niveles[vel_sel];
//    auto timeCfg = time_niveles[time_sel];
//
//    unsigned int delay_ms = velCfg.delay_ms;
//
//    SimonLeds      leds;
//    SimonBotones   botones;
//    SimonBuzzer    buzzer(PWM::P9_14);
//    SimonSecuencia seq;
//    seq.añadirColor();
//
//    intro(leds, buzzer);
//
//    std::cout << "=== SIMON  (Velocidad: "   << velCfg.nombre
//              << ", Tiempo botón: " << timeCfg.nombre << ") ===\n";
//
//    while (true) {
//        /* 1. Mostrar secuencia */
//        playSeq(seq, leds, buzzer, delay_ms);
//
//        /* 2. Jugador la repite */
//        for (int i = 0; i < seq.getNivelActual(); ++i) {
//            int pulsado = -1;
//            auto t0 = std::chrono::steady_clock::now();
//
//            while (pulsado == -1) {
//                for (int j = 0; j < 4; ++j) {
//                    if (botones.leer_boton(static_cast<SimonBotones::COLOR>(j))) {
//                        pulsado = j;
//                        leds.encenderLed(static_cast<SimonLeds::LedColor>(j));
//                        buzzer.reproducirColor(static_cast<SimonBuzzer::COLOR>(j), 150);
//                        leds.apagarLed(static_cast<SimonLeds::LedColor>(j));
//                        while (botones.leer_boton(static_cast<SimonBotones::COLOR>(j)))
//                            usleep(10000);
//                        break;
//                    }
//                }
//                /* ► Tiempo agotado */
//                auto t1 = std::chrono::steady_clock::now();
//                if (std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count() >= timeCfg.btn_ms) {
//                    buzzer.reproducirFallo();
//                    std::cout << "¡Tiempo agotado!\n"; return 0;
//                }
//                usleep(10000);
//            }
//            if (pulsado != seq.getColor(i)) {
//                buzzer.reproducirFallo();
//                std::cout << "¡Fallaste!\n"; return 0;
//            }
//        }
//
//        /* 3. Acierto → secuencia crece y se ajusta velocidad */
//        buzzer.reproducirAcierto();
//        if (delay_ms > velCfg.minimo_ms + velCfg.step_ms)
//            delay_ms -= velCfg.step_ms;
//        seq.añadirColor();
//        usleep(500000);
//    }
//}
