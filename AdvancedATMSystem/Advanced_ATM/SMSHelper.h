#pragma once
#include <Python.h>
#include <iostream>
#include <fstream>
#include <sstream>

typedef unsigned int UINT32;
typedef signed int INT32;

class SMSHelper {
private:
	// Create some Python objects that will later be assigned values.
	PyObject *pName, *pModule, *pDict;

public:
	SMSHelper(const char *PyFile);
	~SMSHelper();

	PyObject* CallFunction(const char *FuncName, const char *Arguement1 = NULL, const char *Arguement2 = NULL);
};