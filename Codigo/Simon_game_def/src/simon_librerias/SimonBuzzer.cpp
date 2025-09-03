#include "SimonBuzzer.hpp"
#include <unistd.h>
#include <algorithm>  // std::shuffle
#include <random>     // std::random_device, std::mt19937


namespace BBB {

SimonBuzzer::SimonBuzzer(PWM::pinPWM pin) : pwm(pin) {
    pwm.setPolarity(PWM::ACTIVE_HIGH);
    pwm.setPeriod(20000);
    pwm.run();
}

SimonBuzzer::~SimonBuzzer() {
    pwm.stop();
}

void SimonBuzzer::reproducirNota(const Nota& nota, float duty) {
    if (nota.frecuencia > 0) {
        unsigned period_ns = static_cast<unsigned>(1e9 / nota.frecuencia);
        pwm.setPeriod(period_ns);
        pwm.setDutyCycle(static_cast<unsigned>(period_ns * duty));
    } else {
        pwm.setDutyCycle(0u);  // silencio
    }

    usleep(nota.duracion_ms * 1000);
    pwm.setDutyCycle(0u);
    usleep(50000);
}

void SimonBuzzer::reproducirMelodia(const std::vector<Nota>& melodia, float duty) {
    keepPlaying = true;
	for (const Nota& n : melodia) {
        if (keepPlaying){
        	reproducirNota(n, duty);
        }else{
        	break;
        }

    }
}

void SimonBuzzer::reproducirInicio() {
    // Demo de colores
    std::vector<Nota> melodia = {
        {C5, 150}, {REST, 50},
        {E5, 150}, {REST, 50},
        {G5, 150}, {REST, 100},
        // Cierre breve y contundente
        {C6, 200}, {REST, 100}
    };
    reproducirMelodia(melodia, 0.25f);

}

void SimonBuzzer::reproducirFallo() {
    // Efecto de fallo: caída dramática
    std::vector<Nota> melodia = {
        {C5, 100},    // inicio agudo
        {A4, 100},    // baja un tono
        {F4, 100},    // baja un tercer
        {D4, 200},    // sostenido final
        {REST, 150}
    };
    // Usamos un duty más alto para que suene fuerte
    reproducirMelodia(melodia, 0.40f);
}


void SimonBuzzer::reproducirAcierto() {
    // Melodía de acierto más viva y ascendente
    std::vector<Nota> melodia = {
        {C5, 100},  // tónica
        {E5, 100},  // tercera
        {G5, 150},  // quinta
        {C6, 200},  // octava
        {REST, 100} // breve silencio para separar
    };
    reproducirMelodia(melodia, 0.35f);  // duty un poco más alto para más presencia
}

void SimonBuzzer::reproducirSilencio() {
    std::vector<Nota> melodia = {
    		{REST, 1000}
    };
    reproducirMelodia(melodia);
}
void SimonBuzzer::reproducirFaseExtra(){
		auto notaDur = [&](int len) {
			int base = 60000 / 120;        // ms por negra a 120 BPM = 500 ms
			int n = abs(len);
			int dur = base * 4 / n;        // base*(4/denominador)
			if (len < 0) dur = dur + dur/2; // nota con punto → 1.5×
			return dur;
		};
		std::vector<Nota> melodia = {
		    {G5,    notaDur(8)},  // paso inicial
		    {B5,    notaDur(8)},  // sube la tensión
		    {D6,    notaDur(4)},  // cima de fase
		    {REST,  notaDur(8)}   // breve silencio antes de continuar
		};

		reproducirMelodia(melodia, 0.6f);  // duty 60% para un tono más brillante
}

void SimonBuzzer::reproducirVictoria() {
	// Notas «victoria» estilo FF (uso constantes de notas.h y notaDur)
	// Aquí una fanfarria de 8 tonos corta:
	auto notaDur = [&](int len) {
		int base = 60000 / 120;        // ms por negra a 120 BPM = 500 ms
		int n = abs(len);
		int dur = base * 4 / n;        // base*(4/denominador)
		if (len < 0) dur = dur + dur/2; // nota con punto → 1.5×
		return dur;
	};
	std::vector<Nota> melodia = {
	    {E6,    notaDur(8)},  // subida rápida
	    {C6,    notaDur(8)},  // recordando a Mario
	    {G5,    notaDur(4)},  // base melódica
	    {E5,    notaDur(4)},  // llamada al tema
	    {A5,    notaDur(8)},  // variación triunfal
	    {B5,    notaDur(8)},
	    {Bb5,   notaDur(8)},  // pequeña disonancia característica
	    {A5,    notaDur(8)},  // resolución
	    {G5,    notaDur(4)},  // paso hacia el cierre
	    {E5,    notaDur(4)},
	    {C5,    notaDur(2)},  // nota final
	    {REST,  notaDur(4)}   // silencio para rematar
	};

	reproducirMelodia(melodia, 0.6f);  // duty 60% para un tono más brillante
}

void SimonBuzzer::reproducirColor(COLOR color, int duracion_ms) {
    Nota n = {0.0, duracion_ms};
    float duty = 0.60f; // 30% en lugar del tipico 15-20%
    switch (color) {
        case ROJO:     n.frecuencia = D5; break;
        case VERDE:    n.frecuencia = C5; break;
        case AMARILLO: n.frecuencia = G5; break;
        case AZUL:     n.frecuencia = F5; break;
    }
    reproducirNota(n, duty);
}

void SimonBuzzer::reproducirConfirmacion(int duracion_ms) {
    Nota n = {A5, duracion_ms};
    reproducirNota(n);
}

void SimonBuzzer::reproducirZeldaMainTheme() {
	// Helper lambda para calcular duración en ms
	    auto notaDur = [&](int len) {
	        int base = 60000 / 120;        // ms por negra a 120 BPM = 500 ms
	        int n = abs(len);
	        int dur = base * 4 / n;        // base*(4/denominador)
	        if (len < 0) dur = dur + dur/2; // nota con punto → 1.5×
	        return dur;
	    };

	    std::vector<Nota> melodia = {
	        // compás 1
	        {As4,   notaDur(-2)},  // blanca con punto
	        {F4,    notaDur(8)},   // corchea
	        {F4,    notaDur(8)},   // corchea
	        {As4,   notaDur(8)},   // corchea

	        {Gs4,   notaDur(16)},  // semicorchea
	        {Fs4,   notaDur(16)},  // semicorchea
	        {Gs4,   notaDur(-2)},  // blanca con punto

	        // compás 2
	        {As4,   notaDur(-2)},
	        {Fs4,   notaDur(8)},
	        {Fs4,   notaDur(8)},
	        {As4,   notaDur(8)},

	        {A4,    notaDur(16)},
	        {G4,    notaDur(16)},
	        {A4,    notaDur(-2)},

	        {REST,  notaDur(1)},   // redonda

	        // compás 3
	        {As4,   notaDur(4)},
	        {F4,    notaDur(-4)},
	        {As4,   notaDur(8)},
	        {As4,   notaDur(16)},
	        {C5,    notaDur(16)},
	        {D5,    notaDur(16)},
	        {Ds5,   notaDur(16)},

	        {F5,    notaDur(2)},
	        {F5,    notaDur(8)},
	        {F5,    notaDur(8)},
	        {F5,    notaDur(8)},
	        {Fs5,   notaDur(16)},
	        {Gs5,   notaDur(16)},

	        {As5,   notaDur(-2)},
	        {As5,   notaDur(8)},
	        {As5,   notaDur(8)},
	        {Gs5,   notaDur(8)},
	        {Fs5,   notaDur(16)},

	        {Gs5,   notaDur(-8)},
	        {Fs5,   notaDur(16)},
	        {F5,    notaDur(2)},
	        {F5,    notaDur(4)},

	        // compás 4
	        {Ds5,   notaDur(-8)},
	        {F5,    notaDur(16)},
	        {Fs5,   notaDur(2)},
	        {F5,    notaDur(8)},
	        {Ds5,   notaDur(8)},

	        {Cs5,   notaDur(-8)},
	        {Ds5,   notaDur(16)},
	        {F5,    notaDur(2)},
	        {Ds5,   notaDur(8)},
	        {Cs5,   notaDur(8)},

	        {C5,    notaDur(-8)},
	        {D5,    notaDur(16)},
	        {E5,    notaDur(2)},
	        {G5,    notaDur(8)},

	        // relleno rítmico de F4
	        {F5,    notaDur(16)},
	        {F4,    notaDur(16)}, {F4, notaDur(16)}, {F4, notaDur(16)},
	        {F4,    notaDur(16)}, {F4, notaDur(16)}, {F4, notaDur(16)},
	        {F4,    notaDur(16)}, {F4, notaDur(8)},  {F4, notaDur(16)},
	        {F4,    notaDur(8)},

	        // compás 5 (repite frase de compás 3)
	        {As4,   notaDur(4)},
	        {F4,    notaDur(-4)},
	        {As4,   notaDur(8)},
	        {As4,   notaDur(16)},
	        {C5,    notaDur(16)},
	        {D5,    notaDur(16)},
	        {Ds5,   notaDur(16)},

	        {F5,    notaDur(2)},
	        {F5,    notaDur(8)},
	        {F5,    notaDur(8)},
	        {F5,    notaDur(8)},
	        {Fs5,   notaDur(16)},
	        {Gs5,   notaDur(16)},

	        {As5,   notaDur(-2)},
	        {Cs6,   notaDur(4)},

	        {C6,    notaDur(4)},
	        {A5,    notaDur(2)},
	        {F5,    notaDur(4)},

	        {Fs5,   notaDur(-2)},
	        {As5,   notaDur(4)},

	        {A5,    notaDur(4)},
	        {F5,    notaDur(2)},
	        {F5,    notaDur(4)},

	        {Fs5,   notaDur(-2)},
	        {As5,   notaDur(4)},

	        {A5,    notaDur(4)},
	        {F5,    notaDur(2)},
	        {D5,    notaDur(4)},

	        {Ds5,   notaDur(-2)},
	        {Fs5,   notaDur(4)},

	        {F5,    notaDur(4)},
	        {Cs5,   notaDur(2)},
	        {As4,   notaDur(4)},

	        // final rítmico
	        {C5,    notaDur(-8)}, {D5, notaDur(16)}, {E5, notaDur(2)}, {G5, notaDur(8)},
	        {F5,    notaDur(16)}, {F4, notaDur(16)}, {F4, notaDur(16)}, {F4, notaDur(16)},
	        {F4,    notaDur(16)}, {F4, notaDur(16)}, {F4, notaDur(16)}, {F4, notaDur(16)},
	        {F4,    notaDur(8)},  {F4, notaDur(16)}, {F4, notaDur(8)}
	    };
	    reproducirMelodia(melodia, 0.15);
}

void SimonBuzzer::reproducirZeldaLullaby() {
	// Helper lambda para calcular duración en ms
	    auto notaDur = [&](int len) {
	        int base = 60000 / 120;        // ms por negra a 120 BPM = 500 ms
	        int n = abs(len);
	        int dur = base * 4 / n;        // base*(4/denominador)
	        if (len < 0) dur = dur + dur/2; // nota con punto → 1.5×
	        return dur;
	    };

	    std::vector<Nota> melodia = {
	    		// Frase inicial
				{ E4,  notaDur(2) }, { G4,  notaDur(4) },
				{ D4,  notaDur(2) }, { C4,  notaDur(8) }, { D4, notaDur(8) },
				{ E4,  notaDur(2) }, { G4,  notaDur(4) },
				{ D4,  notaDur(-2) },

				// Repite ligero
				{ E4,  notaDur(2) }, { G4,  notaDur(4) },
				{ D5,  notaDur(2) }, { C5,  notaDur(4) },

				{ G4,  notaDur(2) }, { F4,  notaDur(8) }, { E4, notaDur(8) },
				{ D4,  notaDur(-2) },

				// Otra variación
				{ E4,  notaDur(2) }, { G4,  notaDur(4) },
				{ D4,  notaDur(2) }, { C4,  notaDur(8) }, { D4, notaDur(8) },
				{ E4,  notaDur(2) }, { G4,  notaDur(4) },
				{ D4,  notaDur(-2) },

				// Segunda sección
				{ E4,  notaDur(2) }, { G4,  notaDur(4) },
				{ D5,  notaDur(2) }, { C5,  notaDur(4) },

				{ G4,  notaDur(2) }, { F4,  notaDur(8) }, { E4, notaDur(8) },
				{ F4,  notaDur(8) }, { E4, notaDur(8) }, { C4, notaDur(2) },

				{ F4,  notaDur(2) }, { E4,  notaDur(8) }, { D4, notaDur(8) },
				{ E4,  notaDur(8) }, { D4, notaDur(8) }, { A3, notaDur(2) },

				{ G4,  notaDur(2) }, { F4,  notaDur(8) }, { E4, notaDur(8) },
				{ F4,  notaDur(8) }, { E4, notaDur(8) }, { C4, notaDur(4) }, { F4, notaDur(4) },

				{ C5,  notaDur(-2) }
	    };
	    reproducirMelodia(melodia, 0.15);
}

void SimonBuzzer::reproducirMelodias(const std::vector<std::vector<Nota>>& melodias, float duty) {
    keepPlaying = true;
    for (const auto& melodia : melodias) {
        if (keepPlaying) {
            reproducirMelodia(melodia, duty);
        } else {
            break;
        }
    }
}

void SimonBuzzer::reproducirMelodiasIntro(){
	// Helper lambda para calcular duración en ms
		    auto notaDur = [&](int len) {
		        int base = 60000 / 120;        // ms por negra a 120 BPM = 500 ms
		        int n = abs(len);
		        int dur = base * 4 / n;        // base*(4/denominador)
		        if (len < 0) dur = dur + dur/2; // nota con punto → 1.5×
		        return dur;
		    };
		    std::vector<Nota> MelodiaProfessorLaytonIntro = {
		    	{D5, notaDur(1)},
		    	{Ds5, notaDur(1)},
		    	{F5, notaDur(1)},
		    	{REST, notaDur(4)},
		    	{F5, notaDur(-4)},
		    	{Ds5, notaDur(8)},
		    	{D5, notaDur(8)},
		    	{F5, notaDur(1)},
		    	{As4, notaDur(8)},
		    	{G4, notaDur(-2)},
		    	{F4, notaDur(1)},
		    	{F4, notaDur(1)},
		    	{REST, notaDur(4)},
		    	{REST, notaDur(8)},
		    	{F4, notaDur(8)},
		    	{G4, notaDur(8)},
		    	{Gs4, notaDur(8)},
		    	{As4, notaDur(8)},
		    	{C5, notaDur(8)},
		    	{D5, notaDur(1)},
		    	{Ds5, notaDur(1)},
		    	{F5, notaDur(1)},
		    	{F5, notaDur(-4)},
		    	{Ds5, notaDur(8)},
		    	{D5, notaDur(8)},
		    	{Cs5, notaDur(8)},
		    	{C5, notaDur(-2)},
		    	{As4, notaDur(8)},
		    	{G4, notaDur(1)},
		    	{F4, notaDur(-1)},
		    	{REST, notaDur(4)},
		    	{D5, notaDur(-4)},
		    	{REST, notaDur(16)},
		    	{D5, notaDur(16)},
		    	{D5, notaDur(2)},
		    	{REST, notaDur(4)},
		    	{D5, notaDur(8)},
		    	{Ds5, notaDur(8)},
		    	{F5, notaDur(8)},
		    	{G5, notaDur(8)},
		    	{F5, notaDur(8)},
		    	{Ds5, notaDur(8)},
		    	{D5, notaDur(8)},
		    	{D5, notaDur(-4)},
		    	{Ds5, notaDur(16)},
		    	{Ds5, notaDur(2)},
		    	{REST, notaDur(4)},
		    	{G4, notaDur(8)},
		    	{C5, notaDur(8)},
		    	{D5, notaDur(8)},
		    	{Ds5, notaDur(8)},
		    	{F5, notaDur(8)},
		    	{Ds5, notaDur(8)},
		    	{D5, notaDur(8)},
		    	{C5, notaDur(-4)},
		    	{REST, notaDur(16)},
		    	{G4, notaDur(2)},
		    	{REST, notaDur(4)},
		    	{G4, notaDur(8)},
		    	{Gs4, notaDur(8)},
		    	{As4, notaDur(8)},
		    	{C5, notaDur(8)},
		    	{As4, notaDur(8)},
		    	{Gs4, notaDur(8)},
		    	{G5, notaDur(8)},
		    	{F4, notaDur(-4)},
		    	{As4, notaDur(-4)},
		    	{G4, notaDur(2)},
		    	{REST, notaDur(8)},
		    	{C4, notaDur(8)},
		    	{D4, notaDur(8)},
		    	{Ds4, notaDur(8)},
		    	{G4, notaDur(8)},
		    	{C5, notaDur(8)},
		    	{D5, notaDur(-4)},
		    	{REST, notaDur(16)},
		    	{D5, notaDur(-16)},
		    	{D5, notaDur(2)},
		    	{REST, notaDur(4)},
		    	{D5, notaDur(8)},
		    	{Ds5, notaDur(8)},
		    	{F5, notaDur(8)},
		    	{G5, notaDur(8)},
		    	{F5, notaDur(8)},
		    	{Ds5, notaDur(8)},
		    	{D5, notaDur(8)},
		    	{D5, notaDur(-4)},
		    	{Ds5, notaDur(-16)},
		    	{Ds5, notaDur(2)},
		    	{REST, notaDur(4)},
		    	{C5, notaDur(8)},
		    	{D5, notaDur(8)},
		    	{Ds5, notaDur(8)},
		    	{F5, notaDur(8)},
		    	{Ds5, notaDur(8)},
		    	{D5, notaDur(8)},
		    	{As4, notaDur(8)},
		    	{As4, notaDur(-4)},
		    	{C5, notaDur(-4)},
		    	{C5, notaDur(-4)},
		    	{F4, notaDur(-4)},
		    	{REST, notaDur(8)},
		    	{G4, notaDur(4)},
		    	{D5, notaDur(4)},
		    	{Ds5, notaDur(4)},
		    	{D5, notaDur(-4)},
		    	{REST, notaDur(16)},
		    	{C5, notaDur(16)},
		    	{C5, notaDur(2)},
		    	{REST, notaDur(4)},
		    	{D5, notaDur(4)},
		    	{Ds5, notaDur(4)},
		    	{F5, notaDur(4)},
		    	{G5, notaDur(-4)},
		    	{REST, notaDur(16)},
		    	{F5, notaDur(2)},
		    	{As5, notaDur(-4)},
		    	{G5, notaDur(-4)},
		    	{Ds5, notaDur(4)},
		    	{D5, notaDur(-4)},
		    	{REST, notaDur(16)},
		    	{Ds5, notaDur(2)},
		    	{REST, notaDur(4)},
		    	{C5, notaDur(8)},
		    	{D5, notaDur(8)},
		    	{Ds5, notaDur(8)},
		    	{E5, notaDur(8)},
		    	{F5, notaDur(8)},
		    	{Fs5, notaDur(8)},
		    	{G5, notaDur(-4)},
		    	{F5, notaDur(-4)},
		    	{REST, notaDur(4)},
		    	{As5, notaDur(2)},
		    	{G5, notaDur(4)},
		    	{F5, notaDur(8)},
		    	{G5, notaDur(8)},
		    	{REST, notaDur(8)},
		    	{E5, notaDur(8)},
		    	{REST, notaDur(8)},
		    	{D5, notaDur(8)},
		    	{C5, notaDur(-2)},
		    	{REST, notaDur(8)},
		    	{G4, notaDur(8)},
		    	{A4, notaDur(8)},
		    	{As4, notaDur(8)},
		    	{C5, notaDur(8)},
		    	{D5, notaDur(8)},
		    	{Ds5, notaDur(8)},
		    	{Ds5, notaDur(-4)},
		    	{D5, notaDur(-4)},
		    	{As4, notaDur(4)},
		    	{REST, notaDur(4)},
		    	{Ds5, notaDur(8)},
		    	{E5, notaDur(8)},
		    	{F5, notaDur(4)},
		    	{E5, notaDur(8)},
		    	{Ds5, notaDur(8)},
		    	{D5, notaDur(8)},
		    	{As5, notaDur(8)},
		    	{C5, notaDur(4)},
		    	{G4, notaDur(8)},
		    	{D5, notaDur(4)},
		    	{G4, notaDur(8)},
		    	{D5, notaDur(4)},
		    	{REST, notaDur(8)},
		    	{Fs5, notaDur(8)},
		    	{G5, notaDur(8)},
		    	{Fs5, notaDur(8)},
		    	{F5, notaDur(8)},
		    	{Ds5, notaDur(8)},
		    	{D5, notaDur(8)},
		    	{Ds5, notaDur(8)},
		    	{REST, notaDur(8)},
		    	{As5, notaDur(8)},
		    	{G5, notaDur(8)},
		    	{Ds5, notaDur(8)},
		    	{F5, notaDur(8)},
		    	{REST, notaDur(8)},
		    	{G5, notaDur(8)},
		    	{REST, notaDur(8)},
		    	{Fs5, notaDur(8)},
		    	{F5, notaDur(8)},
		    	{Ds5, notaDur(8)},
		    	{F5, notaDur(8)},
		    	{Ds5, notaDur(8)},
		    	{D5, notaDur(8)},
		    	{Ds5, notaDur(8)},
		    	{D5, notaDur(-4)},
		    	{C5, notaDur(-4)},
		    	{REST, notaDur(4)},
		    	{REST, notaDur(4)},
		    	{C5, notaDur(8)},
		    	{D5, notaDur(8)},
		    	{Ds5, notaDur(8)},
		    	{D5, notaDur(8)},
		    	{C5, notaDur(8)},
		    	{As4, notaDur(8)},
		    	{D5, notaDur(8)},
		    	{Ds5, notaDur(8)},
		    	{F5, notaDur(8)},
		    	{G5, notaDur(8)},
		    	{D5, notaDur(8)},
		    	{C5, notaDur(8)},
		    	{D5, notaDur(8)},
		    	{Ds5, notaDur(8)},
		    	{F5, notaDur(8)},
		    	{G5, notaDur(8)},
		    	{As5, notaDur(8)},
		    	{Gs5, notaDur(8)},
		    	{G5, notaDur(8)},
		    	{F5, notaDur(8)},
		    	{Ds5, notaDur(8)},
		    	{F5, notaDur(8)},
		    	{Ds5, notaDur(8)},
		    	{D5, notaDur(16)},
		    	{Ds5, notaDur(16)},
		    	{D5, notaDur(16)},
		    	{As4, notaDur(8)},
		    	{C5, notaDur(8)},
		    	{D5, notaDur(8)},
		    	{Ds5, notaDur(8)},
		    	{F5, notaDur(8)},
		    	{G5, notaDur(8)},
		    	{As5, notaDur(8)},
		    	{Gs5, notaDur(8)},
		    	{G5, notaDur(8)},
		    	{F5, notaDur(8)},
		    	{Ds5, notaDur(8)},
		    	{D5, notaDur(8)},
		    	{Ds5, notaDur(8)},
		    	{C5, notaDur(8)},
		    	{D5, notaDur(8)},
		    	{Ds5, notaDur(8)},
		    	{F5, notaDur(8)},
		    	{C5, notaDur(8)},
		    	{G4, notaDur(8)},
		    	{C5, notaDur(8)},
		    	{D5, notaDur(8)},
		    	{Ds5, notaDur(8)},
		    	{F5, notaDur(8)},
		    	{As5, notaDur(8)},
		    	{F5, notaDur(8)},
		    	{Ds5, notaDur(8)},
		    	{D5, notaDur(8)},
		    	{As4, notaDur(8)},
		    	{Ds5, notaDur(8)},
		    	{D5, notaDur(8)},
		    	{D5, notaDur(16)},
		    	{Ds5, notaDur(16)},
		    	{D5, notaDur(16)},
		    	{G4, notaDur(8)},
		    	{C5, notaDur(8)},
		    	{D5, notaDur(8)},
		    	{Ds5, notaDur(8)},
		    	{F5, notaDur(8)},
		    	{F5, notaDur(8)},
		    	{As5, notaDur(8)},
		    	{F5, notaDur(8)},
		    	{Ds5, notaDur(8)},
		    	{D5, notaDur(8)},
		    	{Ds5, notaDur(8)},
		    	{D5, notaDur(8)},
		    	{As4, notaDur(8)},
		    	{D5, notaDur(8)},
		    	{Ds5, notaDur(8)},
		    	{F5, notaDur(8)},
		    	{G5, notaDur(8)},
		    	{As4, notaDur(8)},
		    	{G4, notaDur(8)},
		    	{As4, notaDur(8)},
		    	{Ds5, notaDur(8)},
		    	{As5, notaDur(8)},
		    	{Ds5, notaDur(8)},
		    	{As5, notaDur(8)},
		    	{Gs5, notaDur(8)},
		    	{G5, notaDur(8)},
		    	{Gs5, notaDur(8)},
		    	{G5, notaDur(8)},
		    	{F5, notaDur(8)},
		    	{Ds5, notaDur(8)},
		    	{D5, notaDur(16)},
		    	{Ds5, notaDur(16)},
		    	{D5, notaDur(16)},
		    	{As4, notaDur(8)},
		    	{C5, notaDur(8)},
		    	{D5, notaDur(8)},
		    	{Ds5, notaDur(8)},
		    	{F5, notaDur(8)},
		    	{C6, notaDur(8)},
		    	{D5, notaDur(8)},
		    	{As5, notaDur(8)},
		    	{D5, notaDur(8)},
		    	{C5, notaDur(8)},
		    	{D5, notaDur(8)},
		    	{B5, notaDur(8)},
		    	{G4, notaDur(8)},
		    	{C4, notaDur(8)},
		    	{Ds4, notaDur(8)},
		    	{G4, notaDur(8)},
		    	{C5, notaDur(8)},
		    	{Ds5, notaDur(8)},
		    	{G5, notaDur(8)},
		    	{REST, notaDur(8)},
		    	{C5, notaDur(8)},
		    	{D5, notaDur(8)},
		    	{Ds5, notaDur(8)},
		    	{D5, notaDur(16)},
		    	{Ds5, notaDur(16)},
		    	{D5, notaDur(16)},
		    	{C5, notaDur(8)},
		    	{G4, notaDur(8)},
		    	{C5, notaDur(8)},
		    	{G5, notaDur(8)},
		    	{D5, notaDur(-4)},
		    	{C5, notaDur(8)},
		    	{C5, notaDur(1)},
		    	{REST, notaDur(4)},
		    	{Ds4, notaDur(8)},
		    	{C4, notaDur(-4)},
		    	{Ds4, notaDur(2)},
		    	{D6, notaDur(2)},
		    	{B3, notaDur(2)},
		    	{Ds4, notaDur(8)},
		    	{C4, notaDur(-4)},
		    	{G3, notaDur(2)},
		    	{D6, notaDur(2)},
		    	{B3, notaDur(2)},
		    	{Ds4, notaDur(8)},
		    	{C4, notaDur(-4)},
		    	{G4, notaDur(2)},
		    	{Fs4, notaDur(2)},
		    	{D4, notaDur(2)},
		    	{F4, notaDur(2)},
		    	{D4, notaDur(2)},
		    	{D4, notaDur(2)},
		    	{G4, notaDur(2)},
		    	{G4, notaDur(1)},
		    	{G4, notaDur(1)},
		    	{G4, notaDur(1)},
		    	{G4, notaDur(1)},
		    	{REST, notaDur(1)},
		    	{REST, notaDur(1)},
		    	{G4, notaDur(1)},
		    	{G4, notaDur(1)},
		    	{Ds4, notaDur(2)},
		    	{G4, notaDur(2)},
		    	{G4, notaDur(2)},
		    	{C4, notaDur(4)},
		    	{D4, notaDur(8)},
		    	{Ds4, notaDur(8)},
		    	{F4, notaDur(2)},
		    	{As4, notaDur(2)},
		    	{As4, notaDur(2)},
		    	{C4, notaDur(4)},
		    	{D4, notaDur(8)},
		    	{Ds4, notaDur(8)},
		    	{Ds4, notaDur(2)},
		    	{G4, notaDur(-2)},
		    	{F4, notaDur(2)},
		    	{G4, notaDur(8)},
		    	{F4, notaDur(8)},
		    	{G4, notaDur(-2)},
		    	{D4, notaDur(-1)},
		    	{C4, notaDur(2)},
		    	{G4, notaDur(-2)},
		    	{F4, notaDur(2)},
		    	{D4, notaDur(8)},
		    	{Ds4, notaDur(8)},
		    	{F4, notaDur(2)},
		    	{As3, notaDur(2)},
		    	{As4, notaDur(2)},
		    	{C4, notaDur(4)},
		    	{D4, notaDur(8)},
		    	{Ds4, notaDur(8)},
		    	{Ds4, notaDur(2)},
		    	{As4, notaDur(-2)},
		    	{Gs4, notaDur(2)},
		    	{G4, notaDur(8)},
		    	{F4, notaDur(8)},
		    	{F4, notaDur(8)},
		    	{G4, notaDur(-1)},
		    	{Ds4, notaDur(2)},
		    	{G4, notaDur(2)},
		    	{G4, notaDur(2)},
		    	{C4, notaDur(4)},
		    	{D4, notaDur(8)},
		    	{Ds4, notaDur(8)},
		    	{F4, notaDur(2)},
		    	{As4, notaDur(2)},
		    	{As4, notaDur(2)},
		    	{C4, notaDur(4)},
		    	{D4, notaDur(8)},
		    	{Ds4, notaDur(8)},
		    	{Ds4, notaDur(2)},
		    	{G4, notaDur(-2)},
		    	{F4, notaDur(2)},
		    	{G4, notaDur(8)},
		    	{F4, notaDur(8)},
		    	{G4, notaDur(-2)},
		    	{D4, notaDur(-1)},
		    	{C4, notaDur(2)},
		    	{G4, notaDur(-2)},
		    	{F4, notaDur(2)},
		    	{D4, notaDur(8)},
		    	{Ds4, notaDur(8)},
		    	{F4, notaDur(2)},
		    	{As3, notaDur(2)},
		    };
		    std::vector<Nota> melodiaLullaby = {
		    		// Frase inicial
					{ E4,  notaDur(2) }, { G4,  notaDur(4) },
					{ D4,  notaDur(2) }, { C4,  notaDur(8) }, { D4, notaDur(8) },
					{ E4,  notaDur(2) }, { G4,  notaDur(4) },
					{ D4,  notaDur(-2) },

					// Repite ligero
					{ E4,  notaDur(2) }, { G4,  notaDur(4) },
					{ D5,  notaDur(2) }, { C5,  notaDur(4) },

					{ G4,  notaDur(2) }, { F4,  notaDur(8) }, { E4, notaDur(8) },
					{ D4,  notaDur(-2) },

					// Otra variación
					{ E4,  notaDur(2) }, { G4,  notaDur(4) },
					{ D4,  notaDur(2) }, { C4,  notaDur(8) }, { D4, notaDur(8) },
					{ E4,  notaDur(2) }, { G4,  notaDur(4) },
					{ D4,  notaDur(-2) },

					// Segunda sección
					{ E4,  notaDur(2) }, { G4,  notaDur(4) },
					{ D5,  notaDur(2) }, { C5,  notaDur(4) },

					{ G4,  notaDur(2) }, { F4,  notaDur(8) }, { E4, notaDur(8) },
					{ F4,  notaDur(8) }, { E4, notaDur(8) }, { C4, notaDur(2) },

					{ F4,  notaDur(2) }, { E4,  notaDur(8) }, { D4, notaDur(8) },
					{ E4,  notaDur(8) }, { D4, notaDur(8) }, { A3, notaDur(2) },

					{ G4,  notaDur(2) }, { F4,  notaDur(8) }, { E4, notaDur(8) },
					{ F4,  notaDur(8) }, { E4, notaDur(8) }, { C4, notaDur(4) }, { F4, notaDur(4) },

					{ C5,  notaDur(-2) }
		    };

		    std::vector<Nota> melodiaZeldaMainTheme = {
		    	        // compás 1
		    	        {As4,   notaDur(-2)},  // blanca con punto
		    	        {F4,    notaDur(8)},   // corchea
		    	        {F4,    notaDur(8)},   // corchea
		    	        {As4,   notaDur(8)},   // corchea

		    	        {Gs4,   notaDur(16)},  // semicorchea
		    	        {Fs4,   notaDur(16)},  // semicorchea
		    	        {Gs4,   notaDur(-2)},  // blanca con punto

		    	        // compás 2
		    	        {As4,   notaDur(-2)},
		    	        {Fs4,   notaDur(8)},
		    	        {Fs4,   notaDur(8)},
		    	        {As4,   notaDur(8)},

		    	        {A4,    notaDur(16)},
		    	        {G4,    notaDur(16)},
		    	        {A4,    notaDur(-2)},

		    	        {REST,  notaDur(1)},   // redonda

		    	        // compás 3
		    	        {As4,   notaDur(4)},
		    	        {F4,    notaDur(-4)},
		    	        {As4,   notaDur(8)},
		    	        {As4,   notaDur(16)},
		    	        {C5,    notaDur(16)},
		    	        {D5,    notaDur(16)},
		    	        {Ds5,   notaDur(16)},

		    	        {F5,    notaDur(2)},
		    	        {F5,    notaDur(8)},
		    	        {F5,    notaDur(8)},
		    	        {F5,    notaDur(8)},
		    	        {Fs5,   notaDur(16)},
		    	        {Gs5,   notaDur(16)},

		    	        {As5,   notaDur(-2)},
		    	        {As5,   notaDur(8)},
		    	        {As5,   notaDur(8)},
		    	        {Gs5,   notaDur(8)},
		    	        {Fs5,   notaDur(16)},

		    	        {Gs5,   notaDur(-8)},
		    	        {Fs5,   notaDur(16)},
		    	        {F5,    notaDur(2)},
		    	        {F5,    notaDur(4)},

		    	        // compás 4
		    	        {Ds5,   notaDur(-8)},
		    	        {F5,    notaDur(16)},
		    	        {Fs5,   notaDur(2)},
		    	        {F5,    notaDur(8)},
		    	        {Ds5,   notaDur(8)},

		    	        {Cs5,   notaDur(-8)},
		    	        {Ds5,   notaDur(16)},
		    	        {F5,    notaDur(2)},
		    	        {Ds5,   notaDur(8)},
		    	        {Cs5,   notaDur(8)},

		    	        {C5,    notaDur(-8)},
		    	        {D5,    notaDur(16)},
		    	        {E5,    notaDur(2)},
		    	        {G5,    notaDur(8)},

		    	        // relleno rítmico de F4
		    	        {F5,    notaDur(16)},
		    	        {F4,    notaDur(16)}, {F4, notaDur(16)}, {F4, notaDur(16)},
		    	        {F4,    notaDur(16)}, {F4, notaDur(16)}, {F4, notaDur(16)},
		    	        {F4,    notaDur(16)}, {F4, notaDur(8)},  {F4, notaDur(16)},
		    	        {F4,    notaDur(8)},

		    	        // compás 5 (repite frase de compás 3)
		    	        {As4,   notaDur(4)},
		    	        {F4,    notaDur(-4)},
		    	        {As4,   notaDur(8)},
		    	        {As4,   notaDur(16)},
		    	        {C5,    notaDur(16)},
		    	        {D5,    notaDur(16)},
		    	        {Ds5,   notaDur(16)},

		    	        {F5,    notaDur(2)},
		    	        {F5,    notaDur(8)},
		    	        {F5,    notaDur(8)},
		    	        {F5,    notaDur(8)},
		    	        {Fs5,   notaDur(16)},
		    	        {Gs5,   notaDur(16)},

		    	        {As5,   notaDur(-2)},
		    	        {Cs6,   notaDur(4)},

		    	        {C6,    notaDur(4)},
		    	        {A5,    notaDur(2)},
		    	        {F5,    notaDur(4)},

		    	        {Fs5,   notaDur(-2)},
		    	        {As5,   notaDur(4)},

		    	        {A5,    notaDur(4)},
		    	        {F5,    notaDur(2)},
		    	        {F5,    notaDur(4)},

		    	        {Fs5,   notaDur(-2)},
		    	        {As5,   notaDur(4)},

		    	        {A5,    notaDur(4)},
		    	        {F5,    notaDur(2)},
		    	        {D5,    notaDur(4)},

		    	        {Ds5,   notaDur(-2)},
		    	        {Fs5,   notaDur(4)},

		    	        {F5,    notaDur(4)},
		    	        {Cs5,   notaDur(2)},
		    	        {As4,   notaDur(4)},

		    	        // final rítmico
		    	        {C5,    notaDur(-8)}, {D5, notaDur(16)}, {E5, notaDur(2)}, {G5, notaDur(8)},
		    	        {F5,    notaDur(16)}, {F4, notaDur(16)}, {F4, notaDur(16)}, {F4, notaDur(16)},
		    	        {F4,    notaDur(16)}, {F4, notaDur(16)}, {F4, notaDur(16)}, {F4, notaDur(16)},
		    	        {F4,    notaDur(8)},  {F4, notaDur(16)}, {F4, notaDur(8)}
		    	    };
		    std::vector<Nota> MelodiaJigglypuffSong = {
		    	{D5, notaDur(-4)},
		    	{A5, notaDur(8)},
		    	{Fs5, notaDur(8)},
		    	{D5, notaDur(8)},
		    	{E5, notaDur(-4)},
		    	{Fs5, notaDur(8)},
		    	{G5, notaDur(4)},
		    	{Fs5, notaDur(-4)},
		    	{E5, notaDur(8)},
		    	{Fs5, notaDur(4)},
		    	{D5, notaDur(-2)},
		    	{D5, notaDur(-4)},
		    	{A5, notaDur(8)},
		    	{Fs5, notaDur(8)},
		    	{D5, notaDur(8)},
		    	{E5, notaDur(-4)},
		    	{Fs5, notaDur(8)},
		    	{G5, notaDur(4)},
		    	{Fs5, notaDur(-1)},
		    	{D5, notaDur(-4)},
		    	{A5, notaDur(8)},
		    	{Fs5, notaDur(8)},
		    	{D5, notaDur(8)},
		    	{E5, notaDur(-4)},
		    	{Fs5, notaDur(8)},
		    	{G5, notaDur(4)},
		    	{Fs5, notaDur(-4)},
		    	{E5, notaDur(8)},
		    	{Fs5, notaDur(4)},
		    	{D5, notaDur(-2)},
		    	{D5, notaDur(-4)},
		    	{A5, notaDur(8)},
		    	{Fs5, notaDur(8)},
		    	{D5, notaDur(8)},
		    	{E5, notaDur(-4)},
		    	{Fs5, notaDur(8)},
		    	{G5, notaDur(4)},
		    	{Fs5, notaDur(-1)},
		    };

		    auto notaDurMario = [&](int len) {
		        int base = 60000 / 160;  // 160 BPM
		        int n = std::abs(len);
		        int dur = base * 4 / n;
		        if (len < 0) dur += dur / 2;
		        return dur;
		    };
		    std::vector<Nota> melodiaSuperMarioBros = {
		    	{E5, notaDurMario(8)},
		    	{E5, notaDurMario(8)},
		    	{REST, notaDurMario(8)},
		    	{E5, notaDurMario(8)},
		    	{REST, notaDurMario(8)},
		    	{C5, notaDurMario(8)},
		    	{E5, notaDurMario(8)},
		    	{G5, notaDurMario(4)},
		    	{REST, notaDurMario(4)},
		    	{G4, notaDurMario(8)},
		    	{REST, notaDurMario(4)},
		    	{C5, notaDurMario(-4)},
		    	{G4, notaDurMario(8)},
		    	{REST, notaDurMario(4)},
		    	{E4, notaDurMario(-4)},
		    	{A4, notaDurMario(4)},
		    	{B4, notaDurMario(4)},
		    	{As4, notaDurMario(8)},
		    	{A4, notaDurMario(4)},
		    	{G4, notaDurMario(-8)},
		    	{E5, notaDurMario(-8)},
		    	{G5, notaDurMario(-8)},
		    	{A5, notaDurMario(4)},
		    	{F5, notaDurMario(8)},
		    	{G5, notaDurMario(8)},
		    	{REST, notaDurMario(8)},
		    	{E5, notaDurMario(4)},
		    	{C5, notaDurMario(8)},
		    	{D5, notaDurMario(8)},
		    	{B4, notaDurMario(-4)},
		    	{C5, notaDurMario(-4)},
		    	{G4, notaDurMario(8)},
		    	{REST, notaDurMario(4)},
		    	{E4, notaDurMario(-4)},
		    	{A4, notaDurMario(4)},
		    	{B4, notaDurMario(4)},
		    	{As4, notaDurMario(8)},
		    	{A4, notaDurMario(4)},
		    	{G4, notaDurMario(-8)},
		    	{E5, notaDurMario(-8)},
		    	{G5, notaDurMario(-8)},
		    	{A5, notaDurMario(4)},
		    	{F5, notaDurMario(8)},
		    	{G5, notaDurMario(8)},
		    	{REST, notaDurMario(8)},
		    	{E5, notaDurMario(4)},
		    	{C5, notaDurMario(8)},
		    	{D5, notaDurMario(8)},
		    	{B4, notaDurMario(-4)},
		    	{REST, notaDurMario(4)},
		    	{G5, notaDurMario(8)},
		    	{Fs5, notaDurMario(8)},
		    	{F5, notaDurMario(8)},
		    	{Ds5, notaDurMario(4)},
		    	{E5, notaDurMario(8)},
		    	{REST, notaDurMario(8)},
		    	{Gs4, notaDurMario(8)},
		    	{A4, notaDurMario(8)},
		    	{C4, notaDurMario(8)},
		    	{REST, notaDurMario(8)},
		    	{A4, notaDurMario(8)},
		    	{C5, notaDurMario(8)},
		    	{D5, notaDurMario(8)},
		    	{REST, notaDurMario(4)},
		    	{Ds5, notaDurMario(4)},
		    	{REST, notaDurMario(8)},
		    	{D5, notaDurMario(-4)},
		    	{C5, notaDurMario(2)},
		    	{REST, notaDurMario(2)},
		    	{REST, notaDurMario(4)},
		    	{G5, notaDurMario(8)},
		    	{Fs5, notaDurMario(8)},
		    	{F5, notaDurMario(8)},
		    	{Ds5, notaDurMario(4)},
		    	{E5, notaDurMario(8)},
		    	{REST, notaDurMario(8)},
		    	{Gs4, notaDurMario(8)},
		    	{A4, notaDurMario(8)},
		    	{C4, notaDurMario(8)},
		    	{REST, notaDurMario(8)},
		    	{A4, notaDurMario(8)},
		    	{C5, notaDurMario(8)},
		    	{D5, notaDurMario(8)},
		    	{REST, notaDurMario(4)},
		    	{Ds5, notaDurMario(4)},
		    	{REST, notaDurMario(8)},
		    	{D5, notaDurMario(-4)},
		    	{C5, notaDurMario(2)},
		    	{REST, notaDurMario(2)},
		    	{C5, notaDurMario(8)},
		    	{C5, notaDurMario(4)},
		    	{C5, notaDurMario(8)},
		    	{REST, notaDurMario(8)},
		    	{C5, notaDurMario(8)},
		    	{D5, notaDurMario(4)},
		    	{E5, notaDurMario(8)},
		    	{C5, notaDurMario(4)},
		    	{A4, notaDurMario(8)},
		    	{G4, notaDurMario(2)},
		    	{C5, notaDurMario(8)},
		    	{C5, notaDurMario(4)},
		    	{C5, notaDurMario(8)},
		    	{REST, notaDurMario(8)},
		    	{C5, notaDurMario(8)},
		    	{D5, notaDurMario(8)},
		    	{E5, notaDurMario(8)},
		    	{REST, notaDurMario(1)},
		    	{C5, notaDurMario(8)},
		    	{C5, notaDurMario(4)},
		    	{C5, notaDurMario(8)},
		    	{REST, notaDurMario(8)},
		    	{C5, notaDurMario(8)},
		    	{D5, notaDurMario(4)},
		    	{E5, notaDurMario(8)},
		    	{C5, notaDurMario(4)},
		    	{A4, notaDurMario(8)},
		    	{G4, notaDurMario(2)},
		    	{E5, notaDurMario(8)},
		    	{E5, notaDurMario(8)},
		    	{REST, notaDurMario(8)},
		    	{E5, notaDurMario(8)},
		    	{REST, notaDurMario(8)},
		    	{C5, notaDurMario(8)},
		    	{E5, notaDurMario(4)},
		    	{G5, notaDurMario(4)},
		    	{REST, notaDurMario(4)},
		    	{G4, notaDurMario(4)},
		    	{REST, notaDurMario(4)},
		    	{C5, notaDurMario(-4)},
		    	{G4, notaDurMario(8)},
		    	{REST, notaDurMario(4)},
		    	{E4, notaDurMario(-4)},
		    	{A4, notaDurMario(4)},
		    	{B4, notaDurMario(4)},
		    	{As4, notaDurMario(8)},
		    	{A4, notaDurMario(4)},
		    	{G4, notaDurMario(-8)},
		    	{E5, notaDurMario(-8)},
		    	{G5, notaDurMario(-8)},
		    	{A5, notaDurMario(4)},
		    	{F5, notaDurMario(8)},
		    	{G5, notaDurMario(8)},
		    	{REST, notaDurMario(8)},
		    	{E5, notaDurMario(4)},
		    	{C5, notaDurMario(8)},
		    	{D5, notaDurMario(8)},
		    	{B4, notaDurMario(-4)},
		    	{C5, notaDurMario(-4)},
		    	{G4, notaDurMario(8)},
		    	{REST, notaDurMario(4)},
		    	{E4, notaDurMario(-4)},
		    	{A4, notaDurMario(4)},
		    	{B4, notaDurMario(4)},
		    	{As4, notaDurMario(8)},
		    	{A4, notaDurMario(4)},
		    	{G4, notaDurMario(-8)},
		    	{E5, notaDurMario(-8)},
		    	{G5, notaDurMario(-8)},
		    	{A5, notaDurMario(4)},
		    	{F5, notaDurMario(8)},
		    	{G5, notaDurMario(8)},
		    	{REST, notaDurMario(8)},
		    	{E5, notaDurMario(4)},
		    	{C5, notaDurMario(8)},
		    	{D5, notaDurMario(8)},
		    	{B4, notaDurMario(-4)},
		    	{E5, notaDurMario(8)},
		    	{C5, notaDurMario(4)},
		    	{G4, notaDurMario(8)},
		    	{REST, notaDurMario(4)},
		    	{Gs4, notaDurMario(4)},
		    	{A4, notaDurMario(8)},
		    	{F5, notaDurMario(4)},
		    	{F5, notaDurMario(8)},
		    	{A4, notaDurMario(2)},
		    	{D5, notaDurMario(-8)},
		    	{A5, notaDurMario(-8)},
		    	{A5, notaDurMario(-8)},
		    	{A5, notaDurMario(-8)},
		    	{G5, notaDurMario(-8)},
		    	{F5, notaDurMario(-8)},
		    	{E5, notaDurMario(8)},
		    	{C5, notaDurMario(4)},
		    	{A4, notaDurMario(8)},
		    	{G4, notaDurMario(2)},
		    	{E5, notaDurMario(8)},
		    	{C5, notaDurMario(4)},
		    	{G4, notaDurMario(8)},
		    	{REST, notaDurMario(4)},
		    	{Gs4, notaDurMario(4)},
		    	{A4, notaDurMario(8)},
		    	{F5, notaDurMario(4)},
		    	{F5, notaDurMario(8)},
		    	{A4, notaDurMario(2)},
		    	{B4, notaDurMario(8)},
		    	{F5, notaDurMario(4)},
		    	{F5, notaDurMario(8)},
		    	{F5, notaDurMario(-8)},
		    	{E5, notaDurMario(-8)},
		    	{D5, notaDurMario(-8)},
		    	{C5, notaDurMario(8)},
		    	{E4, notaDurMario(4)},
		    	{E4, notaDurMario(8)},
		    	{C4, notaDurMario(2)},
		    	{E5, notaDurMario(8)},
		    	{C5, notaDurMario(4)},
		    	{G4, notaDurMario(8)},
		    	{REST, notaDurMario(4)},
		    	{Gs4, notaDurMario(4)},
		    	{A4, notaDurMario(8)},
		    	{F5, notaDurMario(4)},
		    	{F5, notaDurMario(8)},
		    	{A4, notaDurMario(2)},
		    	{D5, notaDurMario(-8)},
		    	{A5, notaDurMario(-8)},
		    	{A5, notaDurMario(-8)},
		    	{A5, notaDurMario(-8)},
		    	{G5, notaDurMario(-8)},
		    	{F5, notaDurMario(-8)},
		    	{E5, notaDurMario(8)},
		    	{C5, notaDurMario(4)},
		    	{A4, notaDurMario(8)},
		    	{G4, notaDurMario(2)},
		    	{E5, notaDurMario(8)},
		    	{C5, notaDurMario(4)},
		    	{G4, notaDurMario(8)},
		    	{REST, notaDurMario(4)},
		    	{Gs4, notaDurMario(4)},
		    	{A4, notaDurMario(8)},
		    	{F5, notaDurMario(4)},
		    	{F5, notaDurMario(8)},
		    	{A4, notaDurMario(2)},
		    	{B4, notaDurMario(8)},
		    	{F5, notaDurMario(4)},
		    	{F5, notaDurMario(8)},
		    	{F5, notaDurMario(-8)},
		    	{E5, notaDurMario(-8)},
		    	{D5, notaDurMario(-8)},
		    	{C5, notaDurMario(8)},
		    	{E4, notaDurMario(4)},
		    	{E4, notaDurMario(8)},
		    	{C4, notaDurMario(2)},
		    	{C5, notaDurMario(8)},
		    	{C5, notaDurMario(4)},
		    	{C5, notaDurMario(8)},
		    	{REST, notaDurMario(8)},
		    	{C5, notaDurMario(8)},
		    	{D5, notaDurMario(8)},
		    	{E5, notaDurMario(8)},
		    	{REST, notaDurMario(1)},
		    	{C5, notaDurMario(8)},
		    	{C5, notaDurMario(4)},
		    	{C5, notaDurMario(8)},
		    	{REST, notaDurMario(8)},
		    	{C5, notaDurMario(8)},
		    	{D5, notaDurMario(4)},
		    	{E5, notaDurMario(8)},
		    	{C5, notaDurMario(4)},
		    	{A4, notaDurMario(8)},
		    	{G4, notaDurMario(2)},
		    	{E5, notaDurMario(8)},
		    	{E5, notaDurMario(8)},
		    	{REST, notaDurMario(8)},
		    	{E5, notaDurMario(8)},
		    	{REST, notaDurMario(8)},
		    	{C5, notaDurMario(8)},
		    	{E5, notaDurMario(4)},
		    	{G5, notaDurMario(4)},
		    	{REST, notaDurMario(4)},
		    	{G4, notaDurMario(4)},
		    	{REST, notaDurMario(4)},
		    	{E5, notaDurMario(8)},
		    	{C5, notaDurMario(4)},
		    	{G4, notaDurMario(8)},
		    	{REST, notaDurMario(4)},
		    	{Gs4, notaDurMario(4)},
		    	{A4, notaDurMario(8)},
		    	{F5, notaDurMario(4)},
		    	{F5, notaDurMario(8)},
		    	{A4, notaDurMario(2)},
		    	{D5, notaDurMario(-8)},
		    	{A5, notaDurMario(-8)},
		    	{A5, notaDurMario(-8)},
		    	{A5, notaDurMario(-8)},
		    	{G5, notaDurMario(-8)},
		    	{F5, notaDurMario(-8)},
		    	{E5, notaDurMario(8)},
		    	{C5, notaDurMario(4)},
		    	{A4, notaDurMario(8)},
		    	{G4, notaDurMario(2)},
		    	{E5, notaDurMario(8)},
		    	{C5, notaDurMario(4)},
		    	{G4, notaDurMario(8)},
		    	{REST, notaDurMario(4)},
		    	{Gs4, notaDurMario(4)},
		    	{A4, notaDurMario(8)},
		    	{F5, notaDurMario(4)},
		    	{F5, notaDurMario(8)},
		    	{A4, notaDurMario(2)},
		    	{B4, notaDurMario(8)},
		    	{F5, notaDurMario(4)},
		    	{F5, notaDurMario(8)},
		    	{F5, notaDurMario(-8)},
		    	{E5, notaDurMario(-8)},
		    	{D5, notaDurMario(-8)},
		    	{C5, notaDurMario(8)},
		    	{E4, notaDurMario(4)},
		    	{E4, notaDurMario(8)},
		    	{C4, notaDurMario(2)},
		    	{C5, notaDurMario(-4)},
		    	{G4, notaDurMario(-4)},
		    	{E4, notaDurMario(4)},
		    	{A4, notaDurMario(-8)},
		    	{B4, notaDurMario(-8)},
		    	{A4, notaDurMario(-8)},
		    	{Gs4, notaDurMario(-8)},
		    	{As4, notaDurMario(-8)},
		    	{Gs4, notaDurMario(-8)},
		    	{G4, notaDurMario(8)},
		    	{D4, notaDurMario(8)},
		    	{E4, notaDurMario(-2)},
		    };
		    std::vector<Nota> melodiaSongOfStorms = {
		    	{D4, notaDur(4)},
		    	{A4, notaDur(4)},
		    	{A4, notaDur(4)},
		    	{REST, notaDur(8)},
		    	{E4, notaDur(8)},
		    	{B4, notaDur(2)},
		    	{F4, notaDur(4)},
		    	{C5, notaDur(4)},
		    	{C5, notaDur(4)},
		    	{REST, notaDur(8)},
		    	{E4, notaDur(8)},
		    	{B4, notaDur(2)},
		    	{D4, notaDur(4)},
		    	{A4, notaDur(4)},
		    	{A4, notaDur(4)},
		    	{REST, notaDur(8)},
		    	{E4, notaDur(8)},
		    	{B4, notaDur(2)},
		    	{F4, notaDur(4)},
		    	{C5, notaDur(4)},
		    	{C5, notaDur(4)},
		    	{REST, notaDur(8)},
		    	{E4, notaDur(8)},
		    	{B4, notaDur(2)},
		    	{D4, notaDur(8)},
		    	{F4, notaDur(8)},
		    	{D5, notaDur(2)},
		    	{D4, notaDur(8)},
		    	{F4, notaDur(8)},
		    	{D5, notaDur(2)},
		    	{E5, notaDur(-4)},
		    	{F5, notaDur(8)},
		    	{E5, notaDur(8)},
		    	{E5, notaDur(8)},
		    	{E5, notaDur(8)},
		    	{C5, notaDur(8)},
		    	{A4, notaDur(2)},
		    	{A4, notaDur(4)},
		    	{D4, notaDur(4)},
		    	{F4, notaDur(8)},
		    	{G4, notaDur(8)},
		    	{A4, notaDur(-2)},
		    	{A4, notaDur(4)},
		    	{D4, notaDur(4)},
		    	{F4, notaDur(8)},
		    	{G4, notaDur(8)},
		    	{E4, notaDur(-2)},
		    	{D4, notaDur(8)},
		    	{F4, notaDur(8)},
		    	{D5, notaDur(2)},
		    	{D4, notaDur(8)},
		    	{F4, notaDur(8)},
		    	{D5, notaDur(2)},
		    	{E5, notaDur(-4)},
		    	{F5, notaDur(8)},
		    	{E5, notaDur(8)},
		    	{E5, notaDur(8)},
		    	{E5, notaDur(8)},
		    	{C5, notaDur(8)},
		    	{A4, notaDur(2)},
		    	{A4, notaDur(4)},
		    	{D4, notaDur(4)},
		    	{F4, notaDur(8)},
		    	{G4, notaDur(8)},
		    	{A4, notaDur(2)},
		    	{A4, notaDur(4)},
		    	{D4, notaDur(1)},
		    };
		    //MelodiaProfessorLaytonIntro, MelodiaJigglypuffSong,melodiaSuperMarioBros

		    std::vector<std::vector<Nota>> todasLasMelodias = {melodiaSongOfStorms,melodiaLullaby, melodiaZeldaMainTheme};

		    // Baraja el orden de las melodías
		    std::random_device rd;
		    std::mt19937 g(rd());
		    std::shuffle(todasLasMelodias.begin(), todasLasMelodias.end(), g);

		    reproducirMelodias(todasLasMelodias, 0.15);
}




} // namespace BBB
