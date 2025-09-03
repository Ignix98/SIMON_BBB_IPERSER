
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <chrono>
#include "state_monitor/stateMonitor.hpp"   // Gestor de estados
#include "state_monitor/threadConf.hpp"   // Configuración de hilos por estado
#include "simon_librerias/SimonBotones.hpp"  // Lectura de botones
#include "simon_librerias/SimonLeds.h"      // Control de LEDs
#include "simon_librerias/SimonBuzzer.hpp"  // Generación de sonidos
#include "simon_librerias/SimonSecuencia.hpp" // Secuencia de colores del juego
#include "simon_librerias/simon_dial.hpp"   // Lectura de diales (potenciómetros)

using namespace BBB;

/* ────── Variables Globales ────── */
StateMonitor stateMgr;            // Control de estados del juego (0 a 5)
SimonBotones botones;             // Interfaz de lectura de botones
SimonLeds    leds;                // Interfaz de control de LEDs
SimonBuzzer  buzzer(PWM::P9_14);  // Buzzer conectado a PWM P9_14
SimonSecuencia secuencia;         // Guarda y gestiona la secuencia de colores

// Diales para ajustar velocidad y tiempo de espera
SimonDial dialVelocidad(3, PWM::P8_19);
SimonDial dialTiempo  (0, PWM::P9_22);

// Niveles ajustables por los diales
int nivelVelocidad = 3, nivelTiempo = 3;
int parpadeoLow = 300000, parpadeoHigh = 300000;  // Duraciones de parpadeo LED

// Retardos configurados según nivel
constexpr unsigned velDelay[4] = {300, 250, 150, 80};  // Delay de sonido/LED
constexpr unsigned tmoBtn [4] = {5000, 4000, 3000, 2000}; // Timeout de pulsación

int SEQ_MAX = secuencia.getNivelMax();/* tu valor máximo de niveles */;

/* ═════ ESTADO 0 — Esperar INICIO ═════ */
void* hiloEstado0(void* p)
{
    auto* cfg = static_cast<threadConf*>(p);
    while (true) {
        // Espera hasta que el estado cambie a 0
        if (stateMgr.waitState(cfg) != 0) continue;

        // Lanzar hilo que reproduce melodía de espera
        pthread_t tMelodia;
        pthread_create(&tMelodia, nullptr, [](void*)->void* {
            while (stateMgr.getState() == 0) {
                buzzer.reproducirMelodiasIntro();
            }
            return nullptr;
        }, nullptr);

        // Parpadeo del LED START hasta que se pulse INICIO
        while (stateMgr.getState() == 0) {
            leds.encenderLed(SimonLeds::START); usleep(parpadeoHigh);
            leds.apagarLed (SimonLeds::START); usleep(parpadeoLow);
        }
        pthread_join(tMelodia, nullptr);  // Espera a que termine la melodía
        leds.apagarLed(SimonLeds::START);
    }
    return nullptr;
}

/* ─ Diales de ajuste (solo en estados 0 y 5) ─ */
void* hiloDial1(void* p)
{
    while (true) {
        int estado = stateMgr.getState();
        if (estado == 0 || estado == 5) {
            dialTiempo.actualizar();          // Leer valor del dial

            usleep(25000);                   // Frecuencia 50 Hz
        } else {
            usleep(1000000);                 // Ahorro CPU en otros estados
        }
    }
    return nullptr;
}

void* hiloDial2(void* p)
{
    while (true) {
        int estado = stateMgr.getState();
        if (estado == 0 || estado == 5) {
            dialVelocidad.actualizar();      // Igual que dial de tiempo

            usleep(25000);
        } else {
            usleep(1000000);
        }
    }
    return nullptr;
}

/* ═════ ESTADO 1 — Secuencia de inicio ═════ */
void* hiloEstado1(void* p)
{
    auto* cfg = static_cast<threadConf*>(p);
    while (true) {
        if (stateMgr.waitState(cfg) != 1) continue;

        // Encender todos los LEDs y reproducir colores uno a uno
        leds.encenderLedsJuego(); usleep(350000);
        leds.apagarTodos();  usleep(250000);
        for (int c = 0; c < 4; ++c) {
            leds.encenderLed(static_cast<SimonLeds::LedColor>(c));
            buzzer.reproducirColor(static_cast<SimonBuzzer::COLOR>(c));
            usleep(15000);
            leds.apagarLed(static_cast<SimonLeds::LedColor>(c));
            usleep(13000);
        }
        // Feedback final antes de empezar el juego
        leds.encenderTodos();  buzzer.reproducirInicio(); usleep(300000);
        leds.apagarTodos();

        stateMgr.changeState(2);  // Pasar a mostrar secuencia real
    }
    return nullptr;
}

