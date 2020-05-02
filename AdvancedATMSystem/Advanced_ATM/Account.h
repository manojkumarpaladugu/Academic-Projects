#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <vector>
#include <sstream>
#include <stdio.h>

typedef unsigned int UINT32;
typedef signed int INT32;

class Account {
private:
	struct CustomerAccount
	{
		std::string Name;
		std::string AccountNumber;
		std::string ATMPin;
		std::string Phone;
		std::string Balance;
	};
	CustomerAccount *pCustomer = NULL;
	std::ifstream file;

public:
	Account();
	~Account();
	bool GetCustomer(std::string AccountNumber);
	bool VerifyCustomer(std::string AccountNumber, std::string ATMPin);
	void EnquireBalance();
	void Withdraw();
	void Deposit();
	void UpdateRecord(void);
	void Menu();
};