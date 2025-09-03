/**
 *
 * PWMuniv.cpp
 * Author: Jose Simo (2024)
 * Universitat Politecnica de Valencia. AI2-DISCA
 * Creative Commons.
 *
 */

// Inclusión de cabeceras personalizadas
#include "PWMuniv.h"  // Cabecera donde se define la clase PWM y enums como pinPWM, POLARITY, etc.
#include "util.h"     // Cabecera con funciones auxiliares para leer/escribir archivos en sysfs

// Inclusión de librerías estándar
#include <iostream>
#include <cstdlib>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

// Uso del espacio de nombres estándar
using namespace std;

// Namespace que agrupa las funcionalidades relacionadas con la BeagleBone Black
namespace BBB {

// Constructor de la clase PWM: inicializa el PWM en el pin indicado
PWM::PWM(pinPWM pinName_p) {
	this->analogFrequency = 100000;  // Frecuencia por defecto para simulación analógica
	this->analogMax = 3.3;           // Voltaje máximo esperado (por defecto 3.3V)

	// Asignación de nombre del pin físico y ruta en el sistema de archivos según el pin seleccionado
	switch (pinName_p) {
	  	case P9_22:
	  		pinName = "P9.22";
	  		chipPath = "/sys/class/pwm/pwmchip3/pwm0";
	  		break;
	  	case P9_31:
	  		pinName = "P9.31";
	  		chipPath = "/sys/class/pwm/pwmchip3/pwm0";
	  		break;
	  	case P9_21:
	  		pinName = "P9.21";
	  		chipPath = "/sys/class/pwm/pwmchip3/pwm1";
	  		break;
	  	case P9_29:
	  		pinName = "P9.29";
	  		chipPath = "/sys/class/pwm/pwmchip3/pwm1";
	  		break;
	  	case P9_42:
	  		pinName = "P9.42";
	  		chipPath = "/sys/class/pwm/pwmchip0/pwm0";
	  		break;
	  	case P9_14:
	  		pinName = "P9.14";
	  		chipPath = "/sys/class/pwm/pwmchip5/pwm0";
	  		break;
	  	case P8_36:
	  		pinName = "P8.36";
	  		chipPath = "/sys/class/pwm/pwmchip5/pwm0";
	  		break;
	  	case P9_16:
	  		pinName = "P9.16";
	  		chipPath = "/sys/class/pwm/pwmchip5/pwm1";
			break;
	  	case P8_34:
	  		pinName = "P8.34";
	  		chipPath = "/sys/class/pwm/pwmchip5/pwm1";
			break;
	  	case P8_19:
	  		pinName = "P8.19";
	  		chipPath = "/sys/class/pwm/pwmchip7/pwm0";
			break;
		case P8_45:
			pinName = "P8.45";
	  		chipPath = "/sys/class/pwm/pwmchip7/pwm0";
			break;
		case P8_13:
			pinName = "P8.13";
	  		chipPath = "/sys/class/pwm/pwmchip7/pwm1";
			break;
		case P8_46:
			pinName = "P8.46";
	  		chipPath = "/sys/class/pwm/pwmchip7/pwm1";
			break;
		case P9_28:
			pinName = "P9.28";
	  		chipPath = "/sys/class/pwm/pwmchip2/pwm0";
			break;
	}

	// Se construye la ruta completa al directorio de control del pin
	path = chipPath + "/";

	// Se configura el pin para que funcione como PWM mediante el comando `config-pin`
	string command_pin = "/usr/bin/config-pin " + pinName + " pwm";
	cout << command_pin << endl; // Se muestra el comando por consola
	system(command_pin.c_str()); // Se ejecuta el comando
	usleep(100000);              // Espera 0.1 segundos para asegurar que se aplique la configuración
}

// Destructor: detiene el PWM y libera el pin
PWM::~PWM() {
	stop(); // Detiene la señal PWM
	string command_freepin = "/usr/bin/config-pin " + pinName + " gpio"; // Devuelve el pin a modo GPIO
	cout << command_freepin << endl;
	system(command_freepin.c_str());
	usleep(100000); // Espera tras liberar el pin
}

// Establece el período de la señal PWM (en nanosegundos)
int PWM::setPeriod(unsigned int period_ns){
	return write(this->path, PWM_PERIOD, period_ns);
}

// Devuelve el período actual del PWM
unsigned int PWM::getPeriod(){
	return atoi(read(this->path, PWM_PERIOD).c_str());
}

// Convierte un período en nanosegundos a frecuencia (Hz)
float PWM::period_nsToFrequency(unsigned int period_ns){
	float period_s = (float)period_ns / 1e9;
	return 1.0f / period_s;
}

// Convierte una frecuencia en Hz a período en nanosegundos
unsigned int PWM::frequencyToPeriod_ns(float frequency_hz){
	float period_s = 1.0f / frequency_hz;
	return (unsigned int)(period_s * 1e9);
}

// Establece la frecuencia (Hz) del PWM
int PWM::setFrequency(float frequency_hz){
	return this->setPeriod(this->frequencyToPeriod_ns(frequency_hz));
}

// Devuelve la frecuencia (Hz) actual del PWM
float PWM::getFrequency(){
	return this->period_nsToFrequency(this->getPeriod());
}

// Establece el duty cycle en nanosegundos
int PWM::setDutyCycle(unsigned int duty_ns){
	return write(this->path, PWM_DUTY, duty_ns);
}

// Establece el duty cycle como porcentaje del período
int PWM::setDutyCycle(float percentage){
	if ((percentage > 100.0f) || (percentage < 0.0f)) return -1;
	unsigned int period_ns = this->getPeriod();
	float duty_ns = period_ns * (percentage / 100.0f);
	this->setDutyCycle((unsigned int)duty_ns);
	return 0;
}

// Devuelve el duty cycle actual en nanosegundos
unsigned int PWM::getDutyCycle(){
	return atoi(read(this->path, PWM_DUTY).c_str());
}

// Devuelve el duty cycle como porcentaje del período
float PWM::getDutyCyclePercent(){
	unsigned int period_ns = this->getPeriod();
	unsigned int duty_ns = this->getDutyCycle();
	return 100.0f * (float)duty_ns / (float)period_ns;
}

// Establece la polaridad del PWM (activo en alto o en bajo)
int PWM::setPolarity(PWM::POLARITY polarity){
	return write(this->path, PWM_POLARITY, polarity);
}

// Invierte la polaridad del PWM
void PWM::invertPolarity(){
	if (this->getPolarity() == PWM::ACTIVE_LOW)
		this->setPolarity(PWM::ACTIVE_HIGH);
	else
		this->setPolarity(PWM::ACTIVE_LOW);
}

// Devuelve la polaridad actual
PWM::POLARITY PWM::getPolarity(){
	if (atoi(read(this->path, PWM_POLARITY).c_str()) == 0)
		return PWM::ACTIVE_LOW;
	else
		return PWM::ACTIVE_HIGH;
}

// Calibra el voltaje máximo esperado para analogWrite (debe estar entre 3.2V y 3.4V)
int PWM::calibrateAnalogMax(float analogMax){
	if ((analogMax < 3.2f) || (analogMax > 3.4f)) return -1;
	else this->analogMax = analogMax;
	return 0;
}

// Simula una salida analógica escribiendo un voltaje usando PWM
int PWM::analogWrite(float voltage){
	if ((voltage < 0.0f) || (voltage > 3.3f)) return -1;
	this->setFrequency(this->analogFrequency);
	this->setPolarity(PWM::ACTIVE_LOW);
	this->setDutyCycle((100.0f * voltage) / this->analogMax);
	return this->run();
}

// Activa el PWM (empieza a generar la señal)
int PWM::run(){
	return write(this->path, PWM_RUN, 1);
}

// Verifica si el PWM está activo
bool PWM::isRunning(){
	string running = read(this->path, PWM_RUN);
	return (running == "1");
}

// Detiene la señal PWM
int PWM::stop(){
	return write(this->path, PWM_RUN, 0);
}

} // Fin del namespace BBB

