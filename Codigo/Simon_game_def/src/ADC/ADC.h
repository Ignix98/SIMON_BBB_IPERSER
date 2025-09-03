/*
 * ADC.h
 *
 * Author: Jose Simo (2016..2022)
 * Universitat Politecnica de Valencia. AI2-DISCA
 * Creative Commons.
 */

#ifndef SRC_ADC_ADC_H_
#define SRC_ADC_ADC_H_

#include <fstream>

using namespace std;

namespace BBB {


class ADC {
	private:
		fstream adc_channelFS;
	public:
		ADC(int number);
		virtual ~ADC();
		int readAnalog();
	};

}  //namespace BBB
#endif /* SRC_ADC_ADC_H_ */
