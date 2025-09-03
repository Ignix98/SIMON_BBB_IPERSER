/**
 *
 * PWMuniv.h
 * Author: Jose Simo (2019)
 * Universitat Politecnica de Valencia. AI2-DISCA
 * Creative Commons.
 *
 */
// Directiva de inclusión condicional: evita que este archivo se incluya más de una vez
#ifndef PWMUNIV_H_
#define PWMUNIV_H_

// Incluye la clase string de la STL
#include<string>
using std::string;

// Definiciones de macros para las rutas dentro del sistema de archivos sysfs de Linux
#define PWM_PERIOD "period"           // Archivo donde se escribe el período del PWM
#define PWM_DUTY "duty_cycle"         // Archivo donde se escribe el duty cycle
#define PWM_POLARITY "polarity"       // Archivo donde se define si el PWM es activo en alto o bajo
#define PWM_RUN "enable"              // Archivo para activar o desactivar la señal PWM

// Namespace personalizado para agrupar los recursos relacionados con la BeagleBone Black
namespace BBB {

// Clase que permite controlar un pin con salida PWM
class PWM {
public:
	// Enumerado para representar la polaridad de la señal PWM
	enum POLARITY { 
		ACTIVE_LOW = 0,  // Señal activa en bajo (0 lógico)
		ACTIVE_HIGH = 1  // Señal activa en alto (1 lógico)
	};

	// Enumerado con todos los pines disponibles para salida PWM
	enum pinPWM { 
		P9_22, P9_31,    // PWM0-A
		P9_21, P9_29,    // PWM0-B
		P9_42,           // ECAPPWM0
		P9_14, P8_36,    // PWM1-A
		P9_16, P8_34,    // PWM1-B
		P8_19, P8_45,    // PWM2-A
		P8_13, P8_46,    // PWM2-B
		P9_28            // ECAPPWM2
	};

private:
	// Atributos internos del objeto PWM
	string pinName;           // Nombre del pin físico, usado para config-pin
	string path;              // Ruta completa al directorio del PWM en sysfs
	string chipPath;          // Ruta base al chip del PWM

	float analogFrequency;    // Frecuencia por defecto para escribir analógicamente
	float analogMax;          // Máximo voltaje para analogWrite (por defecto 3.3V)

public:
	// Constructor: inicializa un PWM en el pin indicado
	PWM(pinPWM pinName_p);

	// Métodos públicos para controlar la señal PWM

	// Establecer y obtener el período (en nanosegundos)
	virtual int setPeriod(unsigned int period_ns);
	virtual unsigned int getPeriod();

	// Establecer y obtener la frecuencia (en Hz)
	virtual int setFrequency(float frequency_hz);
	virtual float getFrequency();

	// Establecer el duty cycle (en tiempo o porcentaje)
	virtual int setDutyCycle(unsigned int duration_ns);
	virtual int setDutyCycle(float percentage);

	// Obtener el duty cycle (en tiempo o porcentaje)
	virtual unsigned int getDutyCycle();
	virtual float getDutyCyclePercent();

	// Establecer o invertir la polaridad de la señal PWM
	virtual int setPolarity(PWM::POLARITY);
	virtual void invertPolarity();
	virtual POLARITY getPolarity();

	// Métodos para simulación analógica con PWM
	virtual void setAnalogFrequency(float frequency_hz) { 
		this->analogFrequency = frequency_hz; 
	}

	// Calibrar el voltaje máximo aceptado en analogWrite (entre 3.2 y 3.4 V)
	virtual int calibrateAnalogMax(float analogMax);

	// Escribir una salida analógica mediante PWM
	virtual int analogWrite(float voltage);

	// Activar, verificar estado y detener el PWM
	virtual int run();
	virtual bool isRunning();
	virtual int stop();

	// Destructor: libera el pin
	virtual ~PWM();

private:
	// Métodos auxiliares para convertir entre período y frecuencia
	float period_nsToFrequency(unsigned int);
	unsigned int frequencyToPeriod_ns(float);
};

} /* namespace BBB */

// Fin de la inclusión condicional
#endif /* PWMUNIV_H_ */