#include "SMSHelper.h"

std::string GetLastWord(std::string str)
{
	std::string word;

	for (UINT32 i = 0; i < str.size(); i++)
	{
		if (str[i] == ' ')
		{
			word.clear();
			i++;
		}
		if (str[i] != '\n')
			word += str[i];
	}

	return word;
}

SMSHelper::SMSHelper(const char *PyFile)
{
	// Open and read wa2smsProfile.txt to extract Username and Password to login with way2sms
	std::ifstream file("way2smsProfile.txt");
	std::string line, Username, Password;

	if (file.is_open() == false)
	{
		std::cout << "way2smsProfile.txt : Unable to open or does not exists" << std::endl;
	}

	while (getline(file, line))
	{
		if (line.find("Username") != std::string::npos)
		{
			Username = GetLastWord(line);
		}
		else if (line.find("Password") != std::string::npos)
		{
			Password = GetLastWord(line);
		}
		else
		{
			continue;
		}
	}

	// Initialize the Python interpreter.
	Py_Initialize();

	// Convert the file name to a Python string.
	pName = PyUnicode_FromString(PyFile);

	// Import the file as a Python module.
	pModule = PyImport_Import(pName);

	// Create a dictionary for the contents of the module.
	pDict = PyModule_GetDict(pModule);

	// Sign into way2sms with username and password
	CallFunction("SignIn", Username.c_str(), Password.c_str());
}

SMSHelper::~SMSHelper()
{
	// Sign out of way2sms
	CallFunction("SignOut");

	// Destroy the Python interpreter.
	Py_Finalize();
}

PyObject* SMSHelper::CallFunction(const char *FuncName, const char *Arguement1, const char *Arguement2)
{
	PyObject *pFunc, *pArgs, *pValue1, *pValue2;

	// Get the method from the dictionary.
	pFunc = PyDict_GetItemString(pDict, FuncName);

	if (Arguement1 != NULL && Arguement2 != NULL)
	{
		// Create a Python tuple to hold the arguments to the method.
		pArgs = PyTuple_New(2);

		// Convert Arguement1 and Arguement2 to a Python string and set as arguements before passing.
		pValue1 = PyUnicode_FromString(Arguement1);
		PyTuple_SetItem(pArgs, 0, pValue1);

		pValue2 = PyUnicode_FromString(Arguement2);
		PyTuple_SetItem(pArgs, 1, pValue2);
	}
	else
	{
		pArgs = NULL;
	}

	// Call the function with the arguments.
	PyObject* pResult = PyObject_CallObject(pFunc, pArgs);

	return pResult;
}