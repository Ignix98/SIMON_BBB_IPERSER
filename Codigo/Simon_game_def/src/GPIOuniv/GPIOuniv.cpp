 /**
 *
 * GPIOuniv.cpp
 * Author: Jose Simo (2022)
 * Universitat Politecnica de Valencia. AI2-DISCA
 * Creative Commons.
 *
 */

#include "GPIOuniv.h"
#include "util.h"
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<cstdlib>
#include<cstdio>
#include<fcntl.h>
#include<unistd.h>
#include<sys/epoll.h>
#include<pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cerrno>
#include <clocale>
#include <cstring>

using namespace std;

namespace BBB {
/**
 * The constructor will set up the states and export the pin.
 * @param number The GPIO number to be exported
 */

GPIOuniv::GPIOuniv(pinGPIO pinName_p) {
	this->debounceTime = 0;
	this->togglePeriod=100;
	this->toggleNumber=-1; //infinite number
	this->callbackFunction = NULL;
	this->thread = (long unsigned int)NULL;
	this->threadRunning = false;
	this->lastEventTime = getCurrentMicroseconds();;

	pin = pinName_p;
	number = pinGPIONumber.at(pinName_p);
	pinName = pinGPIOName.at(pinName_p);
	cout << "Configurando pin "<< pinName <<" ("<< number << ") ..."<< endl;

	ostringstream s;
	s << "gpio" << number;
	this->name = string(s.str());
	this->path = GPIO_PATH + this->name + "/";
	//Exportar el pin como GPIO
	string command_pin = "/usr/bin/config-pin " + pinName + " gpio";
	cout << command_pin << endl;
	system(command_pin.c_str());
	usleep(100000);
	//Comprobar que el pin está disponible
	struct stat statbuf;
	const char * check_path;
	check_path = this->path.c_str();
	if (stat(check_path, &statbuf) != -1) {
	   if (S_ISDIR(statbuf.st_mode)) {
			cout << "Pin " << pinName <<" ("<< number <<") configurado como GPIO " << endl;
			cout << endl;
	   }
	} else {
			string msg = strerror(errno);
			cout << "Error: "<< msg << endl;
			cout << "Pin " << pinName <<" ("<< number <<") no disponible como GPIO. " << endl;
			cout << "Es necesario cargar el overlay correcto para usar este pin. " << endl;
			cout << endl;
			exit(1);
	}
}

/**
 * Private method to export the GPIO
 * @return int that describes if the operation fails
 */

int GPIOuniv::exportGPIO(){
   return write(GPIO_PATH, "export", this->number);
}

int GPIOuniv::unexportGPIO(){
   return write(GPIO_PATH, "unexport", this->number);
}

int GPIOuniv::setDirection(GPIOuniv::DIRECTION dir){
   switch(dir){
   case INPUT: return write(this->path, "direction", "in");
      break;
   case OUTPUT:return write(this->path, "direction", "out");
      break;
   }
   return -1;
}

int GPIOuniv::setValue(GPIOuniv::VALUE value){
   switch(value){
   case HIGH: return write(this->path, "value", "1");
      break;
   case LOW: return write(this->path, "value", "0");
      break;
   }
   return -1;
}

int GPIOuniv::setEdgeType(GPIOuniv::EDGE value){
   switch(value){
   case NONE: return write(this->path, "edge", "none");
      break;
   case RISING: return write(this->path, "edge", "rising");
      break;
   case FALLING: return write(this->path, "edge", "falling");
      break;
   case BOTH: return write(this->path, "edge", "both");
      break;
   }
   return -1;
}

int GPIOuniv::setActiveLow(bool isLow){
   if(isLow) return write(this->path, "active_low", "1");
   else return write(this->path, "active_low", "0");
}

int GPIOuniv::setActiveHigh(){
   return this->setActiveLow(false);
}

GPIOuniv::VALUE GPIOuniv::getValue(){
	string input = read(this->path, "value");
	if (input == "0") return LOW;
	else return HIGH;
}

GPIOuniv::DIRECTION GPIOuniv::getDirection(){
	string input = read(this->path, "direction");
	if (input == "in") return INPUT;
	else return OUTPUT;
}

GPIOuniv::EDGE GPIOuniv::getEdgeType(){
	string input = read(this->path, "edge");
	if (input == "rising") return RISING;
	else if (input == "falling") return FALLING;
	else if (input == "both") return BOTH;
	else return NONE;
}

int GPIOuniv::streamOpen(){
	stream.open((path + "value").c_str());
	return 0;
}
int GPIOuniv::streamWrite(GPIOuniv::VALUE value){
	stream << value << std::flush;
	return 0;
}
int GPIOuniv::streamClose(){
	stream.close();
	return 0;
}

int GPIOuniv::toggleOutput(){
	this->setDirection(OUTPUT);
	if ((bool) this->getValue()) this->setValue(LOW);
	else this->setValue(HIGH);
    return 0;
}

int GPIOuniv::toggleOutput(int time){ return this->toggleOutput(-1, time); }
int GPIOuniv::toggleOutput(int numberOfTimes, int time){
	this->setDirection(OUTPUT);
	this->toggleNumber = numberOfTimes;
	this->togglePeriod = time;
	this->threadRunning = true;
    if(pthread_create(&this->thread, NULL, &threadedToggle, static_cast<void*>(this))){
    	perror("GPIO: Failed to create the toggle thread");
    	this->threadRunning = false;
    	return -1;
    }
    return 0;
}

// This thread function is a friend function of the class
void* threadedToggle(void *value){
	GPIOuniv *gpio = static_cast<GPIOuniv*>(value);
	bool isHigh = (bool) gpio->getValue(); //find current value
	while(gpio->threadRunning){
		if (isHigh)	gpio->setValue(GPIOuniv::HIGH);
		else gpio->setValue(GPIOuniv::LOW);
		usleep(gpio->togglePeriod * 500);
		isHigh=!isHigh;
		if(gpio->toggleNumber>0) gpio->toggleNumber--;
		if(gpio->toggleNumber==0) gpio->threadRunning=false;
	}
	return 0;
}

// Blocking Poll - based on the epoll socket code in the epoll man page

int GPIOuniv::waitForEdge(){
	this->setDirection(INPUT); // must be an input pin to poll its value
	int fd, epollfd;
	//int count=0;
	struct epoll_event ev;
	epollfd = epoll_create1(0);
    if (epollfd == -1) {
	   perror("GPIO: Failed to create epollfd");
	   return -1;
    }
    if ((fd = open((this->path + "value").c_str(), O_RDONLY | O_NONBLOCK)) == -1) {
       perror("GPIO: Failed to open file");
       return -1;
    }

    //ev.events = read operation | edge triggered | urgent data
    ev.events = EPOLLIN | EPOLLET | EPOLLPRI;
    ev.data.fd = fd;  // attach the file file descriptor

    //Register the file descriptor on the epoll instance, see: man epoll_ctl
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
       perror("GPIO: Failed to add control interface");
       return -1;
    }
    //Wait for event
    long currentTime;
    do {
    	int retval, i;
    	for (i=0; i<2; i++) { //Discard the first event
    		retval = epoll_wait(epollfd, &ev, 1, -1);
    	};
    	if ( retval == -1) {
    		perror("GPIO: Poll Wait fail");
    	    close(fd);
    	    close(epollfd);
    		return -1;
    	}
    	currentTime = getCurrentMicroseconds();
    } while(currentTime - lastEventTime < this->debounceTime * 1000);
    lastEventTime = currentTime;
    close(fd);
    close(epollfd);
	return 0;
}

// This thread function is a friend function of the class
void* threadedPoll(void *value){
	GPIOuniv *gpio = static_cast<GPIOuniv*>(value);
	while(gpio->threadRunning){
		gpio->waitForEdge();
		gpio->callbackFunction(gpio->number);
	}
	return 0;
}

int GPIOuniv::waitForEdge(CallbackType callback){
	this->threadRunning = true;
	this->callbackFunction = callback;
    // create the thread, pass the reference, address of the function and data
    if(pthread_create(&this->thread, NULL, &threadedPoll, static_cast<void*>(this))){
    	perror("GPIOuniv: Failed to create the poll thread");
    	this->threadRunning = false;
    	return -1;
    }
    return 0;
}


GPIOuniv::~GPIOuniv() {
	//this->unexportGPIO();
}

} /* namespace BBB */