/* ═════ ESTADO 2 — Mostrar secuencia al jugador ═════ */
void* hiloEstado2(void* p)
{
    auto* cfg = static_cast<threadConf*>(p);
    while (true) {
        if (stateMgr.waitState(cfg) != 2) continue;
        nivelVelocidad = dialVelocidad.getNivel();
        //cout << nivelVelocidad << endl;
        // Recorre la secuencia guardada y la muestra con LED + sonido
        for (int i = 0; i < secuencia.getNivelActual(); ++i) {
            int col = secuencia.getColor(i);
            leds.encenderLed(static_cast<SimonLeds::LedColor>(col));
            buzzer.reproducirColor(
                static_cast<SimonBuzzer::COLOR>(col), velDelay[nivelVelocidad]
            );
            usleep(velDelay[nivelVelocidad] * 1000);
            leds.apagarLed(static_cast<SimonLeds::LedColor>(col));
            usleep(velDelay[nivelVelocidad] * 500);
        }
        stateMgr.changeState(3);  // Cambia al modo lectura de jugador
    }
    return nullptr;
}

/* ═════ ESTADO 3 — Leer pulsaciones del jugador (con fases y victoria) ═════ */
void* hiloEstado3(void* p)
{
    auto* cfg = static_cast<threadConf*>(p);
    while (true){
        if (stateMgr.waitState(cfg) != 3) continue;

        nivelTiempo = dialTiempo.getNivel();
        //cout << nivelTiempo << endl;
        bool fallo = false;
        // Leer cada color de la secuencia
        for (int i = 0; i < secuencia.getNivelActual() && !fallo; ++i) {
            auto start = std::chrono::steady_clock::now();
            bool leido = false;
            int esp = secuencia.getColor(i);

            while (!leido) {
                // Escanea los 4 botones
                for (int c = 0; c < 4; ++c) {
                    if (botones.leer_boton(static_cast<SimonBotones::COLOR>(c))) {
                        buzzer.reproducirColor(
                            static_cast<SimonBuzzer::COLOR>(c), 200
                        );
                        leido = true;
                        if (c != esp) fallo = true;
                        // Espera a soltar
                        while (botones.leer_boton(
                            static_cast<SimonBotones::COLOR>(c)
                        )) usleep(10000);
                        usleep(100000);
                        break;
                    }
                }
                // Comprueba timeout
                if (std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::steady_clock::now() - start
                    ).count() > tmoBtn[nivelTiempo]) {
                    fallo = true;
                    break;
                }
                usleep(10000);
            }
        }

        if (fallo) {
            // ERROR: reiniciar juego
            buzzer.reproducirFallo();
            secuencia.reiniciar();
            stateMgr.changeState(0);
        }
        else {
            // ACIERTO: añadir siguiente color
            buzzer.reproducirAcierto();
            secuencia.añadirColor();


            int nivel = secuencia.getNivelActual();
            //cout << nivel << endl;

            if (nivel % 5 == 0 && nivel < SEQ_MAX) {
                // Cada 5 niveles, fase superada
            	//cout << "fase superada" << endl;
                stateMgr.changeState(6);
            }
            else if (nivel >= SEQ_MAX) {
                // Juego completado: victoria
                stateMgr.changeState(7);
            }
            else {
                // Pausa breve normal
                stateMgr.changeState(4);
            }
        }
    }
    return nullptr;
}


/* ═════ ESTADO 4 — Feedback pausa breve ═════ */
void* hiloEstado4(void* p)
{
    auto* cfg = static_cast<threadConf*>(p);
    constexpr int TIMEOUT_MS = 1500; // 2 segundos para volver a mostrar

    while (true) {
        if (stateMgr.waitState(cfg) != 4) continue;

        auto inicio = std::chrono::steady_clock::now();
        while (stateMgr.getState() == 4) {
            leds.encenderLed(SimonLeds::PAUSE); usleep(parpadeoHigh);
            leds.apagarLed (SimonLeds::PAUSE); usleep(parpadeoLow);
            // Timeout: regresa al estado 2
            if (std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::steady_clock::now() - inicio
                ).count() > TIMEOUT_MS) {
                stateMgr.changeState(2);
                break;
            }
        }
    }
    return nullptr;
}

/* ═════ ESTADO 5 — Pausa manual ═════ */
void* hiloEstado5(void* p)
{
    auto* cfg = static_cast<threadConf*>(p);
    while (true) {
        if (stateMgr.waitState(cfg) != 5) continue;

        leds.encenderLedsJuego(); // Mantiene los LEDs de juego encendidos
        // Parpadeo LED PAUSE hasta reanudar
        while (stateMgr.getState() == 5) {
            leds.encenderLed(SimonLeds::PAUSE); usleep(parpadeoHigh);
            leds.apagarLed (SimonLeds::PAUSE); usleep(parpadeoLow);

        }
        // Al salir, se apagarán LEDs en el callback de pausa
    }
    return nullptr;
}

