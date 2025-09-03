//#include "SimonBotones.hpp"
//#include <iostream>
//#include <thread>
//#include <atomic>
//#include <csignal>
//#include <unistd.h>
//
//// Importamos el namespace correcto para SimonBotones
//using namespace BBB;
//
//std::atomic<int> estado_actual(0);  // 0: espera inicio, 1: ejecutando, 2: pausado
//
//// Callback para INICIO
//int callback_inicio(int gpio)
//{
//    if (estado_actual == 0) {
//        std::cout << "[INTERRUPCIÓN] Botón INICIO pulsado (GPIO " << gpio << ") en estado 0\n";
//        estado_actual = 1;
//    } else {
//        std::cout << "[IGNORADO] INICIO pulsado en estado " << estado_actual << "\n";
//    }
//    return 0;
//}
//
//// Callback para PAUSA
//int callback_pausa(int gpio)
//{
//    if (estado_actual == 1) {
//        std::cout << "[INTERRUPCIÓN] Botón PAUSA pulsado (GPIO " << gpio << ") en estado 1\n";
//        estado_actual = 2;
//    } else if (estado_actual == 2) {
//        std::cout << "[INTERRUPCIÓN] Botón PAUSA pulsado (GPIO " << gpio << ") en estado 2\n";
//        estado_actual = 0;
//    } else {
//        std::cout << "[IGNORADO] PAUSA pulsado en estado " << estado_actual << "\n";
//    }
//    return 0;
//}
//
//int main()
//{
//    std::cout << "Iniciando test de interrupciones con botones INICIO y PAUSA...\n";
//
//    SimonBotones botones;
//
//    // Configurar las interrupciones
//    botones.configurar_interrupcion_inicio(callback_inicio);
//    botones.configurar_interrupcion_pausa(callback_pausa);
//
//    std::cout << "Estados posibles:\n";
//    std::cout << "  0: Esperando INICIO\n";
//    std::cout << "  1: Ejecutando (puede pausar)\n";
//    std::cout << "  2: Pausado (puede reanudar)\n";
//
//    // Bucle infinito para monitorear el estado
//    while (true) {
//        std::cout << "[ESTADO] Actual: " << estado_actual.load() << std::endl;
//        std::this_thread::sleep_for(std::chrono::seconds(2));
//    }
//
//    return 0;
//}
