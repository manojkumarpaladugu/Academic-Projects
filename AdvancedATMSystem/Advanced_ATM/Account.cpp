#include "Account.h"
#include "SMSHelper.h"

Account::Account()
{
	file.open("BankCustomerDatabase.csv");

	if (file.is_open() == false)
	{
		std::cout << "BankCustomerDatabase.csv : Unable to open or does not exists" << std::endl;
	}

	pCustomer = new CustomerAccount();
}

Account::~Account()
{
	delete pCustomer;
	file.close();
}

void SplitCharsByDelim(std::string line, const char delim, std::vector<std::string> &elems)
{
	std::stringstream ss(line);
	std::string item;
	while (getline(ss, item, delim))
	{
		elems.push_back(item);
	}
}

bool Account::GetCustomer(std::string AccountNumber)
{
	file.clear();	// Clears the EOF bit or fail bit
	file.seekg(0);	// Sets file position indictor to beginning
	std::string line;
	std::vector<std::string> tokens;

	while (getline(file, line))
	{
		if (line.find(AccountNumber.c_str(),0,10) == std::string::npos)
			continue;
		
		SplitCharsByDelim(line, ',', tokens);

		pCustomer->Name = tokens[0];
		pCustomer->AccountNumber = tokens[1];
		pCustomer->ATMPin = tokens[2];
		pCustomer->Phone = tokens[3];
		pCustomer->Balance = tokens[4];

		return true;
	}
	return false;
}

bool Account::VerifyCustomer(std::string AccountNumber, std::string ATMPin)
{
	std::string GetOTP, OTP, Message = "Peoples Bank : High Security OTP for your transaction is ";
	SMSHelper *pSMS;

	if (AccountNumber == pCustomer->AccountNumber)
	{
		if (ATMPin == pCustomer->ATMPin)
		{
			// Use current time as seed for random generator
			srand(static_cast<unsigned int>(time(NULL)));

			OTP = std::to_string(1000 + (rand()%8999));
			Message += OTP;
			
			pSMS = new SMSHelper("SMSHelper");	//	open and use python script "SMSHelper.py"

			PyObject* result = pSMS->CallFunction("SendSMS", pCustomer->Phone.c_str(), Message.c_str());
			if (result == NULL)
			{
				std::cout << "SendSMS : Function call failed with " << result << std::endl;
				return false;
			}
			
			std::cout << "Enter OTP : ";
			std::cin >> GetOTP;

			if (GetOTP == OTP)
			{
				delete pSMS;
				return true;
			}

			std::cout << "Entered invalid OTP..!";
			delete pSMS;
			return false;
		}

		std::cout << "Entered invalid PIN...!" << std::endl;
		return false;
	}
	return false;
}

void Account::UpdateRecord()
{
	std::ofstream temp("temp.csv");
	std::string line;

	if (temp.is_open() == false)
	{
		std::cout << "temp.csv : Unable to open or does not exists" << std::endl;
	}

	file.clear();	// Clears the EOF bit or fail bit
	file.seekg(0);	// Sets file position indictor to beginning

	while (getline(file, line))
	{
		if (line.find(pCustomer->AccountNumber.c_str()) == std::string::npos)
		{
			temp << line << std::endl;
			continue;
		}

		temp << pCustomer->Name + ',' + pCustomer->AccountNumber + ',' + pCustomer->ATMPin + ',' + pCustomer->Phone + ',' + pCustomer->Balance << std::endl;
	}
	temp.close();
	file.close();

	if (remove("BankCustomerDatabase.csv") != 0)
		std::cout << "BankCustomerDatabase.csv : Remove operation failed" << std::endl;
	if (rename("temp.csv", "BankCustomerDatabase.csv") != 0)
		std::cout << "temp.csv : Rename operation failed" << std::endl;

	file.open("BankCustomerDatabase.csv");

	if (file.is_open() == false)
	{
		std::cout << "BankCustomerDatabase.csv : Unable to open or does not exists" << std::endl;
	}
}

void Account::EnquireBalance()
{
	system("cls");
	std::cout << "Account holder name : " << pCustomer->Name << std::endl;
	std::cout << "Account number : " << pCustomer->AccountNumber << std::endl;
	std::cout << "Available balance : " << pCustomer->Balance << std::endl;
	system("pause");
	Menu();
}

void Account::Withdraw()
{
	std::string Deduct, Message;
	SMSHelper *pSMS;

	system("cls");

	std::cout << "Enter the amount to deduct : ";
	std::cin >> Deduct;

	UINT32 Balance = atoi(pCustomer->Balance.c_str());
	UINT32 Amount = atoi(Deduct.c_str());

	if ((Balance > Amount) && ((Balance - Amount) >= 500))
	{
		Balance -= Amount;
		pCustomer->Balance = std::to_string(Balance);
		UpdateRecord();
		std::cout << "Please collect the cash..! ";

		pSMS = new SMSHelper("SMSHelper");	//	open and use python script "SMSHelper.py"

		Message = "Peoples Bank: " + Deduct + " is withdrawn from your Ac : " + pCustomer->AccountNumber;

		PyObject* result = pSMS->CallFunction("SendSMS", pCustomer->Phone.c_str(), Message.c_str());
		if (result == NULL)
		{
			std::cout << "SendSMS : Function call failed with " << result << std::endl;
			return;
		}

		system("pause");
		delete pSMS;
		Menu();
	}
	else
	{
		std::cout << "Insufficient funds..! ";
		system("pause");
		Menu();
	}
}

void Account::Deposit()
{
	std::string Deposit, Message;
	SMSHelper *pSMS;

	system("cls");

	std::cout << "Enter the amount to deposit : ";
	std::cin >> Deposit;

	UINT32 Balance = atoi(pCustomer->Balance.c_str());
	UINT32 Amount = atoi(Deposit.c_str());

	Balance += Amount;
	pCustomer->Balance = std::to_string(Balance);

	UpdateRecord();
	std::cout << "Amount is deposited..! ";

	pSMS = new SMSHelper("SMSHelper");	//	open and use python script "SMSHelper.py"

	Message = "Peoples Bank: " + Deposit + " is deposited in your Ac : " + pCustomer->AccountNumber;

	PyObject* result = pSMS->CallFunction("SendSMS", pCustomer->Phone.c_str(), Message.c_str());
	if (result == NULL)
	{
		std::cout << "SendSMS : Function call failed with " << result << std::endl;
		return;
	}

	system("pause");
	delete pSMS;
	Menu();
}

void Account::Menu()
{
	system("cls");

	std::cout << "1. Enquire Balance" << std::endl;
	std::cout << "2. Withdraw" << std::endl;
	std::cout << "3. Deposit" << std::endl;
	std::cout << "4. Cancel" << std::endl;

	char choice;
	fflush(stdin);
	std::cout << "Enter you choice : " ;
	std::cin >> choice;

	switch (choice)
	{
		case '1' : EnquireBalance(); break;
		case '2' : Withdraw(); break;
		case '3' : Deposit(); break;
		case '4': return;
	}
}