/* ═════ ESTADO 6 — Fase superada ═════ */
void* hiloEstado6(void* p) {
    auto* cfg = static_cast<threadConf*>(p);
    while (true) {
        if (stateMgr.waitState(cfg) != 6) continue;

        buzzer.reproducirFaseExtra();   // método con el sonidito
        for(int i = 0; i < 3; i++ ){
			leds.encenderTodos(); usleep(150000);
			leds.apagarTodos(); usleep(150000);
		}

        stateMgr.changeState(4);        // volvemos a mostrar secuencia
    }
    return nullptr;
}

/* ═════ ESTADO 7 — Victoria ═════ */
void* hiloEstado7(void* p) {
    auto* cfg = static_cast<threadConf*>(p);
    while (true) {
        if (stateMgr.waitState(cfg) != 7) continue;
        //cout << "fd" << endl;
        buzzer.reproducirVictoria();    // melodía de victoria

        for(int i = 0; i < 4; i++ ){
        	leds.encenderTodos(); usleep(300000);
        	leds.apagarTodos(); usleep(300000);
        }

        secuencia.reiniciar();          // preparamos nuevo juego
        stateMgr.changeState(0);        // volvemos al inicio
    }
    return nullptr;
}

/* ── Callbacks de interrupciones por hardware ── */
int callback_inicio(int /*gpio*/)
{
    // Ignora ruido inicial
    static int edgeCount = 0;
    if (edgeCount < 2) { ++edgeCount; return 0; }

    // Verifica que el botón sigue presionado
    if (!botones.leer_boton(SimonBotones::INICIO)) return 0;
    usleep(10000);

    if (stateMgr.getState() == 0) {
    	buzzer.reproducirConfirmacion();
    	usleep(100000);
        // Arranca nueva partida: reinicia secuencia y comienza demo
        secuencia.reiniciar();
        secuencia.añadirColor();
        buzzer.reproducirSilencio();
        buzzer.keepPlaying = false;
        stateMgr.changeState(1);
    }
    return 0;
}

int callback_pausa(int /*gpio*/)
{
    static int edgeCount = 0;
    if (edgeCount < 2) { ++edgeCount; return 0; }
    if (!botones.leer_boton(SimonBotones::PAUSA)) return 0;
    usleep(10000);

    int s = stateMgr.getState();
    if      (s == 4) { buzzer.reproducirConfirmacion();
    stateMgr.changeState(5); }
    else if (s == 5) {
    	buzzer.reproducirConfirmacion();
        leds.apagarTodos();
        //leds.apagarLed(SimonLeds::PAUSE);
        usleep(100000);
        stateMgr.changeState(2);
    }
    return 0;
}

/* ═════ MAIN ═════ */
int main()
{
    // Configuración de sensibilidad de diales
    dialVelocidad.velocidadMaximaGrados = 40;
    dialTiempo.velocidadMaximaGrados   = 40;

    // Crear hilos para cada estado y diales
    pthread_t th0,th1,th2,th3,th4,th5,tDial1,tDial2, th6, th7;
    pthread_attr_t attr; pthread_attr_init(&attr);

    threadConf cfg0,cfg1,cfg2,cfg3,cfg4,cfg5,cfgDial1,cfgDial2, cfg6, cfg7;
    cfg0.addState(0); cfg1.addState(1); cfg2.addState(2);
    cfg3.addState(3); cfg4.addState(4); cfg5.addState(5);
    cfg6.addState(6); cfg7.addState(7);

    pthread_create(&tDial1, nullptr, hiloDial1,  &cfgDial1);
    pthread_create(&tDial2, nullptr, hiloDial2,  &cfgDial2);
    pthread_create(&th0,    &attr,   hiloEstado0, &cfg0);
    pthread_create(&th1,    &attr,   hiloEstado1, &cfg1);
    pthread_create(&th2,    &attr,   hiloEstado2, &cfg2);
    pthread_create(&th3,    &attr,   hiloEstado3, &cfg3);
    pthread_create(&th4,    &attr,   hiloEstado4, &cfg4);
    pthread_create(&th5,    &attr,   hiloEstado5, &cfg5);
    pthread_create(&th6,    &attr,   hiloEstado6, &cfg6);
    pthread_create(&th7,    &attr,   hiloEstado7, &cfg7);

    stateMgr.changeState(0);                     // Inicia en estado de espera

    // Limpia rebotes pendientes
    botones.leer_boton(SimonBotones::INICIO);
    botones.leer_boton(SimonBotones::PAUSA);

    // Asocia callbacks de interrupción
    botones.configurar_interrupcion_inicio(callback_inicio);
    botones.configurar_interrupcion_pausa (callback_pausa);

    // Espera indefinida por hilos
    pthread_join(th0, nullptr);
    pthread_join(th1, nullptr);
    pthread_join(th2, nullptr);
    pthread_join(th3, nullptr);
    pthread_join(th4, nullptr);
    pthread_join(th5, nullptr);

    return 0;
}
