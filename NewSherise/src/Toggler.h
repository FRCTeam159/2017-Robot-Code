/*
 * Toggler.h
 *
 *  Created on: Nov 3, 2015
 *      Author: 159
 */

#ifndef TOGGLER_H_
#define TOGGLER_H_

class Toggler {
public:
	bool previousInput=false;
	bool previousOutput=false;
	Toggler();
	virtual ~Toggler();
	void Toggle(bool);
};

#endif /* TOGGLER_H_ */
