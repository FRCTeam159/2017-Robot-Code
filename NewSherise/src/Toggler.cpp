/*
 * Toggler.cpp
 *
 *  Created on: Nov 3, 2015
 *      Author: 159
 */

#include <Toggler.h>

Toggler::Toggler() {
	// TODO Auto-generated constructor stub

}

Toggler::~Toggler() {
	// TODO Auto-generated destructor stub
}

void Toggler::Toggle(bool input){
	bool output;
	if(!previousInput&&input){
		output=!previousOutput;
	}
	else
	{
		output=previousOutput;
	}
	previousOutput=output;
	previousInput=input;
}
