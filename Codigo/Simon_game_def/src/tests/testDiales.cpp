//
//#include <iostream>
//#include <unistd.h>
//#include "simon_librerias/simon_dial.hpp"
//
//int main() {
//    BBB::SimonDial dialVelocidad(0, BBB::PWM::P9_22);
//    dialVelocidad.velocidadMaximaGrados = 40;
//    BBB::SimonDial dialTiempo(1, BBB::PWM::P9_42);
//    dialTiempo.velocidadMaximaGrados = 40;
//
//    std::cout << "Test del dial. Presiona CTRL+C para salir." << std::endl;
//
//    while (true) {
//    	// Dial Velocidad
//    	dialVelocidad.actualizar();
//        std::cout << "Ángulo: " << dialVelocidad.getAngulo() << "°, Nivel: " << dialVelocidad.getNivel() << "\r" << std::flush;
//
//        // Dial Tiempo
//        dialTiempo.actualizar();
//        std::cout << "Ángulo: " << dialTiempo.getAngulo() << "°, Nivel: " << dialTiempo.getNivel() << "\r" << std::flush;
//        usleep(25000); // 50 Hz
//    }
//
//    return 0;
//}
