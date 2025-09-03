//
//#include <iostream>
//#include <unistd.h>
//#include "SimonBotones.hpp"
//#include "SimonBuzzer.hpp"
//
//using namespace BBB;
//
//int main() {
//    SimonBotones botones;
//    SimonBuzzer buzzer(PWM::P9_14);  // Ajusta el pin según tu conexión
//
//    std::cout << "=== TEST BOTONES CON SONIDO ===" << std::endl;
//    std::cout << "Pulsa un botón para escuchar su sonido. Pulsa PAUSA para salir." << std::endl;
//
//    while (true) {
//        if (botones.leer_boton(SimonBotones::ROJO)) {
//            std::cout << "ROJO" << std::endl;
//            buzzer.reproducirColor(SimonBuzzer::ROJO);
//        }
//        if (botones.leer_boton(SimonBotones::VERDE)) {
//            std::cout << "VERDE" << std::endl;
//            buzzer.reproducirColor(SimonBuzzer::VERDE);
//        }
//        if (botones.leer_boton(SimonBotones::AMARILLO)) {
//            std::cout << "AMARILLO" << std::endl;
//            buzzer.reproducirColor(SimonBuzzer::AMARILLO);
//        }
//        if (botones.leer_boton(SimonBotones::AZUL)) {
//            std::cout << "AZUL" << std::endl;
//            buzzer.reproducirColor(SimonBuzzer::AZUL);
//        }
//        if (botones.leer_boton(SimonBotones::INICIO)) {
//            std::cout << "INICIO" << std::endl;
//            buzzer.reproducirConfirmacion();
//        }
//        if (botones.leer_boton(SimonBotones::PAUSA)) {
//            std::cout << "PAUSA (saliendo...)" << std::endl;
//            buzzer.reproducirConfirmacion();
//            break;
//        }
//
//        usleep(200000);  // 200 ms
//    }
//
//    return 0;
//}

