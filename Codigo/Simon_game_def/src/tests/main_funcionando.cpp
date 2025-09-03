//#include <iostream>
//#include <pthread.h>
//#include <unistd.h>
//#include <chrono>
//#include "state_monitor/stateMonitor.hpp"
//#include "state_monitor/threadConf.hpp"
//#include "simon_librerias/SimonBotones.hpp"
//#include "simon_librerias/SimonLeds.h"
//#include "simon_librerias/SimonBuzzer.hpp"
//#include "simon_librerias/SimonSecuencia.hpp"
//
//using namespace BBB;
//
///* ────── Globals ────── */
//StateMonitor stateMgr;
//SimonBotones botones;
//SimonLeds    leds;
//SimonBuzzer  buzzer(PWM::P9_14);
//SimonSecuencia secuencia;
//
///* Velocidad y tiempo */
//
//int nivelVelocidad = 3, nivelTiempo = 3;
//
//constexpr unsigned velDelay[4]={400,300,200,100};
//constexpr unsigned tmoBtn [4]={3000,2500,1800,1200};
//
///* ═════ ESTADO 0 — Esperar INICIO ═════ */
//void* hiloEstado0(void* p)
//{
//    auto* cfg = static_cast<threadConf*>(p);
//    while (true){
//        if (stateMgr.waitState(cfg)!=0) continue;
//
//        pthread_t tMelodia;
//        pthread_create(&tMelodia,nullptr,[](void*)->void*{
//            while(stateMgr.getState()==0) buzzer.reproducirInicio();
//            return nullptr;},nullptr);
//
//        //bool pulsadoAntes=false;
//        while (stateMgr.getState()==0){
//            leds.encenderLed(SimonLeds::START); usleep(350'000);
//            leds.apagarLed (SimonLeds::START); usleep(450'000);
//        }
//        pthread_join(tMelodia,nullptr);
//        leds.apagarLed(SimonLeds::START);
//    }
//    return nullptr;
//}
//
///* ═════ ESTADO 1 — Secuencia demo ═════ */
//void* hiloEstado1(void* p)
//{
//    auto* cfg = static_cast<threadConf*>(p);
//    while (true){
//        if (stateMgr.waitState(cfg)!=1) continue;
//        leds.encenderLedsJuego();
//		for(int c=0;c<4;++c){
//			leds.encenderLed(static_cast<SimonLeds::LedColor>(c));
//			buzzer.reproducirColor(static_cast<SimonBuzzer::COLOR>(c));
//
//			leds.apagarLed(static_cast<SimonLeds::LedColor>(c));
//			usleep(100000);
//		}
//		leds.encenderTodos();
//		buzzer.reproducirInicio();
//		usleep(200000);
//		leds.apagarTodos();
//        stateMgr.changeState(2);
//    }
//    return nullptr;
//}
//
///* ═════ ESTADO 2 — Mostrar secuencia ═════ */
//void* hiloEstado2(void* p)
//{
//    auto* cfg = static_cast<threadConf*>(p);
//    while (true){
//        if (stateMgr.waitState(cfg)!=2) continue;
//        for(int i=0;i<secuencia.getNivelActual();++i){
//            int col=secuencia.getColor(i);
//            leds.encenderLed(static_cast<SimonLeds::LedColor>(col));
//            buzzer.reproducirColor(static_cast<SimonBuzzer::COLOR>(col),velDelay[nivelVelocidad]);
//            usleep(velDelay[nivelVelocidad]*1000);
//            leds.apagarLed(static_cast<SimonLeds::LedColor>(col));
//            usleep(velDelay[nivelVelocidad]*500);
//        }
//        stateMgr.changeState(3);
//    }
//    return nullptr;
//}
//
///* ═════ ESTADO 3 — Leer jugador ═════ */
//void* hiloEstado3(void* p)
//{
//    auto* cfg = static_cast<threadConf*>(p);
//    while (true){
//        if (stateMgr.waitState(cfg)!=3) continue;
//
//        bool fallo=false;
//        for(int i=0;i<secuencia.getNivelActual() && !fallo;++i){
//            auto start=std::chrono::steady_clock::now();
//            bool leido=false; int esp=secuencia.getColor(i);
//            while(!leido){
//                for(int c=0;c<4;++c)
//                    if (botones.leer_boton(static_cast<SimonBotones::COLOR>(c))){
//                        buzzer.reproducirColor(static_cast<SimonBuzzer::COLOR>(c),200);
//                        leido=true; if(c!=esp) fallo=true;
//                        while (botones.leer_boton(static_cast<SimonBotones::COLOR>(c))) usleep(10'000);
//                        usleep(100'000); break;
//                    }
//                if (std::chrono::duration_cast<std::chrono::milliseconds>(
//                        std::chrono::steady_clock::now()-start).count()>tmoBtn[nivelTiempo]){
//                    fallo=true; break;
//                }
//                usleep(10'000);
//            }
//        }
//        if (fallo){ buzzer.reproducirFallo(); secuencia.reiniciar(); stateMgr.changeState(0);}
//        else      { buzzer.reproducirAcierto(); secuencia.añadirColor(); stateMgr.changeState(4);}
//    }
//    return nullptr;
//}
//
//void* hiloEstado4(void* p)
//{
//    auto* cfg = static_cast<threadConf*>(p);
//    constexpr int TIMEOUT_MS = 3000;
//
//    while (true)
//    {
//        if (stateMgr.waitState(cfg) != 4) continue;
//
//        auto inicio = std::chrono::steady_clock::now();
//        while (stateMgr.getState() == 4)
//        {
//            /* parpadeo */
//            leds.encenderLed(SimonLeds::PAUSE); usleep(350'000);
//            leds.apagarLed (SimonLeds::PAUSE); usleep(400'000);
//
//            /* timeout → volver a 2 */
//            auto ahora = std::chrono::steady_clock::now();
//            if (std::chrono::duration_cast<std::chrono::milliseconds>(ahora-inicio).count() > TIMEOUT_MS)
//            {
//                stateMgr.changeState(2);
//                break;
//            }
//        }
//    }
//    return nullptr;
//}
//
///* ═════ ESTADO 5 — Pausa, LED PAUSE parpadea; LEDs Simon encendidos ═════ */
//void* hiloEstado5(void* p)
//{
//    auto* cfg = static_cast<threadConf*>(p);
//
//    constexpr int ON_MS  = 700;   // encendido 0,7 s
//    constexpr int OFF_MS = 400;   // apagado 0,4 s
//
//    while (true)
//    {
//        /* Espera a que StateMonitor cambie a 5 */
//        if (stateMgr.waitState(cfg) != 5)
//            continue;
//
//        std::cout << "[PAUSA] Pulsa PAUSA para reanudar…\n";
//
//        /* Enciende los cuatro LEDs del Simon de forma fija */
//        leds.encenderLedsJuego();      // <-- nómbrala según tu API
//
//        /* Bucle de parpadeo mientras siga en estado 5 */
//        while (stateMgr.getState() == 5)
//        {
//            leds.encenderLed(SimonLeds::PAUSE);
//            for (int i = 0; i < ON_MS/50 && stateMgr.getState() == 5; ++i)
//                usleep(50'000);        // 50 ms
//
//            leds.apagarLed(SimonLeds::PAUSE);
//            for (int i = 0; i < OFF_MS/50 && stateMgr.getState() == 5; ++i)
//                usleep(50'000);
//        }
//
//        /* Sale de estado 5 → apaga LED PAUSE y LEDs del Simon */
//
//
//    }
//    return nullptr;
//}
//
//
//#include <chrono>
//
///* ---- INICIO -------------------------------------------------- */
//int callback_inicio(int /*gpio*/)
//{
//    /* a) ignora los 2 primeros edges (arranque + rebote) */
//    static int edgeCount = 0;
//    if (edgeCount < 2) { ++edgeCount; return 0; }
//
//    /* b) verifica que el pin esté realmente en LOW (= pulsado) */
//    if (!botones.leer_boton(SimonBotones::INICIO))   // pull-up + FALLING
//        return 0;                                    // fantasma → descarta
//    usleep(10000);
//    /* c) lógica normal */
//    if (stateMgr.getState() == 0) {
//        secuencia.reiniciar();
//        secuencia.añadirColor();
//        stateMgr.changeState(1);
//    }
//    return 0;
//}
//
//// ---------- PAUSA ----------
//int callback_pausa(int /*gpio*/)
//{
//	/* a) ignora los 2 primeros edges (arranque + rebote) */
//    static int edgeCount = 0;
//    if (edgeCount < 2) { ++edgeCount; return 0; }
//
//    /* 2. Comprobamos si realmente está pulsado ahora (LOW)      */
//    if (!botones.leer_boton(SimonBotones::PAUSA))        // HIGH → no hay pulsación real
//        return 0;
//    usleep(10000);
//    /* 3. Esta es una pulsación válida: actuamos según el estado */
//    int s = stateMgr.getState();
//    if      (s == 4){
//    	stateMgr.changeState(5);
//    }else if (s == 5){
//    	leds.apagarTodos();        // idem, usa tu función de apagado
//    	leds.apagarLed(SimonLeds::PAUSE);
//    	stateMgr.changeState(2);
//    }
//
//    return 0;
//}
//
///* ═════ MAIN ═════ */
//int main()
//{
//	// Hilos
//    pthread_t th0,th1,th2,th3,th4,th5; pthread_attr_t attr; pthread_attr_init(&attr);
//
//    threadConf cfg0,cfg1,cfg2,cfg3,cfg4,cfg5;
//    cfg0.addState(0); cfg1.addState(1); cfg2.addState(2);
//    cfg3.addState(3); cfg4.addState(4); cfg5.addState(5);
//
//    pthread_create(&th0,&attr,hiloEstado0,&cfg0);
//    pthread_create(&th1,&attr,hiloEstado1,&cfg1);
//    pthread_create(&th2,&attr,hiloEstado2,&cfg2);
//    pthread_create(&th3,&attr,hiloEstado3,&cfg3);
//    pthread_create(&th4,&attr,hiloEstado4,&cfg4);
//    pthread_create(&th5,&attr,hiloEstado5,&cfg5);
//
//    stateMgr.changeState(0);
//    /* Limpia transición pendiente */
//	botones.leer_boton(SimonBotones::INICIO);
//	botones.leer_boton(SimonBotones::PAUSA);
//
//	// Configurar las interrupciones
//	botones.configurar_interrupcion_inicio(callback_inicio);
//	botones.configurar_interrupcion_pausa(callback_pausa);
//	// Hilos
//    pthread_join(th0,nullptr); pthread_join(th1,nullptr);
//    pthread_join(th2,nullptr); pthread_join(th3,nullptr);
//    pthread_join(th4,nullptr); pthread_join(th5,nullptr);
//    return 0;
//}


