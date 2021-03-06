#include "SMSHelper.h"
#include "Account.h"

std::string ATMState;

void StartATM()
{
	ATMState = "ON";

	while (ATMState == "ON")
	{
		Account *pAcc = new Account();

		std::string AccountNumber;
		std::string ATMPin;

		system("cls");
		fflush(stdin);

		std::cout << std::endl << "Hi... Welcome to Peoples Bank ATM!" << std::endl << std::endl;

		std::cout << "Enter Accout Number : ";
		std::cin >> AccountNumber;
		
		std::cout << "Enter ATM PIN : ";
		std::cin >> ATMPin;

		bool result1 = pAcc->GetCustomer(AccountNumber);
		if (result1 != true)
		{
			std::cout << "Entered invalid account number " << AccountNumber << std::endl;
			system("pause");
			delete pAcc;
			continue;
		}

		bool result2 = pAcc->VerifyCustomer(AccountNumber, ATMPin);
		if (result2 != true)
		{
			std::cout << "Failed to verify customer...!, Try again" << std::endl;
			system("pause");
			delete pAcc;
			continue;
		}

		system("cls");
		pAcc->Menu();

		delete pAcc;
	}
}

int main(int argc, char *argv[])
{
	StartATM();
}