/*
 * BusDevice.cpp  Created on: 23 May 2014
 * Copyright (c) 2014 Derek Molloy (www.derekmolloy.ie)
 * Made available for the book "Exploring Raspberry Pi"
 */

#include "device/bus/BusDevice.h"

namespace exploringBB {

BusDevice::BusDevice(unsigned int bus, unsigned int device) {
	this->bus = bus;
	this->device = device;
	this->file=-1;
}

BusDevice::~BusDevice() {}

} /* namespace exploringBB */
