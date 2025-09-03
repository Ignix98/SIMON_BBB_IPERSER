/*
 * ADC.cpp
 *
 * Author: Jose Simo (2016..2022)
 * Universitat Politecnica de Valencia. AI2-DISCA
 * Creative Commons.
 */

#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "ADC.h"

using namespace std;

namespace BBB {

	ADC::ADC(int number) {
		//the sysfs base name path for ADC is "/sys/bus/iio/devices/iio:device0/in_voltage"
		stringstream ss;
		ss << "/sys/bus/iio/devices/iio:device0/in_voltage" << number << "_raw";
		adc_channelFS.open(ss.str().c_str(), fstream::in);
	}

	ADC::~ADC() {
		adc_channelFS.close();
	}

	/**
	 *
	 */
	int ADC::readAnalog() {
		int sample;
		adc_channelFS >> sample;
		adc_channelFS.seekg(0);
		return sample;
	}

} //namespace BBB
