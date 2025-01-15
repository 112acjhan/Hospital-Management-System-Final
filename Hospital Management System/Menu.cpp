#include <iostream>
#include <conio.h> // _getch(), KEY...
#include "Menu.h"
#include <iterator>
#include <cctype> //isdigit
#include <cstring> //.c_str()
#include <sstream> //Used stringstream
#include <iomanip> //Used for setprecision
#include <cmath> //Used for abs
#include <regex>
#include <ctime>
#include <pdflib.hpp>
#include <codecvt> // wstring convertor

using namespace std;
using std::cout;
using namespace pdflib;

/**********************************************************************************************************************/
//User defined function
namespace eraseOperations
{
	void Erase_Elements(char input[], int& currentIndex)
	{
		if (currentIndex > 0)
		{
			currentIndex--;
			input[currentIndex] = '\0';
		}
	}

	void Erase_EncryptedElements(char input[], int currentIndex)
	{
		if (currentIndex > 0)
		{
			currentIndex--;
			input[currentIndex] = '\0';
		}
	}

	void Erase_Lines(const int start, const int end)
	{
		if (start < end)
		{
			for (int i = start; i <= end; ++i)
			{
				cout << "\x1b[2K"; // Delete current line
				if (i < end)
				{
					cout << "\x1b[A"; // Move up
				}
			}
		}

	}

}

using namespace eraseOperations;
using eraseOperations::Erase_Lines;

namespace inputOperations
{
	void Input(const int size, int& currentIndex, char input[], char keyIn)
	{
		if (currentIndex < size - 1)
		{
			input[currentIndex] = keyIn;
			currentIndex++;
		}
	}

	void InputEncrypted(const int size, int currentIndex, char input[], char keyIn)
	{
		if (currentIndex < size - 1)
		{
			input[currentIndex] = keyIn;
			currentIndex++;
		}
	}

	void Selected_Input(bool& exit, const int size, char input[])
	{
		int currentIndex = strlen(input);

		char keyIn = _getch();

		if (keyIn == 0 || keyIn == -32) // Check for special keys (like arrow keys)
		{
			keyIn = _getch(); // Get the actual key code
		}
		else if (keyIn == KEY_DELETE)
		{
			Erase_Elements(input, currentIndex);

		}
		else if (keyIn == KEY_ENTER)
		{
			exit = true;
		}
		else
		{
			Input(size, currentIndex, input, keyIn);
		}
	}
	//Overload function
	void Selected_Input(bool& exit, const int size, char input[], float& salary)
	{
		int currentIndex = strlen(input);

		char keyIn = _getch();

		if (keyIn == 0 || keyIn == -32) // Check for special keys (like arrow keys)
		{
			keyIn = _getch(); // Get the actual key code
		}
		else if (keyIn == KEY_DELETE)
		{
			Erase_Elements(input, currentIndex);
			salary = atof(input);
		}
		else if (keyIn == KEY_ENTER)
		{
			exit = true;
		}
		else
		{
			if (isdigit(keyIn) || keyIn == '.') //Make sure the user input digit instead of character
			{
				Input(size, currentIndex, input, keyIn);
				salary = atof(input);
			}
		}
	}
	
	void Selected_Input(bool& exit, const int size, char input[], int& salary)
	{
		int currentIndex = strlen(input);

		char keyIn = _getch();

		if (keyIn == 0 || keyIn == -32) // Check for special keys (like arrow keys)
		{
			keyIn = _getch(); // Get the actual key code
		}
		else if (keyIn == KEY_DELETE)
		{
			Erase_Elements(input, currentIndex);
			salary = atof(input);
		}
		else if (keyIn == KEY_ENTER)
		{
			exit = true;
		}
		else
		{
			if (isdigit(keyIn)) //Make sure the user input digit instead of character
			{
				Input(size, currentIndex, input, keyIn);
				salary = atof(input);
			}
		}
	}

	void Available_Input(bool& exit, string input_option[], const int size_option, int& selected_option, char input[], const int input_size)
	{
		string* showed_option = new string[size_option];
		for (int i = 0; i < size_option; i++)
		{
			showed_option[i] = "  < " + input_option[i] + " >  ";
		}
		char keyIn = _getch();

		if (keyIn == 0 || keyIn == -32) // Check for special keys (arrow keys)
		{
			keyIn = _getch(); // Get the actual key code
			if (keyIn == KEY_LEFT)
			{
				if (selected_option > 0)
				{
					selected_option--;
				}
				else
				{
					selected_option = size_option - 1;
				}
				strcpy_s(input, input_size, showed_option[selected_option].c_str());
			}
			else if (keyIn == KEY_RIGHT)
			{
				if (selected_option < size_option - 1)
				{
					selected_option++;
				}
				else
				{
					selected_option = 0;
				}
				strcpy_s(input, input_size, showed_option[selected_option].c_str());
			}
		}
		else if (keyIn == KEY_ENTER)
		{
			strcpy_s(input, input_size, input_option[selected_option].c_str());
			exit = true;
		}
		delete[]showed_option;
	}

}

using namespace inputOperations;

int Get_RowCount(string query)
{
	Sql_DB db;
	int row_count = 0;
	db.PrepareStatement(query);
	db.QueryResult();

	if (db.result != nullptr) //Avoid nullptr
	{
		while (db.result->next())
		{
			row_count++;
		}
		return row_count;
	}
	else
	{
		return row_count;
	}
}

string blank(int default_space, int input)
{
	string space;
	string convert = to_string(input);
	int used_space = convert.length();
	for (int i = 0; i < default_space - used_space; i++)
	{
		space = space + " ";
	}
	return space;
}

string blank(int default_space, string input)
{
	string space;
	int used_space = input.length();
	for (int i = 0; i < default_space - used_space; i++)
	{
		space = space + " ";
	}
	return space;
}

void printTimeSlots(const std::string& startTimeStr, const std::string& endTimeStr) {
	struct Time {
		int hour;
		int minute;
	};

	auto parseTime = [](const std::string& timeStr) -> Time {
		int hour, minute;
		char colon;
		std::istringstream timeStream(timeStr);
		timeStream >> hour >> colon >> minute;
		return { hour, minute };
		};

	auto timeToMinutes = [](const Time& time) -> int {
		return time.hour * 60 + time.minute;
		};

	Time startTime = parseTime(startTimeStr);
	Time endTime = parseTime(endTimeStr);

	int startMinutes = timeToMinutes(startTime);
	int endMinutes = timeToMinutes(endTime);

	// Handle the case where end time is on the next day
	if (endMinutes < startMinutes) {
		endMinutes += 24 * 60;  // Add 24 hours
	}

	int slot = 0;
	string q1 = "[slot ";
	string q2 = "]";

	std::cout << "Time slots:" << std::endl;
	for (int currentMinutes = startMinutes; currentMinutes <= endMinutes; currentMinutes += 30) {
		int currentHour = (currentMinutes / 60) % 24;
		int currentMinute = currentMinutes % 60;
		// Corrected printing logic with proper formatting and alignment
		std::cout << "     " << q1 + std::to_string(slot) + q2 << "     "
			<< std::setfill('0') << std::setw(2) << currentHour << ":" 
			<< std::setfill('0') << std::setw(2) << currentMinute << std::endl;
		slot++;
	}
}
/*====================================================================================================================*/



/**********************************************************************************************************************/
//Home Menu function
Menu::Menu() {}

Menu::~Menu() {}

void Menu::Registration_Option_Menu(int option, string selected, string not_selected, Account& reg)
{
	Erase_Lines(0, 21);
	selected = selected + ">";

	cout << (option == 0 ? selected : not_selected) << "                  Name            : " << reg.Name << RESET << "\n"
		<< (option == 1 ? selected : not_selected) << "                  DOB(YYYY-MM-DD) : " << reg.DOB << RESET << "\n"
		<< (option == 2 ? selected : not_selected) << "                  Gender(M,F)     : " << reg.Gender << RESET << "\n"
		<< (option == 3 ? selected : not_selected) << "                  Race            : " << reg.Race << RESET << "\n"
		<< (option == 4 ? selected : not_selected) << "                  Religion        : " << reg.Religion << RESET << "\n"
		<< (option == 5 ? selected : not_selected) << "                  Address         : " << reg.Address << RESET << "\n"
		<< (option == 6 ? selected : not_selected) << "                  IC              : " << reg.IC << RESET << "\n"
		<< (option == 7 ? selected : not_selected) << "                  Salary(RM)      : " << reg.Salary << RESET << "\n"
		<< (option == 8 ? selected : not_selected) << "                  Role            : " << reg.Role << RESET << "\n"
		<< (option == 9 ? selected : not_selected) << "                  Contact Number  : " << reg.Contact_Number << RESET << "\n"
		<< (option == 10 ? selected : not_selected) << "                  Email           : " << reg.Email << RESET << "\n"
		<< (option == 11 ? selected : not_selected) << "                  Password        : " << reg.Password << RESET << "\n"
		<< (option == 12 ? selected : not_selected) << "                  Manage By*      : " << reg.Manage_By << RESET << "\n\n\n\n"
		<< (option == 13 ? selected + ">" : not_selected) << "                  Register" << RESET << endl
		<< "\n\n\n" << CYAN_BG
		<< "                                                                                                                 " << RESET << endl;
}

int Menu::Home_Menu()
{
	int option = 0;
	bool selecting = true;
	string selected = BWHITE_TEXT + BOLD + ">";
	string not_selected = GRAY_TEXT;

	cout << WHITE_BG << RED_TEXT << RAPID_BLINK << BOLD
		<< "                                                                                                                 " << endl
		<< "                                             Welcome To HMS                                                      " << endl
		<< "                                                                                                                 " << endl
		<< RESET << endl; //3

	do
	{
		cout << "\n\n\n\n\n\n" //9
			<< (option == 0 ? selected : not_selected) << "                          Login          " << RESET << "\n\n"
			<< (option == 1 ? selected : not_selected) << "                          Registration   " << RESET << "\n\n"
			<< (option == 2 ? selected : not_selected) << "                          Exit           " << RESET << endl
			<< "\n\n\n\n\n\n" << WHITE_BG //20
			<< "                                                                                                                 " << endl
			<< RESET << endl; //22

		switch (_getch())
		{
		case KEY_UP:
			if (option > 0)
			{
				option--;
			}
			else
			{
				option = 2;
			}
			break;
		case KEY_DOWN:
			if (option < 2)
			{
				option++;
			}
			else
			{
				option = 0;
			}
			break;
		case KEY_ENTER:
			selecting = false;
			break;
		}

		Erase_Lines(0, 19);
	} while (selecting);

	system("cls");

	return option;
}

bool Menu::Login_Menu(Account& usedAcc)
{
	Account acc;
	Sql_DB db;
	int size = 0, option = 0, optionStart = 0, optionEnd = 2;
	int currentPasswordIndex = strlen(acc.Password);
	bool selecting = true;
	char encrypted[13] = {};
	string selected = BWHITE_TEXT + BOLD;
	string not_selected = GRAY_TEXT;
	string message;

	cout << CYAN_BG << YELLOW_TEXT << RAPID_BLINK << BOLD
		<< "                                                                                                                 " << endl
		<< "                                                    LOGIN                                                        " << endl
		<< "                                                                                                                 " << endl
		<< RESET << endl; //3

	do
	{
		cout << "\n\n\n\n\n\n" //9
			<< (option == 0 ? selected : not_selected) << "                          ID      : " << acc.IC << RESET << "\n\n"
			<< (option == 1 ? selected : not_selected) << "                          Password: " << encrypted << RESET << "\n\n\n\n"
			<< (option == 2 ? selected + ">" : not_selected) << "                          Sign In                                                                        " << RESET << endl
			<< "\n\n\n" << message
			<< "\n\n\n" << CYAN_BG //23
			<< "                                                                                                                 " << endl
			<< RESET << endl; //25

		message = "";

		switch (_getch())
		{
		case KEY_UP:
			if (option > optionStart)
			{
				option--;
			}
			else
			{
				option = optionEnd;
			}
			break;

		case KEY_DOWN:
			if (option < optionEnd)
			{
				option++;
			}
			else
			{
				option = optionStart;
			}
			break;

		case KEY_ESCAPE:
			selecting = false;
			break;

		case KEY_ENTER:
			if (option == 0)
			{
				size = sizeof(acc.IC) / sizeof(acc.IC[0]);
				bool exit = false;

				while (!exit)
				{
					Erase_Lines(0, 21);
					cout << "\n\n\n\n\n\n"
						<< (option == 0 ? selected + ">" : not_selected) << "                          ID      : " << acc.IC << RESET << "\n\n"
						<< (option == 1 ? selected : not_selected) << "                          Password: " << encrypted << RESET << "\n\n\n\n"
						<< (option == 2 ? selected + ">" : not_selected) << "                          Sign In                                                                        " << RESET << endl
						<< "\n\n\n\n\n\n" << CYAN_BG
						<< "                                                                                                                 " << endl
						<< RESET << endl;
					inputOperations::Selected_Input(exit, size, acc.IC);
				}
			}
			else if (option == 1)
			{
				size = sizeof(acc.Password) / sizeof(acc.Password[0]);
				bool exit = false;

				while (!exit)
				{
					Erase_Lines(0, 21);
					cout << "\n\n\n\n\n\n"
						<< (option == 0 ? selected : not_selected) << "                          ID      : " << acc.IC << RESET << "\n\n"
						<< (option == 1 ? selected + ">" : not_selected) << "                          Password: " << acc.Password << RESET << "\n\n\n\n"
						<< (option == 2 ? selected + ">" : not_selected) << "                          Sign In                                                                        " << RESET << endl
						<< "\n\n\n\n\n\n" << CYAN_BG
						<< "                                                                                                                 " << endl
						<< RESET << endl;
					char keyIn = _getch();

					if (keyIn == 0 || keyIn == -32)
					{
						keyIn = _getch(); // Get the actual key code
					}
					else if (keyIn == KEY_DELETE)
					{
						Erase_EncryptedElements(encrypted, currentPasswordIndex);
						Erase_Elements(acc.Password, currentPasswordIndex);
					}
					else if (keyIn == KEY_ENTER)
					{
						exit = true;
					}
					else
					{
						inputOperations::InputEncrypted(size, currentPasswordIndex, encrypted, '*');
						inputOperations::Input(size, currentPasswordIndex, acc.Password, keyIn);
					}
				}
			}
			else if (option == 2)
			{
				if (acc.Login())
				{
					db.PrepareStatement("SELECT * FROM staff WHERE IC = ?");
					db.statement->setString(1, acc.IC);
					db.QueryResult();
					
					while (db.result->next())
					{
						strcpy_s(usedAcc.Staff_Id, db.result->getString("Staff_Id").c_str());
						strcpy_s(usedAcc.Name, db.result->getString("Name").c_str());
						strcpy_s(usedAcc.DOB, db.result->getString("DOB").c_str());
						strcpy_s(usedAcc.Gender, db.result->getString("Gender").c_str());
						strcpy_s(usedAcc.Race, db.result->getString("Race").c_str());
						strcpy_s(usedAcc.Religion, db.result->getString("Religion").c_str());
						strcpy_s(usedAcc.Address, db.result->getString("Address").c_str());
						strcpy_s(usedAcc.IC, db.result->getString("IC").c_str());
						usedAcc.Salary = db.result->getDouble("Salary");
						strcpy_s(usedAcc.Role, db.result->getString("Role").c_str());
						strcpy_s(usedAcc.Contact_Number, db.result->getString("Contact_Number").c_str());
						strcpy_s(usedAcc.Email, db.result->getString("Email").c_str());
						strcpy_s(usedAcc.Manage_By, db.result->getString("Manage_By").c_str());
					}
					return true;
				}
				else
				{
					message = "# Incorrect ID/Password";
				}
			}
			break;
		}
		Erase_Lines(0, 21);

	} while (selecting);
	return false;
	system("cls");
}

void Menu::Registration_Menu()
{
	Account reg;
	Sql_DB db;
	char salary[10] = {}; //Record the user's data input in salary
	int size = 0, option = 0, optionStart = 0, optionEnd = 13, 
	    option_gender = 0, option_race = 0, option_religion = 0,option_role = 0, option_manage_by = 0; //Record all the option choosen
	bool selecting = true; //Used to quit from registration menu
	string selected = BWHITE_TEXT + BOLD;
	string not_selected = GRAY_TEXT;
	string query; //Store the query for database operations
	string message; //Show whether the registration is success or not
	
	cout << CYAN_BG << YELLOW_TEXT << RAPID_BLINK << BOLD
		<< "                                                                                                                 " << endl
		<< "                                                  REGISTRATION                                                   " << endl
		<< "                                                                                                                 " << endl
		<< RESET << endl;
	cout << "\n\n\n"; //6
	do
	{
		cout << (option == 0 ? selected : not_selected) << "                  Name            : " << reg.Name << RESET << "\n"
			<< (option == 1 ? selected : not_selected) << "                  DOB(YYYY-MM-DD) : " << reg.DOB << RESET << "\n"
			<< (option == 2 ? selected : not_selected) << "                  Gender(M,F)     : " << reg.Gender << RESET << "\n"
			<< (option == 3 ? selected : not_selected) << "                  Race            : " << reg.Race << RESET << "\n"
			<< (option == 4 ? selected : not_selected) << "                  Religion        : " << reg.Religion << RESET << "\n" //11
			<< (option == 5 ? selected : not_selected) << "                  Address         : " << reg.Address << RESET << "\n"
			<< (option == 6 ? selected : not_selected) << "                  IC              : " << reg.IC << RESET << "\n"
			<< (option == 7 ? selected : not_selected) << "                  Salary(RM)      : " << reg.Salary << RESET << "\n"
			<< (option == 8 ? selected : not_selected) << "                  Role            : " << reg.Role << RESET << "\n"
			<< (option == 9 ? selected : not_selected) << "                  Contact Number  : " << reg.Contact_Number << RESET << "\n"
			<< (option == 10 ? selected : not_selected) << "                  Email           : " << reg.Email << RESET << "\n"
			<< (option == 11 ? selected : not_selected) << "                  Password        : " << reg.Password << RESET << "\n"
			<< (option == 12 ? selected : not_selected) << "                  Manage By*      : " << reg.Manage_By << RESET << "\n\n\n\n" //22
			<< (option == 13 ? selected + ">" : not_selected) << "                  Register" << RESET << endl
			<< "\n" << message
			<< "\n\n" << CYAN_BG
			<< "                                                                                                                 " << RESET << endl; //28

		message = "";

		switch (_getch())
		{
		case KEY_UP:
			if (option > optionStart)
			{
				option--;
			}
			else
			{
				option = optionEnd;
			}
			break;

		case KEY_DOWN:
			if (option < optionEnd)
			{
				option++;
			}
			else
			{
				option = optionStart;
			}
			break;

		case KEY_ESCAPE:
			selecting = false;
			break;

		case KEY_ENTER:
			switch (option)
			{
				case 0:
				{
					size = sizeof(reg.Name) / sizeof(reg.Name[0]);
					bool exit = false;

					while (!exit)
					{
						Registration_Option_Menu(option, selected, not_selected, reg);
						inputOperations::Selected_Input(exit, size, reg.Name);
					}
					break;
				}
				case 1:
				{
					size = sizeof(reg.DOB) / sizeof(reg.DOB[0]);
					bool exit = false;

					while (!exit)
					{
						Registration_Option_Menu(option, selected, not_selected, reg);
						inputOperations::Selected_Input(exit, size, reg.DOB);
					}
					break;
				}
				case 2:
				{
					size = sizeof(reg.Gender) / sizeof(reg.Gender[0]);
					bool exit = false;
					string input_option[2] = { "M", "F" }; //Customize the gender option for user
					int size_option = sizeof(input_option) / sizeof(input_option[0]);

					while (!exit)
					{
						string showed = "  < " + input_option[option_gender] + " >";
						strcpy_s(reg.Gender, size, showed.c_str());
						Registration_Option_Menu(option, selected, not_selected, reg);
						inputOperations::Available_Input(exit, input_option, size_option, option_gender, reg.Gender, size);
					}
					break;
				}
				case 3:
				{
					size = sizeof(reg.Race) / sizeof(reg.Race[0]);
					bool exit = false;
					string input_option[4] = { "Chinese", "Malay", "Indian", "Other" }; //Customize the race option for user
					int size_option = sizeof(input_option) / sizeof(input_option[0]);

					while (!exit)
					{
						string showed = "  < " + input_option[option_race] + " >";
						strcpy_s(reg.Race, size, showed.c_str());
						Registration_Option_Menu(option, selected, not_selected, reg);
						inputOperations::Available_Input(exit, input_option, size_option, option_race, reg.Race, size);
					}
					break;
				}
				case 4:
				{
					size = sizeof(reg.Religion) / sizeof(reg.Religion[0]);
					bool exit = false;
					string input_option[4] = { "Buddhism", "Islam", "Hinduism", "Other" }; //Customize the race option for user
					int size_option = sizeof(input_option) / sizeof(input_option[0]);

					while (!exit)
					{
						string showed = "  < " + input_option[option_religion] + " >";
						strcpy_s(reg.Religion, size, showed.c_str());
						Registration_Option_Menu(option, selected, not_selected, reg);
						inputOperations::Available_Input(exit, input_option, size_option, option_religion, reg.Religion, size);
					}
					break;
				}
				case 5:
				{
					size = sizeof(reg.Address) / sizeof(reg.Address[0]);
					bool exit = false;

					while (!exit)
					{
						Registration_Option_Menu(option, selected, not_selected, reg);
						inputOperations::Selected_Input(exit, size, reg.Address);
					}
					break;
				}
				case 6:
				{
					size = sizeof(reg.IC) / sizeof(reg.IC[0]);
					bool exit = false;

					while (!exit)
					{
						Registration_Option_Menu(option, selected, not_selected, reg);
						inputOperations::Selected_Input(exit, size, reg.IC);
					}
					break;
				}
				case 7:
				{
					size = sizeof(salary) / sizeof(salary[0]);
					bool exit = false;

					while (!exit)
					{
						Registration_Option_Menu(option, selected, not_selected, reg);
						inputOperations::Selected_Input(exit, size, salary, reg.Salary);
					}
					break;
				}
				case 8:
				{
					size = sizeof(reg.Role) / sizeof(reg.Role[0]);
					bool exit = false;
					string input_option[5] = { "Cleaner", "Doctor", "HR", "Nurse", "Pharmacist" }; //Customize the role option for user
					int size_option = sizeof(input_option) / sizeof(input_option[0]);

					while (!exit)
					{
						string showed = "  < " + input_option[option_role] + " >";
						strcpy_s(reg.Role, size, showed.c_str());
						Registration_Option_Menu(option, selected, not_selected, reg);
						inputOperations::Available_Input(exit, input_option, size_option, option_role, reg.Role, size);
					}
					break;
				}
				case 9:
				{
					size = sizeof(reg.Contact_Number) / sizeof(reg.Contact_Number[0]);
					bool exit = false;

					while (!exit)
					{
						Registration_Option_Menu(option, selected, not_selected, reg);
						inputOperations::Selected_Input(exit, size, reg.Contact_Number);
					}
					break;
				}
				case 10:
				{
					size = sizeof(reg.Email) / sizeof(reg.Email[0]);
					bool exit = false;

					while (!exit)
					{
						Registration_Option_Menu(option, selected, not_selected, reg);
						inputOperations::Selected_Input(exit, size, reg.Email);
					}
					break;
				}
				case 11:
				{
					size = sizeof(reg.Password) / sizeof(reg.Password[0]);
					bool exit = false;

					while (!exit)
					{
						Registration_Option_Menu(option, selected, not_selected, reg);
						inputOperations::Selected_Input(exit, size, reg.Password);
					}
					break;
				}
				case 12:
				{
					size = sizeof(reg.Manage_By) / sizeof(reg.Manage_By[0]);
					bool exit = false;

					//Get how many manager available in the database
					string query = "SELECT * FROM Staff WHERE Manage_By IS NULL AND Role = '" + string(reg.Role) + "'";
					int row_count = Get_RowCount(query) + 1; //To set 0 as NULL, if user dont want input anything
					string* Manager_Name = new string[row_count]; //dynamic array
					string* Manager_Id = new string[row_count]; //dynamic array

					//Get the Manager's name and staff_id
					db.PrepareStatement(query);
					db.QueryResult();

					if (db.result)
					{
						int i = 1;
						while (db.result->next())
						{
							Manager_Name[i] = db.result->getString("Name");
							Manager_Id[i] = db.result->getString("Staff_Id");
							i++;
						}
					}

					while (!exit)
					{
						string showed = "  < " + Manager_Name[option_manage_by] + " >";
						strcpy_s(reg.Manage_By, size, showed.c_str());
						Registration_Option_Menu(option, selected, not_selected, reg);
						inputOperations::Available_Input(exit, Manager_Name, row_count, option_manage_by, reg.Manage_By, size);
					}
					strcpy_s(reg.Manage_By, Manager_Id[option_manage_by].c_str()); //Insert manager's staff_id into the Register object

					//delete dynamic array
					delete[] Manager_Name;
					delete[] Manager_Id;
					break;
				}
				case 13:
				{
					reg.Registration(message);
					if (message == "# Successfully registered")
					{
						cout << message;
						_getch();
						selecting = false;
					}
					break;
				}
			}
			break;
		}
		Erase_Lines(0, 21);

	} while (selecting);
}
/*====================================================================================================================*/



/**********************************************************************************************************************/
//Cleaner Menu function
int Cleaner_Menu::Operations_Menu(Cleaner usedAccount)
{
	int option = 0, optionStart = 0, optionEnd;
	usedAccount.Is_Manager() == true ? optionEnd = 3 : optionEnd = 1;
	string selected = BWHITE_TEXT + BOLD;
	string not_selected = GRAY_TEXT;
	string query; //Store the query for database operations


	cout << GREEN_BG << YELLOW_TEXT << RAPID_BLINK << BOLD
		<< "                                                                                                                 " << endl
		<< "                                            CLEANER OPERATIONS MENU                                              " << endl
		<< "                                                                                                                 " << endl
		<< RESET << endl;
	cout << "\n\n\n"; //6

	do
	{
		if (usedAccount.Is_Manager())
		{
			cout << (option == 0 ? selected + ">" : not_selected) << "         View Cleaning Schedule" << RESET << "\n\n"
				<< (option == 1 ? selected + ">" : not_selected) << "         Manage Room Cleaning Schedule" << RESET << "\n\n"
				<< (option == 2 ? selected + ">" : not_selected) << "         Manage Ward Cleaning Schedule" << RESET << "\n\n\n\n\n"
				<< (option == 3 ? selected + ">" : not_selected) << "         Sign Out" << RESET << endl
				<< "\n\n\n" << GREEN_BG
				<< "                                                                                                                 " << RESET << "\n\n"; //15

		}
		else
		{
			cout << (option == 0 ? selected + ">" : not_selected) << "         View Cleaning Schedule" << RESET << "\n\n\n\n\n\n\n\n\n"
				<< (option == 1 ? selected + ">" : not_selected) << "         Sign Out" << RESET << endl
				<< "\n\n\n" << GREEN_BG
				<< "                                                                                                                 " << RESET << "\n\n"; //15
		}

		cout << "Current Staff\n"
			<< "-----------------\n"
			<< "Name: " << usedAccount.Name << endl
			<< "Staff ID: " << usedAccount.Staff_Id << (usedAccount.Is_Manager() ? " (Manager)" : "") << endl;//29

		switch (_getch())
		{
		case KEY_UP:
			if (option > optionStart)
			{
				option--;
			}
			else
			{
				option = optionEnd;
			}
			break;

		case KEY_DOWN:
			if (option < optionEnd)
			{
				option++;
			}
			else
			{
				option = optionStart;
			}
			break;
		case KEY_ENTER:
			if (option == optionEnd)
			{
				return 3; //3 is sign out
			}
			else
			{
				return option;
			}
			
			break;
		}
		Erase_Lines(0, 19);

	} while (true);
}

void Cleaner_Menu::Room_Schedule(string room_schedule[][12], const int rooms, const int slots)
{
	Sql_DB db;
	string query = "SELECT * FROM clean_room";

	int room = 0;
	string timeSlot = "";
	string timeSet[12] = {"00", "02", "04", "06", "08", "10", "12", "14", "16", "18", "20", "22"};

	if (Get_RowCount(query) != 0)
	{
		db.PrepareStatement(query);
		db.QueryResult();

		if (db.result)
		{
			while (db.result->next())
			{
				room = db.result->getInt("Room_Id");
				timeSlot = db.result->getString("Cleaning_Time");
				for (int roomNumber = 0; roomNumber < rooms; roomNumber++)
				{
					if (room == roomNumber+1)
					{
						for (int slotNumber = 0; slotNumber < slots; slotNumber++)
						{
							if (timeSlot.substr(0, 2) == timeSet[slotNumber])
							{
								room_schedule[roomNumber][slotNumber] = db.result->getString("Staff_Id");
								break;
							}
						}
					}
					
				}
				
			}
		}
	}
}

void Cleaner_Menu::Ward_Schedule(string ward_schedule[][12], const int wards, const int slots)
{
	Sql_DB db;
	string query = "SELECT * FROM clean_ward";

	int ward = 0;
	string timeSlot = "";
	string timeSet[12] = { "00", "02", "04", "06", "08", "10", "12", "14", "16", "18", "20", "22" };

	if (Get_RowCount(query) != 0)
	{
		db.PrepareStatement(query);
		db.QueryResult();

		if (db.result)
		{
			while (db.result->next())
			{
				ward = db.result->getInt("Ward_Id");
				timeSlot = db.result->getString("Cleaning_Time");
				for (int wardNumber = 0; wardNumber < wards; wardNumber++)
				{
					if (ward == wardNumber + 1)
					{
						for (int slotNumber = 0; slotNumber < slots; slotNumber++)
						{
							if (timeSlot.substr(0, 2) == timeSet[slotNumber])
							{
								ward_schedule[wardNumber][slotNumber] = db.result->getString("Staff_Id");
								break;
							}
						}
					}

				}

			}
		}
	}
}

void Cleaner_Menu::Show_Room_Schedule(string room_schedule[][12], const int rooms, const int slots)
{
	cout << "\n";
	cout << "                                         ROOM CLEANING SCHEDULE                                         ";
	cout << "\n\n";

	cout << "========================================================================================================\n";
	cout << "| Room | 00:00 | 02:00 | 04:00 | 06:00 | 08:00 | 10:00 | 12:00 | 14:00 | 16:00 | 18:00 | 20:00 | 22:00 |\n";
	cout << "========================================================================================================\n";

	for (int room = 0; room < rooms; room++) {
		cout << "|  " << room + 1 << "   ";
		for (int slot = 0; slot < slots; slot++) {
			cout << "| " << setw(5) << (room_schedule[room][slot].empty() ? "    " : room_schedule[room][slot]) << " ";
		}
		cout << "|\n";
		cout << "--------------------------------------------------------------------------------------------------------\n";
	}
}

void Cleaner_Menu::Show_Ward_Schedule(string ward_schedule[][12], const int wards, const int slots)
{
	cout << "\n\n";
	cout << "                                         WARD CLEANING SCHEDULE                                         ";
	cout << "\n\n";

	cout << "========================================================================================================\n";
	cout << "| Ward | 00:00 | 02:00 | 04:00 | 06:00 | 08:00 | 10:00 | 12:00 | 14:00 | 16:00 | 18:00 | 20:00 | 22:00 |\n";
	cout << "========================================================================================================\n";

	for (int ward = 0; ward < wards; ward++) {
		cout << "|  " << ward + 1 << "   ";
		for (int slot = 0; slot < slots; slot++) {
			cout << "| " << setw(5) << (ward_schedule[ward][slot].empty() ? "    " : ward_schedule[ward][slot]) << " ";
		}
		cout << "|\n";
		cout << "--------------------------------------------------------------------------------------------------------\n";
	}
}

void Cleaner_Menu::Selected_ViewSchedule(Cleaner cln, string room_schedule[][12], string ward_schedule[][12],const int rooms, const int wards,const int slots)
{
	system("cls");

	Show_Room_Schedule(room_schedule, rooms, slots);
	Show_Ward_Schedule(ward_schedule, wards, slots);

	cout << "\n\nYour Staff_Id: " << cln.Get_Id();
}

//Only shows up when the user is Manager
void Cleaner_Menu::Selected_Manage(Cleaner cln, string room_schedule[][12], string ward_schedule[][12], const int rooms, const int wards,const int slots, string RoomOrWard)
{
	system("cls");
	Sql_DB db;
	string selected = BWHITE_TEXT + BOLD;
	string not_selected = GRAY_TEXT;
	string timeSet[12] = { "00", "02", "04", "06", "08", "10", "12", "14", "16", "18", "20", "22" };
	int option = 0, optionStart = 0, optionEnd = 1, optionCleanerId = 0, optionAdd = 0;
	bool selecting = true;

	cout << "\n";
	if (RoomOrWard == "Room")
	{
		cout << "                                         ROOM CLEANING SCHEDULE                                         ";
	}
	else
	{
		cout << "                                         WARD CLEANING SCHEDULE                                         ";
	}
	cout << "\n\n";

	cout << "========================================================================================================\n";

	if (RoomOrWard == "Room")
	{
		cout << "| Room | 00:00 | 02:00 | 04:00 | 06:00 | 08:00 | 10:00 | 12:00 | 14:00 | 16:00 | 18:00 | 20:00 | 22:00 |\n";
	}
	else
	{
		cout << "| Ward | 00:00 | 02:00 | 04:00 | 06:00 | 08:00 | 10:00 | 12:00 | 14:00 | 16:00 | 18:00 | 20:00 | 22:00 |\n";
	}
	cout << "========================================================================================================\n";

	do
	{
		Room_Schedule(room_schedule, rooms, slots);
		Ward_Schedule(ward_schedule, rooms, slots);

		bool adding = true, addingSelecting = true;

		if (RoomOrWard == "Room")
		{
			for (int room = 0; room < rooms; room++) {
				cout << "|  " << room + 1 << "   ";
				for (int slot = 0; slot < slots; slot++) {
					cout << "| " << setw(5) << (room_schedule[room][slot].empty() ? "    " : room_schedule[room][slot]) << " ";
				}
				cout << "|\n";
				cout << "--------------------------------------------------------------------------------------------------------\n";
			}
		}
		else
		{
			for (int ward = 0; ward < wards; ward++) {
				cout << "|  " << ward + 1 << "   ";
				for (int slot = 0; slot < slots; slot++) {
					cout << "| " << setw(5) << (ward_schedule[ward][slot].empty() ? "    " : ward_schedule[ward][slot]) << " ";
				}
				cout << "|\n";
				cout << "--------------------------------------------------------------------------------------------------------\n";
			}
		}


		cout << "\n\nYour Staff Id: " << cln.Get_Id() << "\n\n";
		cout << (option == 0 ? selected + "> " : not_selected) << "Add" << RESET << "\n\n";
		cout << (option == 1 ? selected + "> " : not_selected) << "Delete\n" << RESET;

		switch (_getch())
		{
			case KEY_UP:
			{
				if (option > optionStart)
				{
					option--;
				}
				else
				{
					option = optionEnd;
				}
				break;

			}
			case KEY_DOWN:
			{
				if (option < optionEnd)
				{
					option++;
				}
				else
				{
					option = optionStart;
				}
				break;
			}
			case KEY_ESCAPE:
			{
				selecting = false;
				break;
			}
			case KEY_ENTER:
			{
				switch (option)
				{
				case 0: //Add Cleaner into the time schedule
				{
					adding = true;
					int currentRow = 0, currentColumn = 0;

					RoomOrWard == "Room" ? Erase_Lines(0, 2 * rooms + 7) : Erase_Lines(0, 2 * wards + 7);

					do
					{
						if (RoomOrWard == "Room") //Display Room Schedule
						{
							for (int room = 0; room < rooms; room++)
							{
								cout << "|  " << room + 1 << "   ";
								for (int slot = 0; slot < slots; slot++)
								{
									if (room == currentRow && slot == currentColumn)
									{
										cout << "| " << selected << setw(5) << (room_schedule[room][slot].empty() ? " *  " : room_schedule[room][slot]) << " " << RESET;
									}
									else
									{
										cout << "| " << not_selected << setw(5) << (room_schedule[room][slot].empty() ? " *  " : room_schedule[room][slot]) << " " << RESET;
									}

								}
								cout << "|\n";
								cout << "--------------------------------------------------------------------------------------------------------\n";
							}
						}
						else
						{
							for (int ward = 0; ward < wards; ward++) //Display Ward Schedule
							{
								cout << "|  " << ward + 1 << "   ";
								for (int slot = 0; slot < slots; slot++)
								{
									if (ward == currentRow && slot == currentColumn)
									{
										cout << "| " << selected << setw(5) << (ward_schedule[ward][slot].empty() ? " *  " : ward_schedule[ward][slot]) << " " << RESET;
									}
									else
									{
										cout << "| " << not_selected << setw(5) << (ward_schedule[ward][slot].empty() ? " *  " : ward_schedule[ward][slot]) << " " << RESET;
									}

								}
								cout << "|\n";
								cout << "--------------------------------------------------------------------------------------------------------\n";
							}
						}

						switch (_getch())
						{
							case KEY_UP:
							{
								if (currentRow > 0)
								{
									currentRow--;
								}
								break;

							}
							case KEY_DOWN:
							{
								if (currentRow < (RoomOrWard == "Room" ? rooms - 1 : wards - 1))
								{
									currentRow++;
								}
								break;
							}
							case KEY_LEFT:
							{
								if (currentColumn > 0)
								{
									currentColumn--;
								}
								break;
							}
							case KEY_RIGHT:
							{
								if (currentColumn < slots - 1)
								{
									currentColumn++;
								}
								break;
							}
							case KEY_ESCAPE:
							{
								adding = false;
								break;
							}
							case KEY_ENTER:
							{
								Account account_detailed;
								addingSelecting = true;
								do
								{
									cout << "\n\n";
									cout << (optionAdd == 0 ? selected : not_selected) << "Staff Id: " << account_detailed.Staff_Id << RESET;
									cout << "\n\n";
									cout << (optionAdd == 1 ? selected : not_selected) << "Confirm" << RESET << "\n\n\n";

									switch (_getch())
									{
									case KEY_UP:
									{
										if (optionAdd > 0)
										{
											optionAdd--;
										}
										break;
									}
									case KEY_DOWN:
									{
										if (optionAdd < 1)
										{
											optionAdd++;
										}
										break;
									}
									case KEY_ESCAPE:
									{
										addingSelecting = false;
										break;
									}
									case KEY_ENTER:
									{
										if (optionAdd == 0)
										{
											int size = sizeof(account_detailed.Staff_Id) / sizeof(account_detailed.Staff_Id[0]);
											bool exit = false;

											//Get how many staff in Cleaner role available in the database
											string query = "SELECT * FROM Staff WHERE Role = 'Cleaner'";
											int row_count = 1; //To set 0 as NULL, if user dont want input anything
											db.PrepareStatement(query);
											db.QueryResult();

											if (db.result != nullptr) //Avoid nullptr
											{
												while (db.result->next())
												{
													row_count++;
												}
											}

											string* Cleaner_Id = new string[row_count]; //dynamic array

											//Get the Cleaner's Id
											db.PrepareStatement(query);
											db.QueryResult();

											if (db.result)
											{
												int i = 1;
												while (db.result->next())
												{
													Cleaner_Id[i] = db.result->getString("Staff_Id");
													i++;
												}
											}
											while (!exit)
											{
												Erase_Lines(0, 7);
												string showed = "  < " + Cleaner_Id[optionCleanerId] + " >";
												strcpy_s(account_detailed.Staff_Id, size, showed.c_str());

												cout << "\n\n";
												cout << (optionAdd == 0 ? selected + "> " : not_selected) << "Staff Id: " << account_detailed.Staff_Id << RESET;
												cout << "\n\n";
												cout << (optionAdd == 1 ? selected : not_selected) << "Confirm" << RESET << "\n\n\n";

												inputOperations::Available_Input(exit, Cleaner_Id, row_count, optionCleanerId, account_detailed.Staff_Id, size);
											}

											delete[] Cleaner_Id;
										}
										else if (optionAdd == 1)
										{
											if (RoomOrWard == "Room")
											{
												AddRoom(account_detailed, timeSet, currentRow, currentColumn, addingSelecting, adding);
											}
											else
											{
												AddWard(account_detailed, timeSet, currentRow, currentColumn, addingSelecting, adding);
											}
											
										}
										break;
									}
								}
								Erase_Lines(0, 7);
							} while (addingSelecting);
							break;
							}
						}
						if (adding == true) { RoomOrWard == "Room" ? Erase_Lines(0, 2 * rooms) : Erase_Lines(0, 2 * wards); }
					} while (adding);
					break;
					}
				case 1: //Delete Cleaner from the time schedule
				{
					adding = true;
					int currentRow = 0, currentColumn = 0;

					RoomOrWard == "Room" ? Erase_Lines(0, 2 * rooms + 7) : Erase_Lines(0, 2 * wards + 7);

					do
					{
						if (RoomOrWard == "Room")
						{
							for (int room = 0; room < rooms; room++)
							{
								cout << "|  " << room + 1 << "   ";
								for (int slot = 0; slot < slots; slot++)
								{
									if (room == currentRow && slot == currentColumn)
									{
										cout << "| " << selected << setw(5) << (room_schedule[room][slot].empty() ? " *  " : room_schedule[room][slot]) << " " << RESET;
									}
									else
									{
										cout << "| " << not_selected << setw(5) << (room_schedule[room][slot].empty() ? " *  " : room_schedule[room][slot]) << " " << RESET;
									}

								}
								cout << "|\n";
								cout << "--------------------------------------------------------------------------------------------------------\n";
							}

						}
						else
						{
							for (int ward = 0; ward < wards; ward++)
							{
								cout << "|  " << ward + 1 << "   ";
								for (int slot = 0; slot < slots; slot++)
								{
									if (ward == currentRow && slot == currentColumn)
									{
										cout << "| " << selected << setw(5) << (ward_schedule[ward][slot].empty() ? " *  " : ward_schedule[ward][slot]) << " " << RESET;
									}
									else
									{
										cout << "| " << not_selected << setw(5) << (ward_schedule[ward][slot].empty() ? " *  " : ward_schedule[ward][slot]) << " " << RESET;
									}

								}
								cout << "|\n";
								cout << "--------------------------------------------------------------------------------------------------------\n";
							}

						}
						
						switch (_getch())
						{
							case KEY_UP:
							{
								if (currentRow > 0)
								{
									currentRow--;
								}
								break;

							}
							case KEY_DOWN:
							{
								if (currentRow < (RoomOrWard == "Room" ? rooms - 1 : wards - 1))
								{
									currentRow++;
								}
								break;
							}
							case KEY_LEFT:
							{
								if (currentColumn > 0)
								{
									currentColumn--;
								}
								break;
							}
							case KEY_RIGHT:
							{
								if (currentColumn < slots - 1)
								{
									currentColumn++;
								}
								break;
							}
							case KEY_ESCAPE:
							{
								adding = false;
								break;
							}
							case KEY_ENTER:
							{
								if (RoomOrWard == "Room")
								{
									db.PrepareStatement("DELETE FROM clean_room WHERE Staff_Id = ? AND Room_Id = ? AND Cleaning_Time = ?");
									db.statement->setString(1, room_schedule[currentRow][currentColumn]);
								}
								else
								{
									db.PrepareStatement("DELETE FROM clean_ward WHERE Staff_Id = ? AND Ward_Id = ? AND Cleaning_Time = ?");
									db.statement->setString(1, ward_schedule[currentRow][currentColumn]);
								}
								db.statement->setString(2, to_string(currentRow + 1));
								db.statement->setString(3, timeSet[currentColumn] + ":00");
								db.QueryStatement();
								adding = false;

								if (RoomOrWard == "Room")
								{
									room_schedule[currentRow][currentColumn].clear();
								}
								else
								{
									ward_schedule[currentRow][currentColumn].clear();
								}
								
								break;
							}

						}

						if (adding == true) { RoomOrWard == "Room" ? Erase_Lines(0, 2 * rooms) : Erase_Lines(0, 2 * wards); }


					} while (adding);

					break;
					}	
				}
			}
		}
		RoomOrWard == "Room" ? Erase_Lines(0, 2 * rooms + (adding == true ? 7 : 0)) : Erase_Lines(0, 2 * wards + (adding == true ? 7 : 0));
	} while (selecting);

}

void Cleaner_Menu::AddRoom(Account account_detailed, string timeSet[], int currentRow, int currentColumn, bool& addingSelecting, bool& adding)
{
	//Check if the staff is working on other room at that time
	int working = 0;

	//Check working hour at room
	Sql_DB db;
	db.PrepareStatement("SELECT COUNT(*) AS TotalWorking FROM clean_room WHERE Staff_Id = ? AND Cleaning_Time = ?");
	db.statement->setString(1, account_detailed.Staff_Id);
	db.statement->setString(2, timeSet[currentColumn] + ":00");
	db.QueryResult();

	db.result->next();
	working += db.result->getInt("TotalWorking");

	//Check working hour at ward
	db.PrepareStatement("SELECT COUNT(*) AS TotalWorking FROM clean_ward WHERE Staff_Id = ? AND Cleaning_Time = ?");
	db.statement->setString(1, account_detailed.Staff_Id);
	db.statement->setString(2, timeSet[currentColumn] + ":00");
	db.QueryResult();

	db.result->next();
	working += db.result->getInt("TotalWorking");

	//Check whether there's cleaner working at there
	db.PrepareStatement("SELECT COUNT(*) AS Is_Cleaning FROM clean_room WHERE Cleaning_Time = '" + timeSet[currentColumn] + ":00:00' AND Room_Id = '" + to_string(currentRow + 1) + "'");
	db.QueryResult();
	db.result->next();
	int otherCleaner = db.result->getInt("Is_Cleaning");

	if (working > 0)
	{
		cout << "Invalid, Cleaner is working at that time";
		_getch();
	}
	else if (otherCleaner != 0)
	{
		cout << "Invalid, there's other Cleaner working at that time";
		_getch();
	}
	else
	{
		try
		{
			db.PrepareStatement("INSERT INTO clean_room VALUES(?,?,?)");
			db.statement->setString(1, account_detailed.Staff_Id);
			db.statement->setString(2, to_string(currentRow + 1));
			db.statement->setString(3, timeSet[currentColumn] + ":00");
			db.statement->executeUpdate();
			addingSelecting = false;
			adding = false;
		}
		catch (sql::SQLException& e)
		{
			cout << "Invalid Input.";
			_getch();
		}
		
	}
}

void Cleaner_Menu::AddWard(Account account_detailed, string timeSet[], int currentRow, int currentColumn, bool& addingSelecting, bool& adding)
{
	//Check if the staff is working on other room at that time
	int working = 0;

	//Check working hour at room
	Sql_DB db;
	db.PrepareStatement("SELECT COUNT(*) AS TotalWorking FROM clean_ward WHERE Staff_Id = ? AND Cleaning_Time = ?");
	db.statement->setString(1, account_detailed.Staff_Id);
	db.statement->setString(2, timeSet[currentColumn] + ":00");
	db.QueryResult();

	db.result->next();
	working += db.result->getInt("TotalWorking");

	//Check working hour at ward
	db.PrepareStatement("SELECT COUNT(*) AS TotalWorking FROM clean_ward WHERE Staff_Id = ? AND Cleaning_Time = ?");
	db.statement->setString(1, account_detailed.Staff_Id);
	db.statement->setString(2, timeSet[currentColumn] + ":00");
	db.QueryResult();

	db.result->next();
	working += db.result->getInt("TotalWorking");

	//Check whether there's cleaner working at there
	db.PrepareStatement("SELECT COUNT(*) AS Is_Cleaning FROM clean_ward WHERE Cleaning_Time = '" + timeSet[currentColumn] + ":00:00' AND Ward_Id = '" + to_string(currentRow + 1) + "'");
	db.QueryResult();
	db.result->next();
	int otherCleaner = db.result->getInt("Is_Cleaning");

	if (working > 0)
	{
		cout << "Invalid, Cleaner is working at that time";
		_getch();
	}
	else if (otherCleaner != 0)
	{
		cout << "Invalid, there's other Cleaner working at that time";
		_getch();
	}
	else
	{
		try
		{
			db.PrepareStatement("INSERT INTO clean_ward VALUES(?,?,?)");
			db.statement->setString(1, account_detailed.Staff_Id);
			db.statement->setString(2, to_string(currentRow + 1));
			db.statement->setString(3, timeSet[currentColumn] + ":00");
			db.statement->executeUpdate();
			addingSelecting = false;
			adding = false;
		}
		catch (sql::SQLException& e)
		{
			cout << "Invalid Input.";
			_getch();
		}
	}
}
/*====================================================================================================================*/



/**********************************************************************************************************************/
//Doctor Menu function
int Doctor_Menu::Operations_Menu(Doctor usedAccount)
{
	int option = 0, optionStart = 0, optionEnd;
	usedAccount.Is_Manager() == true ? optionEnd = 4 : optionEnd = 3;
	string selected = BWHITE_TEXT + BOLD;
	string not_selected = GRAY_TEXT;
	string query; //Store the query for database operations


	cout << BRED_BG << YELLOW_TEXT << RAPID_BLINK << BOLD
		<< "                                                                                                                 " << endl
		<< "                                            DOCTOR OPERATIONS MENU                                               " << endl
		<< "                                                                                                                 " << endl
		<< RESET << endl;
	cout << "\n\n\n"; //6

	do
	{
		if (usedAccount.Is_Manager())
		{
			cout << (option == 0 ? selected + ">" : not_selected) << "         Appointment with Patient" << RESET << "\n\n" //Check current scheduled patient's record
				<< (option == 1 ? selected + ">" : not_selected) << "         Update Unpaid Patient's Record" << RESET << "\n\n" //Modify completed appoinment patient's record
				<< (option == 2 ? selected + ">" : not_selected) << "         View Patient's Record" << RESET << "\n\n" //View all the patient's record that doctor did
				<< (option == 3 ? selected + ">" : not_selected) << "         Manage Consultation Room" << RESET << "\n\n" //Only for Doctor who is manager, allow to see and add Room for Doctors
				<< "\n\n\n\n\n" //15
				<< (option == 4 ? selected + ">" : not_selected) << "         Sign Out" << RESET << endl
				<< "\n\n\n" << BRED_BG
				<< "                                                                                                                 " << RESET << "\n\n"; //19
		}
		else
		{
			cout << (option == 0 ? selected + ">" : not_selected) << "         Appointment with Patient" << RESET << "\n\n" //Check current scheduled patient's record
				<< (option == 1 ? selected + ">" : not_selected) << "         Update Unpaid Patient's Record" << RESET << "\n\n" //Modify completed appointment patient's record
				<< (option == 2 ? selected + ">" : not_selected) << "         View Patient's Record" << RESET << "\n\n" //View all the patient's record that doctor did
				<< "\n\n"
				<< "\n\n\n\n\n" //15
				<< (option == 3 ? selected + ">" : not_selected) << "         Sign Out" << RESET << endl
				<< "\n\n\n" << BRED_BG
				<< "                                                                                                                 " << RESET << "\n\n"; //19
		}

		cout << "Current Staff\n"
			<< "-----------------\n"
			<< "Name: " << usedAccount.Name << endl
			<< "Staff ID: " << usedAccount.Staff_Id << (usedAccount.Is_Manager() ? " (Manager)" : "") << endl;//23

		switch (_getch())
		{
		case KEY_UP:
			if (option > optionStart)
			{
				option--;
			}
			else
			{
				option = optionEnd;
			}
			break;

		case KEY_DOWN:
			if (option < optionEnd)
			{
				option++;
			}
			else
			{
				option = optionStart;
			}
			break;
		case KEY_ENTER:
			if (option == optionEnd)
			{
				return 4; //Sign out
			}
			else
			{
				return option; //Return operations
			}
			break;
		}
		Erase_Lines(0, 23);

	} while (true);
}

string Doctor_Menu::Get_List(string query, string first_column, string third_column, string tittle, string additionalOption)
{
	system("cls");
	Sql_DB db;
	char search[31] = {};
	bool selecting = true;
	int option = 0, optionStart = 0, delete_line = 0, optionView = 0;
	string paid = RESET + GREEN_TEXT;
	string unpaid = RESET + RED_TEXT;
	string selected = BWHITE_TEXT + BOLD;
	string not_selected = GRAY_TEXT;
	string originalQuery = query;

	cout << BRED_BG << YELLOW_TEXT << BOLD
		<< "                                                                                                                 " << endl
		<< tittle << endl
		<< "                                                                                                                 " << endl
		<< RESET << endl;
	cout << "\n"; //5

	cout << "---------------------------------------------------------------------------------------" << endl;
	cout << "|     No." << blank(9, "No.") << "|          Name" << blank(40, "Name") << "|    " << third_column << blank(15, third_column) << "|          " << endl;
	cout << "|              " << "|                                                  " << "|                   " << "|\n";
	cout << "---------------------------------------------------------------------------------------" << endl; //11

	do
	{
		int optionEnd = Get_RowCount(query);

		//List Menu
		string* id = new string[optionEnd];
		string* name = new string[optionEnd];
		string* third_column_showed = new string[optionEnd];

		//Get the id, name and status
		db.PrepareStatement(query);
		db.QueryResult();

		delete_line = 0;

		if (db.statement)
		{
			int i = 0;
			while (db.result->next())
			{
				id[i] = db.result->getString(first_column);
				name[i] = db.result->isNull("Name") ? "N/A" : db.result->getString("Name");
				third_column_showed[i] = db.result->isNull(third_column) ? "N/A" : db.result->getString(third_column);
				i++;
				delete_line += 2;
			}
		}

		for (int i = 0; i < optionEnd; i++)
		{
			cout << (i == option ? selected : not_selected) << "|     " << id[i] << blank(9, id[i])
				<< "|          " << name[i] << blank(40, name[i]) << "|    "
				<< (third_column_showed[i] == "Y" ? paid : "") << (third_column_showed[i] == "N" ? unpaid : "") << third_column_showed[i] << RESET << blank(15, third_column_showed[i]) << (i == option ? selected : not_selected) << "|          \n";
			cout << "|              " << "|                                                  " << "|                   " << "|\n";
		}
		cout << RESET << "=======================================================================================\n\n" << endl; //14 , erase_line where 9 - 2(print) = 7

		if (additionalOption == "Search")
		{
			cout << (option == optionEnd ? selected : not_selected) << "      Search: " << search << RESET << endl;
		}
		else
		{
			cout << endl;
		}


		switch (_getch())
		{
			case KEY_UP:
				if (option > optionStart)
				{
					option--;
				}
				else
				{
					if (additionalOption.empty())
					{
						option = optionEnd - 1;
					}
					else
					{
						option = optionEnd;
					}
				}
				break;

			case KEY_DOWN:
				if (additionalOption.empty() ? option < optionEnd - 1 : option < optionEnd)
				{
					option++;
				}
				else
				{
					option = optionStart;
				}
				break;
			case KEY_ESCAPE:
				delete[]id;
				delete[]name;
				delete[]third_column_showed;
				return "-1";
				break;
			case KEY_ENTER:
				if (additionalOption.empty())
				{
					delete[]name;
					delete[]third_column_showed;
					return id[option];
				}
				
				if (additionalOption == "Search")
				{
					if (option == optionEnd)
					{
						int size = sizeof(search) / sizeof(search[0]);
						bool exit = false;

						while (!exit)
						{
							Erase_Lines(0, 4 + delete_line);
							for (int i = 0; i < optionEnd; i++)
							{
								cout << (i == option ? selected : not_selected) << "|     " << id[i] << blank(9, id[i])
									<< "|          " << name[i] << blank(40, name[i]) << "|    "
									<< third_column_showed[i] << RESET << blank(15, third_column_showed[i]) << (i == option ? selected : not_selected) << "|          \n";
								cout << "|              " << "|                                                  " << "|                   " << "|\n";
							}
							cout << RESET << "=======================================================================================\n\n" << endl; //14 , erase_line where 9 - 2(print) = 7
							cout << (option == optionEnd ? selected + ">" : not_selected) << "      Search: " << search << endl;

							Selected_Input(exit, size, search);
						}

						if (search[0] != '\0')
						{
							query = originalQuery +  " WHERE Name LIKE '" + string(search) + "%'";
							option = 0;
						}
						else
						{
							query = originalQuery;
							option = 0;
						}
					}
					else
					{
						Erase_Lines(0, 1);
						return id[option];
					}
				}
				break;
		}
		Erase_Lines(0, 4 + delete_line);
		delete[]id;
		delete[]name;
		delete[]third_column_showed;
	} while (selecting);


}

void Doctor_Menu::Patient_Record_Menu(Patient_Record pr, string message, int option, bool keyIn)
{
	string selected;
	keyIn ? selected = BWHITE_TEXT + BOLD + ">" : selected = BWHITE_TEXT + BOLD;
	string not_selected = GRAY_TEXT;

	cout << (option == 0 ? selected : not_selected) << "  Date(YYYY-MM-DD) : " << pr.date << RESET << endl << endl;
	cout << (option == 1 ? selected : not_selected) << "  Diagnosis        : " << pr.diagnosis << RESET << endl << endl;
	cout << (option == 2 ? selected : not_selected) << "  Treatment        : " << pr.treatment << RESET << endl << endl;
	cout << (option == 3 ? selected : not_selected) << "  Treatment Price  : " << pr.treatment_price << RESET << endl << endl;
	cout << (option == 4 ? selected : not_selected) << "  Doctor Charges   : " << pr.doctor_charges << RESET << endl << endl;
	cout << (option == 5 ? selected : not_selected) << "  Description*     : " << pr.Description << RESET << endl << endl;
	cout << "=======================================================================================\n\n";
	cout << (option == 6 ? selected : not_selected) << "  Confirm\n\n\n" << RESET;
	cout << message;
}

void Doctor_Menu::Selected_Appointment(Doctor doc, string number)
{
	system("cls");
	Sql_DB db;
	Patient patient;
	Patient_Record pr;
	string selected = BWHITE_TEXT + BOLD;
	string not_selected = GRAY_TEXT;
	string message;
	char treatment_price[10] = {}, doctor_charges[10] = {};
	int option = 0, optionStart = 0, optionEnd = 6;
	bool selecting = true;

	//Get the patient's information
	patient.Patient_Detail("SELECT P.* FROM appointment A JOIN patient P ON A.Patient_Id = P.Patient_Id WHERE Appointment_Id = ?", number);

	cout << "Patient Information\n";
	cout << "=======================================================================================\n\n";
	cout << "  Name             : " << patient.Name << RESET << endl;
	cout << "  DOB              : " << patient.DOB << RESET << endl;
	cout << "  Gender           : " << patient.Gender << RESET << endl;
	cout << "  Race             : " << patient.Race << RESET << endl;
	cout << "  Religion         : " << patient.Religion << RESET << endl;
	cout << "  Address          : " << patient.Address << RESET << endl;
	cout << "  IC               : " << patient.IC << RESET << endl;
	cout << "=======================================================================================\n\n";

	cout << "Patient Record\n";
	cout << "=======================================================================================\n\n";

	do
	{
		Patient_Record_Menu(pr, message, option, false);

		message = "";

		switch (_getch())
		{
			case KEY_UP:
				if (option == optionStart) { option = optionEnd; }
				else { option--; }
				break;

			case KEY_DOWN:
				if (option == optionEnd) { option = optionStart; }
				else { option++; }
				break;

			case KEY_ESCAPE:
				selecting = false;
				break;

			case KEY_ENTER:
			{
				switch (option)
				{
					case 0:
					{
						int size = sizeof(pr.date) / sizeof(pr.date[0]);
						bool exit = false;

						while (!exit)
						{
							Erase_Lines(0, 17);
							Patient_Record_Menu(pr, message, option, true);
							inputOperations::Selected_Input(exit, size, pr.date);
						}
						break;
					}
					case 1:
					{
						int size = sizeof(pr.diagnosis) / sizeof(pr.diagnosis[0]);
						bool exit = false;

						while (!exit)
						{
							Erase_Lines(0, 17);
							Patient_Record_Menu(pr, message, option, true);
							inputOperations::Selected_Input(exit, size, pr.diagnosis);
						}
						break;
					}
					case 2:
					{
						int size = sizeof(pr.treatment) / sizeof(pr.treatment[0]);
						bool exit = false;

						while (!exit)
						{
							Erase_Lines(0, 17);
							Patient_Record_Menu(pr, message, option, true);
							inputOperations::Selected_Input(exit, size, pr.treatment);
						}
						break;
					}
					case 3:
					{
						int size = sizeof(treatment_price) / sizeof(treatment_price[0]);
						bool exit = false;

						while (!exit)
						{
							Erase_Lines(0, 17);
							Patient_Record_Menu(pr, message, option, true);
							inputOperations::Selected_Input(exit, size, treatment_price, pr.treatment_price);
						}
						break;
					}
					case 4:
					{
						int size = sizeof(doctor_charges) / sizeof(doctor_charges[0]);
						bool exit = false;

						while (!exit)
						{
							Erase_Lines(0, 17);
							Patient_Record_Menu(pr, message, option, true);
							inputOperations::Selected_Input(exit, size, doctor_charges, pr.doctor_charges);
						}
						break;
					}
					case 5:
					{
						int size = sizeof(pr.Description) / sizeof(pr.Description[0]);
						bool exit = false;

						while (!exit)
						{
							Erase_Lines(0, 17);
							Patient_Record_Menu(pr, message, option, true);
							inputOperations::Selected_Input(exit, size, pr.Description);
						}
						break;
					}
					case 6:
					{
						doc.AddPatientRecord(doc.Staff_Id , patient.Patient_Id, number, pr, message);

						if (message != "Invalid Input")
						{
							selecting = false;
						}
					}
				}

			}
		}

		Erase_Lines(0, 17);
	} while (selecting);

}

void Doctor_Menu::Selected_Update_PatientRecord(Doctor doc, string record_no)
{
	system("cls");
	Sql_DB db;
	Patient patient;
	Patient_Record pr;
	string selected = BWHITE_TEXT + BOLD;
	string not_selected = GRAY_TEXT;
	string message;
	char treatment_price[10] = {}, doctor_charges[10] = {};
	int option = 0, optionStart = 0, optionEnd = 6;
	bool selecting = true;

	pr.PatientRecord_Detail(record_no);

	cout << "Update Patient's Record\n";
	cout << "=======================================================================================\n\n";

	do
	{
		Patient_Record_Menu(pr, message, option, false);

		message = "";

		switch (_getch())
		{
			case KEY_UP:
				if (option == optionStart) { option = optionEnd; }
				else
				{
					if (pr.payment_status[0] == 'Y' && option == 5) //If the patient record is paid, the price cnt be modified
					{
						option = 2;
					}
					else
					{
						option--;
					}
				}
				break;

			case KEY_DOWN:
				if (option == optionEnd) { option = optionStart; }
				else
				{ 
					if (pr.payment_status[0] == 'Y' && option == 2) //If the patient record is paid, the price cnt be modified
					{
						option = 5;
					}
					else
					{
						option++;
					}
				}
				break;

			case KEY_ESCAPE:
				selecting = false;
				break;

			case KEY_ENTER:
			{
				switch (option)
				{
					case 0:
					{
						int size = sizeof(pr.date) / sizeof(pr.date[0]);
						bool exit = false;

						while (!exit)
						{
							Erase_Lines(0, 17);
							Patient_Record_Menu(pr, message, option, true);
							inputOperations::Selected_Input(exit, size, pr.date);
						}
						break;
					}
					case 1:
					{
						int size = sizeof(pr.diagnosis) / sizeof(pr.diagnosis[0]);
						bool exit = false;

						while (!exit)
						{
							Erase_Lines(0, 17);
							Patient_Record_Menu(pr, message, option, true);
							inputOperations::Selected_Input(exit, size, pr.diagnosis);
						}
						break;
					}
					case 2:
					{
						int size = sizeof(pr.treatment) / sizeof(pr.treatment[0]);
						bool exit = false;

						while (!exit)
						{
							Erase_Lines(0, 17);
							Patient_Record_Menu(pr, message, option, true);
							inputOperations::Selected_Input(exit, size, pr.treatment);
						}
						break;
					}
					case 3:
					{
						if (pr.payment_status[0] == 'N')
						{
							int size = sizeof(treatment_price) / sizeof(treatment_price[0]);
							bool exit = false;

							while (!exit)
							{
								Erase_Lines(0, 17);
								Patient_Record_Menu(pr, message, option, true);
								inputOperations::Selected_Input(exit, size, treatment_price, pr.treatment_price);
							}
							break;
						}
					}
					case 4:
					{
						if (pr.payment_status[0] == 'N')
						{
							int size = sizeof(doctor_charges) / sizeof(doctor_charges[0]);
							bool exit = false;

							while (!exit)
							{
								Erase_Lines(0, 17);
								Patient_Record_Menu(pr, message, option, true);
								inputOperations::Selected_Input(exit, size, doctor_charges, pr.doctor_charges);
							}
							break;
						}
					}
					case 5:
					{
						int size = sizeof(pr.Description) / sizeof(pr.Description[0]);
						bool exit = false;

						while (!exit)
						{
							Erase_Lines(0, 17);
							Patient_Record_Menu(pr, message, option, true);
							inputOperations::Selected_Input(exit, size, pr.Description);
						}
						break;
					}
					case 6:
					{
						doc.UpdatePatientRecord(doc.Staff_Id , patient.Patient_Id, record_no, pr, message);

						if (message != "Invalid Input")
						{
							selecting = false;
						}
					}
				}

			}
		}

		Erase_Lines(0, 17);

	} while (selecting);

}

void Doctor_Menu::Selected_View_PatientRecord(Doctor doc, string record_no)
{
	Patient_Record pr;
	pr.Show_PatientRecord(record_no);
}

void Doctor_Menu::Selected_Consultation_Room(Doctor doc, string room_no)
{
	Sql_DB db;
	string selected = BWHITE_TEXT + BOLD + ">";
	string not_selected = GRAY_TEXT;
	string query = "SELECT R.Room_Id, S.Name, R.Staff_Id FROM room R LEFT JOIN staff S ON R.Staff_Id = S.Staff_Id";
	bool selecting = true;
	int selected_staffId = 0, selected_room = 0;
	int optionEnd = Get_RowCount(query);
	int optionStaff = Get_RowCount("SELECT Staff_Id FROM Staff WHERE Role = 'Doctor'") + 1;

	//List Menu
	string* roomId = new string[optionEnd];
	string* name = new string[optionEnd];
	string* worked_StaffId = new string[optionEnd];
	string* staffId= new string[optionStaff];

	//Get the room's id, doctor name and doctor's staff id
	db.PrepareStatement(query);
	db.QueryResult();

	if (db.statement)
	{
		int i = 0;
		while (db.result->next())
		{
			roomId[i] = db.result->getString("Room_Id");
			name[i] = db.result->isNull("Name") ? "N/A" : db.result->getString("Name");
			worked_StaffId[i] = db.result->isNull("Staff_Id") ? "N/A" : db.result->getString("Staff_Id");
			i++;
		}
	}

	db.PrepareStatement("SELECT Staff_Id FROM Staff WHERE Role = 'Doctor'");
	db.QueryResult();

	if (db.statement)
	{
		int i = 1;
		while (db.result->next())
		{
			staffId[i] = db.result->getString("Staff_Id");
			i++;
		}
	}

	for (int i = 0; i < optionEnd; i++)
	{
		if (room_no == roomId[i])
		{
			for (int j = 0; j < optionStaff; j++)
			{
				if (worked_StaffId[i] == "N/A")
				{
					selected_room = i;
					selected_staffId = 0;
				}
				else if (staffId[j] == worked_StaffId[i])
				{
					selected_room = i;
					selected_staffId = j;
				}
				
			}
		}
	}

	do
	{
		string showed = "  < " + staffId[selected_staffId] + " >";
		cout << selected << "      Staff ID: " << showed << endl;

		switch (_getch())
		{
			case KEY_LEFT:
			{
				if (selected_staffId > 0)
				{
					selected_staffId--;
				}
				break;
			}
			case KEY_RIGHT:
			{
				if (selected_staffId < optionStaff - 1)
				{
					selected_staffId++;
				}
				break;
			}
			case KEY_ESCAPE:
			{
				selecting = false;
				break;
			}
			case KEY_ENTER:
			{
				doc.UpdateConsultationRoom(roomId[selected_room], staffId[selected_staffId]);
				selecting = false;
				break;
			}
		}
		Erase_Lines(0, 1);

	} while (selecting);

	delete[]roomId;
	delete[]name;
	delete[]staffId;
	delete[]worked_StaffId;
}
/*====================================================================================================================*/



/**********************************************************************************************************************/
//HR Menu function
int HR_Menu::Operations_Menu(HR usedAccount)
{
	int option = 0, optionStart = 0,optionEnd = 5;
	string selected = BWHITE_TEXT + BOLD;
	string not_selected = GRAY_TEXT;
	string query; //Store the query for database operations


	cout << MAGENTA_BG << YELLOW_TEXT << RAPID_BLINK << BOLD
		<< "                                                                                                                 " << endl
		<< "                                            HR OPERATIONS MENU                                                   " << endl
		<< "                                                                                                                 " << endl
		<< RESET << endl;
	cout << "\n\n\n"; //6

	do
	{

		cout << (option == 0 ? selected + ">" : not_selected) << "         Manage Registration Request" << RESET << "\n\n"
			<< (option == 1 ? selected + ">" : not_selected) << "         Update Employee Information" << RESET << "\n\n"
			<< (option == 2 ? selected + ">" : not_selected) << "         Delete Employee " << RESET << "\n\n"
			<< (option == 3 ? selected + ">" : not_selected) << "         View Employee List" << RESET << "\n\n"
			<< (option == 4 ? selected + ">" : not_selected) << "         Generate Report" << RESET << "\n\n\n\n\n" //15
			<< (option == 5 ? selected + ">" : not_selected) << "         Sign Out" << RESET << endl
			<< "\n\n\n" << MAGENTA_BG
			<< "                                                                                                                 " << RESET << "\n\n" //25
			<< "Current Staff\n"
			<< "-----------------\n"
			<< "Name: " << usedAccount.Name << endl
			<< "Staff ID: " << usedAccount.Staff_Id << (usedAccount.Is_Manager() ? " (Manager)" : "") << endl;//29

		switch (_getch())
		{
		case KEY_UP:
			if (option > optionStart)
			{
				option--;
			}
			else
			{
				option = optionEnd;
			}
			break;

		case KEY_DOWN:
			if (option < optionEnd)
			{
				option++;
			}
			else
			{
				option = optionStart;
			}
			break;
		case KEY_ENTER:
			return option;
			break;
		}
		Erase_Lines(0, 23);

	} while (true);
}

int HR_Menu::Report_Menu(HR usedAccount)
{
	system("cls");
	int option = 0, optionStart = 0, optionEnd = 2;
	string selected = BWHITE_TEXT + BOLD;
	string not_selected = GRAY_TEXT;

	cout << MAGENTA_BG << YELLOW_TEXT << RAPID_BLINK << BOLD
		<< "                                                                                                                 " << endl
		<< "                                                   REPORT MENU                                                   " << endl
		<< "                                                                                                                 " << endl
		<< RESET << endl;
	cout << "\n\n\n"; //6

	do
	{
		cout << (option == 0 ? selected + ">" : not_selected) << "         View Transaction Record" << RESET << "\n\n"
			<< (option == 1 ? selected + ">" : not_selected) << "         View Business Report" << RESET << "\n\n\n\n\n"
			<< (option == 2 ? selected + ">" : not_selected) << "         Back" << RESET << "\n\n\n\n\n"; //15

		switch (_getch())
		{
		case KEY_UP:
			if (option > optionStart) { option--; }
			else { option = optionEnd; }
			break;
		case KEY_DOWN:
			if (option < optionEnd) { option++; }
			else { option = optionStart; }
			break;
		case KEY_ENTER:
			if (option == 2) { return -1; }
			else { return option; }
			break;
		case KEY_ESCAPE:
			return -1;
			break;
		}
		Erase_Lines(0, 12);
	} while (true);
}

string HR_Menu::Report_Select_Menu(int option) // NEWEST
{
	Sql_DB db;
	int optionSelect = 0, optionYear = 0, optionMonth = 0;
	string selected = BWHITE_TEXT + BOLD;
	string not_selected = GRAY_TEXT;
	string query; //Store the query for database operations

	// Get how many years inside database
	db.PrepareStatement("SELECT COUNT(DISTINCT YEAR(Date)) AS number_of_years FROM transaction_record");
	db.QueryResult();
	db.result->next();
	int number_of_years = db.result->getInt("number_of_years");

	string* years = new string[number_of_years];
	db.PrepareStatement("SELECT DISTINCT YEAR(Date) AS years FROM transaction_record");
	db.QueryResult();
	int i = 0;
	while (db.result->next())
	{
		years[i] = db.result->getString("years");
		i++;
	}

	//Months array
	// Extra for empty space, if the month is empty which user want years report
	string months[13] = { "", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Okt", "Nov", "Dec" }; 

	if (option == 0) // Transaction and Business Record
	{
		while (true)
		{
			cout << (optionSelect == 0 ? selected : not_selected) << "Year    : " << years[optionYear] << RESET << endl;
			cout << (optionSelect == 1 ? selected : not_selected) << "*Month  : " << months[optionMonth] << RESET << endl;
			cout << (optionSelect == 2 ? selected : not_selected) << "Confirm" << RESET << endl;

			switch (_getch())
			{
			case KEY_UP:
				if (optionSelect > 0) { optionSelect--; }
				else { optionSelect = 2; }
				break;
			case KEY_DOWN:
				if (optionSelect < 2) { optionSelect++; }
				else { optionSelect = 0; }
				break;
			case KEY_ENTER:
			{
				bool exit = false;
				if (optionSelect == 0)
				{
					while (!exit)
					{
						Erase_Lines(0, 3);
						string showed = " < " + years[optionYear] + " >";
						cout << (optionSelect == 0 ? selected : not_selected) << "Year    : " << showed << RESET << endl;
						cout << (optionSelect == 1 ? selected : not_selected) << "*Month  : " << months[optionMonth] << RESET << endl;
						cout << (optionSelect == 2 ? selected : not_selected) << "Confirm" << RESET << endl;

						switch (_getch())
						{
						case KEY_LEFT:
							if (optionYear > 0) { optionYear--; }
							break;
						case KEY_RIGHT:
							if (optionYear < number_of_years - 1) { optionYear++; }
							break;
						case KEY_ENTER:
							exit = true;
							break;
						}
					}
				}
				
				if (optionSelect == 1)
				{
					while (!exit)
					{
						Erase_Lines(0, 3);
						string showed = " < " + months[optionMonth] + " >";
						cout << (optionSelect == 0 ? selected : not_selected) << "Year    : " << years[optionYear] << RESET << endl;
						cout << (optionSelect == 1 ? selected : not_selected) << "*Month  : " << showed << RESET << endl;
						cout << (optionSelect == 2 ? selected : not_selected) << "Confirm" << RESET << endl;

						switch (_getch())
						{
						case KEY_LEFT:
							if (optionMonth > 0) { optionMonth--; }
							break;
						case KEY_RIGHT:
							if (optionMonth < 12) { optionMonth++; }
							break;
						case KEY_ENTER:
							exit = true;
							break;
						}
					}
				}
				
				if (optionSelect == 2)
				{
					if (months[optionMonth].empty()) { return years[optionYear]; }
					else 
					{
						string Month;
						if (optionMonth < 10) { Month = "0" + to_string(optionMonth); }
						else { Month = to_string(optionMonth); }
						return years[optionYear] + "-" + Month;
					}
				}
				break;
			}
			case KEY_ESCAPE:
				return "";
				break;
			}
			Erase_Lines(0, 3);
		}

	}
	
	if (option == 1) // Generate Year Report
	{
		while (true)
		{
			cout << (optionSelect == 0 ? selected : not_selected) << "Year    : " << years[optionYear] << RESET << endl;
			cout << (optionSelect == 1 ? selected : not_selected) << "Confirm" << RESET << endl;

			switch (_getch())
			{
			case KEY_UP:
				if (optionSelect > 0) { optionSelect--; }
				else { optionSelect = 1; }
				break;
			case KEY_DOWN:
				if (optionSelect < 1) { optionSelect++; }
				else { optionSelect = 0; }
				break;
			case KEY_ENTER:
			{
				bool exit = false;
				if (optionSelect == 0)
				{
					while (!exit)
					{
						Erase_Lines(0, 2);
						string showed = " < " + years[optionYear] + " >";
						cout << (optionSelect == 0 ? selected : not_selected) << "Year    : " << showed << RESET << endl;
						cout << (optionSelect == 1 ? selected : not_selected) << "Confirm" << RESET <<endl;

						switch (_getch())
						{
						case KEY_LEFT:
							if (optionYear > 0) { optionYear--; }
							break;
						case KEY_RIGHT:
							if (optionYear < number_of_years - 1) { optionYear++; }
							break;
						case KEY_ENTER:
							exit = true;
							break;
						}
					}
				}
				else if (optionSelect == 1)
				{
					return years[optionYear];
				}
				break;
			}
			case KEY_ESCAPE:
				return "";
				break;
			}
			Erase_Lines(0, 2);
		}
	}
}

string HR_Menu::Get_List(string query, string first_column, string second_column ,string third_column, string tittle)
{
	system("cls");
	Sql_DB db;
	bool selecting = true;
	int option = 0, optionStart = 0, delete_line = 0;
	string selected = BWHITE_TEXT + BOLD;
	string not_selected = GRAY_TEXT;
	string accepted = RESET + GREEN_TEXT;
	string rejected = RESET + RED_TEXT;
	int optionEnd = Get_RowCount(query);

	//List Menu
	string* id = new string[optionEnd];
	string* name = new string[optionEnd];
	string* third_column_showed = new string[optionEnd];

	//Get the registration id, staff's name and status in registration
	db.PrepareStatement(query);
	db.QueryResult();

	if (db.statement)
	{
		int i = 0;
		while (db.result->next())
		{
			id[i] = db.result->getString(first_column);
			name[i] = db.result->getString(second_column);
			third_column_showed[i] = db.result->getString(third_column);

			if (third_column == "Total_Price")
			{
				// Get the position of the decimal point
				int dotPosition = third_column_showed[i].find('.');

				// Copy the string start from '.' to 2 decimal point which is (1 + 2 = 3)
				third_column_showed[i] = third_column_showed[i].substr(0, dotPosition + 3);
			}

			i++;
			delete_line+=2;
		}
	}
	cout << MAGENTA_BG << YELLOW_TEXT << BOLD
		<< "                                                                                                                 " << endl
		<< tittle << endl
		<< "                                                                                                                 " << endl
		<< RESET << endl;
	cout << "\n"; //5

	do
	{
		cout << "---------------------------------------------------------------------------------------" << endl;
		cout << "|     No." << blank(9, "No.") << "|          Name" << blank(40, "Name") << "|    " << third_column << blank(15, third_column) << "|          " << endl;
		cout << "|              " << "|                                                  " << "|                   " << "|\n";
		cout << "---------------------------------------------------------------------------------------" << endl; //11

		for (int i = 0; i < optionEnd; i++)
		{
			cout << (i == option ? selected : not_selected) << "|     " << id[i] << blank(9, id[i])
				 << "|          " << name[i] << blank(40, name[i]) << "|    " 
				 << (third_column_showed[i] == "Accepted" ? accepted : "") << (third_column_showed[i] == "Rejected" ? rejected : "") << third_column_showed[i] << RESET << blank(15, third_column_showed[i]) << (i == option ? selected : not_selected) << "|          \n";
			cout << "|              " << "|                                                  "  << "|                   " << "|\n";
		}
		cout << RESET << "=======================================================================================\n\n" << endl;

		switch (_getch())
		{
		case KEY_UP:
			if (option > optionStart)
			{
				option--;
			}
			else
			{
				option = optionEnd-1;
			}
			break;

		case KEY_DOWN:
			if (option < optionEnd-1)
			{
				option++;
			}
			else
			{
				option = optionStart;
			}
			break;
		case KEY_ESCAPE:
			return "-1";
			break;
		case KEY_ENTER:
			if(optionEnd != 0){ return id[option]; }
			break;
		}
		Erase_Lines(0, 7 + delete_line);
	} while (selecting);

	delete[]id;
	delete[]name;
	delete[]third_column_showed;
}

void HR_Menu::Register_Detail(string query, int number, Account& account_detailed)
{
	Sql_DB db;
	db.PrepareStatement(query);
	db.statement->setInt(1, number);
	db.QueryResult();
	if (db.result != nullptr)
	{
		while (db.result->next())
		{
			strcpy_s(account_detailed.Name, db.result->getString("Name").c_str());
			strcpy_s(account_detailed.DOB, db.result->getString("DOB").c_str());
			strcpy_s(account_detailed.Gender, db.result->getString("Gender").c_str());
			strcpy_s(account_detailed.Race, db.result->getString("Race").c_str());
			strcpy_s(account_detailed.Religion, db.result->getString("Religion").c_str());
			strcpy_s(account_detailed.Address, db.result->getString("Address").c_str());
			strcpy_s(account_detailed.IC, db.result->getString("IC").c_str());
			account_detailed.Salary = db.result->getDouble("Salary");
			strcpy_s(account_detailed.Role, db.result->getString("Role").c_str());
			strcpy_s(account_detailed.Contact_Number, db.result->getString("Contact_Number").c_str());
			strcpy_s(account_detailed.Email, db.result->getString("Email").c_str());
			strcpy_s(account_detailed.Password, db.result->getString("Password").c_str());
			strcpy_s(account_detailed.Manage_By, db.result->getString("Manage_By").c_str());
			strcpy_s(account_detailed.Approved_By, db.result->getString("Approved_By").c_str());
			strcpy_s(account_detailed.Status, db.result->getString("Status").c_str());
		}
	}

	cout << "\n";
	cout << "=======================================================================================\n\n"; //2
	cout << "  Name           : " << account_detailed.Name << endl;
	cout << "  DOB            : " << account_detailed.DOB << endl;
	cout << "  Gender         : " << account_detailed.Gender << endl;
	cout << "  Race           : " << account_detailed.Race << endl;
	cout << "  Religion       : " << account_detailed.Religion << endl;
	cout << "  Address        : " << account_detailed.Address << endl;
	cout << "  IC             : " << account_detailed.IC << endl;
	cout << "  Salary (RM)    : " << account_detailed.Salary << endl;
	cout << "  Role           : " << account_detailed.Role << endl;
	cout << "  Contact Number : " << account_detailed.Contact_Number << endl;
	cout << "  Email          : " << account_detailed.Email << endl;
	cout << "  Password       : " << account_detailed.Password << endl;
	cout << "  Manage By      : " << account_detailed.Manage_By << endl;
	cout << "  Approved By    : " << account_detailed.Approved_By << endl;
	cout << "  Status         : " << account_detailed.Status << endl << endl;
	cout << "=======================================================================================\n\n"; //21
}

void HR_Menu::Employee_Detail(string query, string number, Account& account_detailed)
{
	Sql_DB db;
	db.PrepareStatement(query);
	db.statement->setString(1, number);
	db.QueryResult();
	if (db.result != nullptr)
	{
		while (db.result->next())
		{
			strcpy_s(account_detailed.Staff_Id, db.result->getString("Staff_Id").c_str());
			strcpy_s(account_detailed.Name, db.result->getString("Name").c_str());
			strcpy_s(account_detailed.DOB, db.result->getString("DOB").c_str());
			strcpy_s(account_detailed.Gender, db.result->getString("Gender").c_str());
			strcpy_s(account_detailed.Race, db.result->getString("Race").c_str());
			strcpy_s(account_detailed.Religion, db.result->getString("Religion").c_str());
			strcpy_s(account_detailed.Address, db.result->getString("Address").c_str());
			strcpy_s(account_detailed.IC, db.result->getString("IC").c_str());
			account_detailed.Salary = db.result->getDouble("Salary");
			strcpy_s(account_detailed.Role, db.result->getString("Role").c_str());
			strcpy_s(account_detailed.Contact_Number, db.result->getString("Contact_Number").c_str());
			strcpy_s(account_detailed.Email, db.result->getString("Email").c_str());
			strcpy_s(account_detailed.Manage_By, db.result->getString("Manage_By").c_str());
		}
	}

}

void HR_Menu::Selected_Registration_Request(HR hr,int registration_no)
{
	system("cls");
	Sql_DB db;
	Account account_detailed;
	string selected = BWHITE_TEXT + BOLD + ">";
	string not_selected = GRAY_TEXT;
	int option = 0, optionStart = 0, optionEnd = 1;
	bool selecting = true;

	//Get the registered user's data from database
	Register_Detail("SELECT * FROM registration WHERE Registration_Id = ?", registration_no, account_detailed);

	//Option for either Accept or Reject the Registration Request
	do
	{
		if (account_detailed.Status[0] == 'W')
		{
			cout << (option == 0 ? selected : not_selected) << "  Accept" << RESET << "\n"
				 << (option == 1 ? selected : not_selected) << "  Reject" << RESET << "\n";

			switch (_getch())
			{
				case KEY_UP:
					if (option == optionStart) { option = optionEnd; }
					else { option--; }
					break;

				case KEY_DOWN:
					if (option == optionEnd) { option = optionStart; }
					else { option++; }
					break;

				case KEY_ESCAPE:
					selecting = false;
					break;
				
				case KEY_ENTER:
					if(option == 0)
					{
						int Staff_Id = 0;
						string data;

						//Create an account for the user
						db.PrepareStatement("INSERT INTO account VALUES(?, ?)");
						db.statement->setString(1, account_detailed.IC);
						db.statement->setString(2, account_detailed.encrypt(account_detailed.Password));
						db.QueryStatement();

						//Insert the data of the user
						db.PrepareStatement("SELECT MAX(Staff_Id) AS Staff_Id FROM staff");
						db.QueryResult();

						while (db.result->next())
						{
							data = db.result->getString("Staff_Id");
							Staff_Id = stoi(data.substr(1, 4)) + 1; //Get the digit from staff_id, for example H1004 we get 1004 by these, and +1 for apply staff_Id to register
						}

						string query = "INSERT INTO staff VALUES('";
						if (account_detailed.Role[0] == 'C') { query += "C" + to_string(Staff_Id) + "'"; }
						else if (account_detailed.Role[0] == 'D') { query += "D" + to_string(Staff_Id) + "'"; }
						else if (account_detailed.Role[0] == 'H') { query += "H" + to_string(Staff_Id) + "'"; }
						else if (account_detailed.Role[0] == 'N') { query += "N" + to_string(Staff_Id) + "'"; }
						else if (account_detailed.Role[0] == 'P') { query += "P" + to_string(Staff_Id) + "'"; }

						query += ", ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ";
						if (account_detailed.Manage_By[0] == '\0')
						{
							query += "NULL, 'Activate')";
						}
						else
						{
							query += "?, 'Activate')";
						}

						db.PrepareStatement(query);
						db.statement->setString(1, account_detailed.Name);
						db.statement->setString(2, account_detailed.DOB);
						db.statement->setString(3, account_detailed.Gender);
						db.statement->setString(4, account_detailed.Race);
						db.statement->setString(5, account_detailed.Religion);
						db.statement->setString(6, account_detailed.Address);
						db.statement->setString(7, account_detailed.IC);
						db.statement->setDouble(8, account_detailed.Salary);
						db.statement->setString(9, account_detailed.Role);
						db.statement->setString(10, account_detailed.Contact_Number);
						db.statement->setString(11, account_detailed.Email);

						//Update the status of register
						if (account_detailed.Manage_By[0] != '\0')
						{
							db.statement->setString(12, account_detailed.Manage_By);
						}
						db.QueryStatement();

						db.PrepareStatement("UPDATE registration SET Status = 'Accepted', Approved_By = ? WHERE Registration_Id = ?");
						db.statement->setString(1, hr.Get_Id());
						db.statement->setInt(2, registration_no);
						db.statement->executeUpdate();

						selecting = false;
					}
					else if (option == 1)
					{
						db.PrepareStatement("UPDATE registration SET Status = 'Rejected', Approved_By = ? WHERE Registration_Id = ?");
						db.statement->setString(1, hr.Get_Id());
						db.statement->setInt(2, registration_no);
						db.statement->executeUpdate();
						selecting = false;
					}

			}
			Erase_Lines(0, 2);
		}
		else
		{
			switch (_getch())
			{
			case KEY_ESCAPE:
				selecting = false;
				break;
			}
		}

	} while (selecting);
}

void HR_Menu::Update_Option_Menu(int option, string selected, string not_selected, Account& account_detailed)
{
	Erase_Lines(0, 22);
	selected = selected + ">";

	cout << "\n";
	cout << "=======================================================================================\n\n";
	cout << (option == 0 ? selected : not_selected) << "  Staff_Id       : " << account_detailed.Staff_Id << RESET << endl;
	cout << (option == 1 ? selected : not_selected) << "  Name           : " << account_detailed.Name << RESET << endl;
	cout << (option == 2 ? selected : not_selected) << "  DOB            : " << account_detailed.DOB << RESET << endl;
	cout << (option == 3 ? selected : not_selected) << "  Gender         : " << account_detailed.Gender << RESET << endl;
	cout << (option == 4 ? selected : not_selected) << "  Race           : " << account_detailed.Race << RESET << endl;
	cout << (option == 5 ? selected : not_selected) << "  Religion       : " << account_detailed.Religion << RESET << endl;
	cout << (option == 6 ? selected : not_selected) << "  Address        : " << account_detailed.Address << RESET << endl;
	cout << (option == 7 ? selected : not_selected) << "  IC             : " << account_detailed.IC << RESET << endl;
	cout << (option == 8 ? selected : not_selected) << "  Salary (RM)    : " << account_detailed.Salary << RESET << endl;
	cout << (option == 9 ? selected : not_selected) << "  Role           : " << account_detailed.Role << RESET << endl;
	cout << (option == 10 ? selected : not_selected) << "  Contact Number : " << account_detailed.Contact_Number << RESET << endl;
	cout << (option == 11 ? selected : not_selected) << "  Email          : " << account_detailed.Email << RESET << endl;
	cout << (option == 12 ? selected : not_selected) << "  Manage By      : " << account_detailed.Manage_By << RESET << endl << endl;
	cout << "=======================================================================================\n\n";
	cout << (option == 13 ? selected : not_selected) << "  Update\n\n\n" << RESET;//21

}

void HR_Menu::Selected_Update_Employee(HR hr, string staff_no)
{
	system("cls");
	Sql_DB db;
	Account account_detailed;
	string selected = BWHITE_TEXT + BOLD;
	string not_selected = GRAY_TEXT;
	string message;
	char salary[15] = {}; //Record the user's data input in salary
	int option = 0, optionStart = 0, optionEnd = 13, option_gender = 0, option_race = 0, option_religion = 0, option_role = 0, option_manage_by = 0;
	int size;
	bool selecting = true;

	Employee_Detail("SELECT * FROM staff WHERE Staff_Id = ?", staff_no, account_detailed);

	//Convert float to char[]
	stringstream ss;
	ss << account_detailed.Salary; //Insert the float value into the ss
	string salaryStr = ss.str();
	strcpy_s(salary, salaryStr.c_str());


	//Get how many manager available in the database
	string query = "SELECT * FROM Staff WHERE Manage_By IS NULL";
	int row_count = Get_RowCount(query) + 1; //To set 0 as NULL, if user dont want input anything
	string* Manager_Name = new string[row_count]; //dynamic array
	string* Manager_Id = new string[row_count]; //dynamic array

	//Get the Manager's name and staff_id
	db.PrepareStatement("SELECT Staff_Id,Name FROM Staff WHERE Manage_By IS NULL");
	db.QueryResult();

	if (db.result)
	{
		int i = 1;
		while (db.result->next())
		{
			Manager_Name[i] = db.result->getString("Name");
			Manager_Id[i] = db.result->getString("Staff_Id");
			i++;
		}
	}

	do
	{
		cout << "\n";
		cout << "=======================================================================================\n\n";
		cout << (option == 0 ? selected : not_selected) << "  Staff_Id       : " << account_detailed.Staff_Id << RESET << endl;
		cout << (option == 1 ? selected : not_selected) << "  Name           : " << account_detailed.Name << RESET << endl;
		cout << (option == 2 ? selected : not_selected) << "  DOB            : " << account_detailed.DOB << RESET << endl;
		cout << (option == 3 ? selected : not_selected) << "  Gender         : " << account_detailed.Gender << RESET << endl;
		cout << (option == 4 ? selected : not_selected) << "  Race           : " << account_detailed.Race << RESET << endl;
		cout << (option == 5 ? selected : not_selected) << "  Religion       : " << account_detailed.Religion << RESET << endl;
		cout << (option == 6 ? selected : not_selected) << "  Address        : " << account_detailed.Address << RESET << endl;
		cout << (option == 7 ? selected : not_selected) << "  IC             : " << account_detailed.IC << RESET << endl;
		cout << (option == 8 ? selected : not_selected) << "  Salary (RM)    : " << account_detailed.Salary << RESET << endl;
		cout << (option == 9 ? selected : not_selected) << "  Role           : " << account_detailed.Role << RESET << endl;
		cout << (option == 10 ? selected : not_selected) << "  Contact Number : " << account_detailed.Contact_Number << RESET << endl;
		cout << (option == 11 ? selected : not_selected) << "  Email          : " << account_detailed.Email << RESET << endl;
		cout << (option == 12 ? selected : not_selected) << "  Manage By      : " << account_detailed.Manage_By << RESET << endl << endl;
		cout << "=======================================================================================\n\n";
		cout << (option == 13 ? selected : not_selected) << "  Update\n\n\n" << RESET;//21
		cout << message;

		message = "";

		switch (_getch())
		{
			case KEY_UP:
				if (option > optionStart)
				{
					option--;
				}
				else
				{
					option = optionEnd;
				}
				break;

			case KEY_DOWN:
				if (option < optionEnd)
				{
					option++;
				}
				else
				{
					option = optionStart;
				}
				break;

			case KEY_ESCAPE:
				selecting = false;
				break;

			case KEY_ENTER:
				switch (option)
				{
					case 1:
					{
						size = sizeof(account_detailed.Name) / sizeof(account_detailed.Name[0]);
						bool exit = false;

						while (!exit)
						{
							Update_Option_Menu(option, selected, not_selected, account_detailed);
							inputOperations::Selected_Input(exit, size, account_detailed.Name);
						}
						break;
					}
					case 2:
					{
						size = sizeof(account_detailed.DOB) / sizeof(account_detailed.DOB[0]);
						bool exit = false;

						while (!exit)
						{
							Update_Option_Menu(option, selected, not_selected, account_detailed);
							inputOperations::Selected_Input(exit, size, account_detailed.DOB);
						}
						break;
					}
					case 3:
					{
						size = sizeof(account_detailed.Gender) / sizeof(account_detailed.Gender[0]);
						bool exit = false;
						string input_option[2] = { "M", "F" }; //Customize the gender option for user
						int size_option = sizeof(input_option) / sizeof(input_option[0]);

						while (!exit)
						{
							string showed = "  < " + input_option[option_gender] + " >";
							strcpy_s(account_detailed.Gender, size, showed.c_str());
							Update_Option_Menu(option, selected, not_selected, account_detailed);
							inputOperations::Available_Input(exit, input_option, size_option, option_gender, account_detailed.Gender, size);
						}
						break;
					}
					case 4:
					{
						size = sizeof(account_detailed.Race) / sizeof(account_detailed.Race[0]);
						bool exit = false;
						string input_option[4] = { "Chinese", "Malay", "Indian", "Other" }; //Customize the race option for user
						int size_option = sizeof(input_option) / sizeof(input_option[0]);

						while (!exit)
						{
							string showed = "  < " + input_option[option_race] + " >";
							strcpy_s(account_detailed.Race, size, showed.c_str());
							Update_Option_Menu(option, selected, not_selected, account_detailed);
							inputOperations::Available_Input(exit, input_option, size_option, option_race, account_detailed.Race, size);
						}
						break;
					}
					case 5:
					{
						size = sizeof(account_detailed.Religion) / sizeof(account_detailed.Religion[0]);
						bool exit = false;
						string input_option[4] = { "Buddhism", "Islam", "Hinduism", "Other" }; //Customize the race option for user
						int size_option = sizeof(input_option) / sizeof(input_option[0]);

						while (!exit)
						{
							string showed = "  < " + input_option[option_religion] + " >";
							strcpy_s(account_detailed.Religion, size, showed.c_str());
							Update_Option_Menu(option, selected, not_selected, account_detailed);
							inputOperations::Available_Input(exit, input_option, size_option, option_religion, account_detailed.Religion, size);
						}
						break;
					}
					case 6:
					{
						size = sizeof(account_detailed.Address) / sizeof(account_detailed.Address[0]);
						bool exit = false;

						while (!exit)
						{
							Update_Option_Menu(option, selected, not_selected, account_detailed);
							inputOperations::Selected_Input(exit, size, account_detailed.Address);
						}
						break;
					}
					/*case 7:
					{
						size = sizeof(account_detailed.IC) / sizeof(account_detailed.IC[0]);
						bool exit = false;

						while (!exit)
						{
							Update_Option_Menu(option, selected, not_selected, account_detailed);
							inputOperations::Selected_Input(exit, size, account_detailed.IC);
						}
						break;
					}*/
					//IC cannot be modified, it should be check corrected when approved the registration by HR
					case 8:
					{
						size = sizeof(salary) / sizeof(salary[0]);
						bool exit = false;

						while (!exit)
						{
							Update_Option_Menu(option, selected, not_selected, account_detailed);
							inputOperations::Selected_Input(exit, size, salary, account_detailed.Salary);
						}
						break;
					}
					case 9:
					{
						size = sizeof(account_detailed.Role) / sizeof(account_detailed.Role[0]);
						bool exit = false;
						string input_option[5] = { "Cleaner", "Doctor", "HR", "Nurse", "Pharmacist" }; //Customize the role option for user
						int size_option = sizeof(input_option) / sizeof(input_option[0]);

						while (!exit)
						{
							string showed = "  < " + input_option[option_role] + " >";
							strcpy_s(account_detailed.Role, size, showed.c_str());
							Update_Option_Menu(option, selected, not_selected, account_detailed);
							inputOperations::Available_Input(exit, input_option, size_option, option_role, account_detailed.Role, size);
						}
						break;
					}
					case 10:
					{
						size = sizeof(account_detailed.Contact_Number) / sizeof(account_detailed.Contact_Number[0]);
						bool exit = false;

						while (!exit)
						{
							Update_Option_Menu(option, selected, not_selected, account_detailed);
							inputOperations::Selected_Input(exit, size, account_detailed.Contact_Number);
						}
						break;
					}
					case 11:
					{
						size = sizeof(account_detailed.Email) / sizeof(account_detailed.Email[0]);
						bool exit = false;

						while (!exit)
						{
							Update_Option_Menu(option, selected, not_selected, account_detailed);
							inputOperations::Selected_Input(exit, size, account_detailed.Email);
						}
						break;
					}
					case 12:
					{
						size = sizeof(account_detailed.Manage_By) / sizeof(account_detailed.Manage_By[0]);
						bool exit = false;

						//Get how many manager available in the database
						string query = "SELECT * FROM Staff WHERE Manage_By IS NULL AND Staff_Id Like ? AND Staff_Id != ? ";
						int row_count = 1; //To set 0 as NULL, if user dont want input anything
						db.PrepareStatement(query);
						db.statement->setString(1, string(1, account_detailed.Staff_Id[0]) + "%"); //Find the manager who have the same Role
						db.statement->setString(2, account_detailed.Staff_Id); //Avoid for manage by himself
						db.QueryResult();

						if (db.result != nullptr) //Avoid nullptr
						{
							while (db.result->next())
							{
								row_count++;
							}
						}

						string* Manager_Name = new string[row_count]; //dynamic array
						string* Manager_Id = new string[row_count]; //dynamic array

						//Get the Manager's name and staff_id
						db.PrepareStatement(query);
						db.statement->setString(1, string(1, account_detailed.Staff_Id[0]) + "%");
						db.statement->setString(2, account_detailed.Staff_Id);
						db.QueryResult();

						if (db.result)
						{
							int i = 1;
							while (db.result->next())
							{
								Manager_Name[i] = db.result->getString("Name");
								Manager_Id[i] = db.result->getString("Staff_Id");
								i++;
							}
						}
						while (!exit)
						{
							string showed = "  < " + Manager_Name[option_manage_by] + " >";
							strcpy_s(account_detailed.Manage_By, size, showed.c_str());
							Update_Option_Menu(option, selected, not_selected, account_detailed);
							inputOperations::Available_Input(exit, Manager_Name, row_count, option_manage_by, account_detailed.Manage_By, size);
						}
						strcpy_s(account_detailed.Manage_By, Manager_Id[option_manage_by].c_str()); //Insert manager's staff_id into the Register object

						//delete dynamic array
						delete[] Manager_Name;
						delete[] Manager_Id;
						break;
					}
					case 13:
					{
						account_detailed.Update(message);
						if (message == "# Updated successfull")
						{
							cout << message;
							_getch();
							selecting = false;
						}
					}

				}
		}
		Erase_Lines(0, 22);
	} while (selecting);
	
	delete[]Manager_Id;
	delete[]Manager_Name;
}

void HR_Menu::Selected_Delete_Employee(HR hr, string staff_no)
{
	system("cls");
	Sql_DB db;
	Account account_detailed;
	string selected = BWHITE_TEXT + BOLD;
	string not_selected = GRAY_TEXT;
	string message;
	int option = 0;
	bool selecting = true;

	Employee_Detail("SELECT * FROM staff WHERE Staff_Id = ?", staff_no, account_detailed);

	do
	{
		cout << "\n";
		cout << "=======================================================================================\n\n";
		cout << "  Staff_Id       : " << account_detailed.Staff_Id << endl;
		cout << "  Name           : " << account_detailed.Name << endl;
		cout << "  DOB            : " << account_detailed.DOB << endl;
		cout << "  Gender         : " << account_detailed.Gender << endl;
		cout << "  Race           : " << account_detailed.Race << endl;
		cout << "  Religion       : " << account_detailed.Religion << endl;
		cout << "  Address        : " << account_detailed.Address << endl;
		cout << "  IC             : " << account_detailed.IC << endl;
		cout << "  Salary (RM)    : " << account_detailed.Salary << endl;
		cout << "  Role           : " << account_detailed.Role << endl;
		cout << "  Contact Number : " << account_detailed.Contact_Number << endl;
		cout << "  Email          : " << account_detailed.Email << endl;
		cout << "  Manage By      : " << account_detailed.Manage_By << endl << endl;
		cout << "=======================================================================================\n\n";
		cout << ">  Delete\n\n\n";//21
		cout << message;

		message = "";

		switch (_getch())
		{
			case KEY_ESCAPE:
			{
				selecting = false;
				break;
			}
			case KEY_ENTER:
			{
				account_detailed.Remove(message);
				if (message != "# Invalid remove, manager cannot be removed")
				{
					selecting = false;
				}
			}
		}
		Erase_Lines(0, 22);
	} while (selecting);
}

void HR_Menu::Selected_View_Employee(HR hr, bool& exit)
{
	system("cls");
	Sql_DB db;
	bool selecting = true;
	int option = 0, optionStart = 0, delete_line = 0;
	string selected = BWHITE_TEXT + BOLD;
	string not_selected = GRAY_TEXT;
	Account search;
	string query = "SELECT * FROM staff";

	cout << MAGENTA_BG << YELLOW_TEXT << BOLD
		<< "                                                                                                                 " << endl
		<< "                                               VIEW EMPLOYEE LIST                                                " << endl
		<< "                                                                                                                 " << endl
		<< RESET << endl;
	cout << "\n"; //5
	cout << "---------------------------------------------------------------------------------------" << endl;
	cout << "|     No." << blank(9, "No.") << "|          Name" << blank(40, "Name") << "|    " << "Role" << blank(15, "Role") << "|          " << endl;
	cout << "|              " << "|                                                  " << "|                   " << "|\n";
	cout << "---------------------------------------------------------------------------------------" << endl; //11



	do
	{
		int optionEnd = Get_RowCount(query);

		//List Menu
		string* id = new string[optionEnd];
		string* name = new string[optionEnd];
		string* third_column_showed = new string[optionEnd];

		//Get the registration id, staff's name and status in registration
		db.PrepareStatement(query);
		db.QueryResult();

		if (db.statement)
		{
			int i = 0;
			while (db.result->next())
			{
				id[i] = db.result->getString("Staff_Id");
				name[i] = db.result->getString("Name");
				third_column_showed[i] = db.result->getString("Role");
				i++;
				delete_line += 2;
			}
		}

		for (int i = 0; i < optionEnd; i++)
		{
			cout << (i == option ? selected : not_selected) << "|     " << id[i] << blank(9, id[i])
				<< "|          " << name[i] << blank(40, name[i]) << "|    "
				<< third_column_showed[i] << blank(15, third_column_showed[i]) << "|          \n";
			cout << "|              " << "|                                                  " << "|                   " << "|\n";
		}
		cout << RESET << "=======================================================================================\n\n" << endl; //14 , erase_line where 9 - 2(print) = 7
		cout << (option == optionEnd ? selected : not_selected) << "      Search: " << search.Name << endl;

		switch (_getch())
		{
		case KEY_UP:
			if (option > optionStart)
			{
				option--;
			}
			else
			{
				option = optionEnd;
			}
			break;

		case KEY_DOWN:
			if (option < optionEnd)
			{
				option++;
			}
			else
			{
				option = optionStart;
			}
			break;
		case KEY_ESCAPE:
			selecting = false;
			exit = false;
			break;

		case KEY_ENTER:
			if (option == optionEnd)
			{
				int size = sizeof(search.Name) / sizeof(search.Name[0]);
				bool exit = false;

				while (!exit)
				{
					Erase_Lines(0, 4 + delete_line);
					for (int i = 0; i < optionEnd; i++)
					{
						cout << (i == option ? selected : not_selected) << "|     " << id[i] << blank(9, id[i])
							<< "|          " << name[i] << blank(40, name[i]) << "|    "
							<< third_column_showed[i] << blank(15, third_column_showed[i]) << "|          \n";
						cout << "|              " << "|                                                  " << "|                   " << "|\n";
					}
					cout << RESET << "=======================================================================================\n\n" << endl; //14 , erase_line where 9 - 2(print) = 7
					cout << (option == optionEnd ? selected + ">" : not_selected) << "      Search: " << search.Name << endl;

					Selected_Input(exit, size, search.Name);
				}

				if (search.Name[0] != '\0')
				{
					query = "SELECT * FROM staff WHERE Name LIKE '%" + string(search.Name) + "%'";
					option = 0;
				}
				else
				{
					query = "SELECT * FROM staff";
				}
			}
			else
			{
				system("cls");
				Account account_detailed;
				Employee_Detail("SELECT * FROM staff WHERE Staff_Id = ?", id[option], account_detailed);
				
				cout << RESET <<"\n";
				cout << "=======================================================================================\n\n";
				cout << "  Staff_Id       : " << account_detailed.Staff_Id << endl;
				cout << "  Name           : " << account_detailed.Name << endl;
				cout << "  DOB            : " << account_detailed.DOB << endl;
				cout << "  Gender         : " << account_detailed.Gender << endl;
				cout << "  Race           : " << account_detailed.Race << endl;
				cout << "  Religion       : " << account_detailed.Religion << endl;
				cout << "  Address        : " << account_detailed.Address << endl;
				cout << "  IC             : " << account_detailed.IC << endl;
				cout << "  Salary (RM)    : " << account_detailed.Salary << endl;
				cout << "  Role           : " << account_detailed.Role << endl;
				cout << "  Contact Number : " << account_detailed.Contact_Number << endl;
				cout << "  Email          : " << account_detailed.Email << endl;
				cout << "  Manage By      : " << account_detailed.Manage_By << endl << endl;
				cout << "=======================================================================================\n\n";

				_getch();
				system("cls");
				selecting = false;
			}

			break;
		}
		Erase_Lines(0, 4 + delete_line);
		delete_line = 0;
		delete[]id;
		delete[]name;
		delete[]third_column_showed;
	} while (selecting);
}

void HR_Menu::Selected_BusinessReport(string year)
{
	system("cls");
	Sql_DB db;
	string selected = BWHITE_TEXT + BOLD + "> ";
	string not_selected = GRAY_TEXT;
	double total_income[12] = {};
	double total_expenses[12] = {};
	double total_profit[12] = {};
	double year_total[3] = {};
	string months[12] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

	cout << "\n\n";
	cout << "                                               HMS Business Report\n\n"
		<< "Year: " << year << endl
		<< "================================================================================================================\n"
		<< "     " << setw(15) << left << "Month"
		<< setw(30) << right << "Total Income"
		<< setw(30) << "Total Expenses"
		<< setw(30) << "Total Profit" << "\n"
		<< "     " << setw(15) << left << ""
		<< setw(30) << right << "(RM)   "
		<< setw(30) << "(RM)   "
		<< setw(30) << "(RM)   " << "\n"
		<< "----------------------------------------------------------------------------------------------------------------\n";

	// Get total income
	db.PrepareStatement("SELECT MONTH(Date) AS Month, SUM(CASE WHEN Total_Price > 0 THEN Total_Price ELSE 0 END) AS Total_Price FROM transaction_record WHERE Date LIKE '" + year + "%' GROUP BY MONTH(Date)");
	db.QueryResult();
	if (db.result)
	{
		int i = 0;
		while (db.result->next())
		{
			// Record total income
			if (db.result->getDouble("Total_Price") > 0) { total_income[i] = db.result->getDouble("Total_Price"); }
			i++;
		}
	}

	// Get total expenses
	db.PrepareStatement("SELECT MONTH(Date) AS Month, SUM(CASE WHEN Total_Price < 0 THEN Total_Price ELSE 0 END) AS Total_Price FROM transaction_record WHERE Date LIKE '" + year + "%' GROUP BY MONTH(Date)");
	db.QueryResult();
	if (db.result)
	{
		int i = 0;
		while (db.result->next())
		{
			// Record total expenses
			if (db.result->getDouble("Total_Price") < 0) { total_expenses[i] = db.result->getDouble("Total_Price"); }
			i++;
		}
	}

	// Get total profit
	for (int i = 0; i < 12; i++)
	{
		total_profit[i] = total_income[i] + total_expenses[i];
	}

	// Calculate the year total based on the business data
	for (int i = 0; i < 12; i++)
	{
		year_total[0] += total_income[i];
		year_total[1] += total_expenses[i];
		year_total[2] += total_profit[i];
	}

	// Print out the business report
	for (int i = 0; i < 12; i++)
	{
		cout << "     " << setw(15) << left << months[i]
			<< setw(30) << right << fixed << setprecision(2) << total_income[i]
			<< setw(30) << fixed << setprecision(2) << total_expenses[i]
			<< setw(30) << fixed << setprecision(2) << total_profit[i] << "\n\n";
	}

	cout << "----------------------------------------------------------------------------------------------------------------\n\n";
	cout << "     " << setw(15) << left << "Total (RM) "
		<< setw(30) << right << fixed << setprecision(2) << year_total[0]
		<< setw(30) << fixed << setprecision(2) << year_total[1]
		<< setw(30) << fixed << setprecision(2) << year_total[2] << "\n\n";

	cout << "================================================================================================================\n\n";

	cout << "\n\n";
	cout << "     ";
	cout << selected << "Generate Bar Chart" << RESET << endl;

	bool exit = false;

	do
	{
		switch (_getch())
		{
		case KEY_ENTER:
			Selected_GenerateBarChart(year);
			exit = true;
			break;
		case KEY_ESCAPE:
			exit = true;
			break;
		}
	} while (!exit);
}

void HR_Menu::Selected_Transaction_Record(string year) {
	system("cls");
	int selected = 0;
	Sql_DB db;
	string months[12] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

	cout << "\n\n";
	cout << "                                            HMS Transaction Record\n\n"
		<< "Year: " << year << endl
		<< "================================================================================================================\n"
		<< "     " << setw(15) << left << "Trans. ID"
		<< setw(30) << right << "Transaction Description"
		<< setw(30) << "Date"
		<< setw(30) << "Total Price" << "\n"
		<< "     " << setw(15) << left << ""
		<< setw(30) << right << "       "
		<< setw(30) << "       "
		<< setw(30) << "(RM)   " << "\n"
		<< "----------------------------------------------------------------------------------------------------------------\n";

	// Print out the Transaction Record
	db.PrepareStatement("SELECT * FROM transaction_record WHERE Date LIKE '" + year + "%'");
	db.QueryResult();

	double total_income = 0;
	double total_expenses = 0;

	while (db.result->next()) {
		cout << "     " << setw(15) << left << db.result->getInt("Transaction_Id")
			<< setw(30) << right << db.result->getString("Description")
			<< setw(30) << db.result->getString("Date")
			<< setw(30) << fixed << setprecision(2) << db.result->getDouble("Total_Price") << "\n";

		if (db.result->getDouble("Total_Price") > 0) { total_income += db.result->getDouble("Total_Price"); }
		else { total_expenses -= db.result->getDouble("Total_Price"); }
	}


	cout << "\n----------------------------------------------------------------------------------------------------------------\n\n";
	cout << "     " << setw(15) << left << "Total (RM) "
		<< setw(30) << right << " "
		<< setw(60) << " "
		<< setw(30) << fixed << setprecision(2) << total_income - total_expenses << "\n\n";

	cout << "================================================================================================================\n\n";

	cout << "\n\n";
	cout << "     ";
	cout << selected << "> Save as pdf" << RESET << endl;

	bool exit = false;

	auto to_wstring = [](string input) -> wstring {
		wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
		return converter.from_bytes(input);
		};

	auto c_to_string = [](wstring input) -> string {
		wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
		return converter.to_bytes(input);
		};

	auto format_column = [](wstring& text, int width) -> wstring {
		std::wstringstream ss;
		if (text.length() > width) {
			ss << text.substr(0, width - 3) << L"..."; // Truncate and add ellipsis if too long
		}
		else {
			ss << std::setw(width) << std::left << text; // Pad if too short } return ss.str();
		}

		return ss.str();
		};

	auto format_price = [](double price) -> wstring {
		std::wstringstream ss;
		ss << std::fixed << std::setprecision(2) << price;
		return ss.str();

		};

	do
	{
		switch (_getch())
		{
		case KEY_ENTER:
			try {
				PDFlib p;
				// wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;

				if (p.begin_document(to_wstring("transaction_record.pdf"), to_wstring("")) == -1) {
					throw runtime_error(c_to_string(p.get_errmsg()));
				}

				// p.set_info("Creator", "Your Name");
				p.set_info(to_wstring("Title"), to_wstring("Transaction Record"));
				p.begin_page_ext(595, 842, to_wstring(""));
				int font = p.load_font(to_wstring("Helvetica"), to_wstring("winansi"), to_wstring(""));

				// Set up your database connection and retrieve records here
				Sql_DB db;
				db.PrepareStatement("SELECT * FROM transaction_record WHERE Date LIKE '" + year + "%'");
				db.QueryResult();

				// Add the header
				p.setfont(font, 10);
				p.show_xy(to_wstring("                                            HMS Transaction Record"), 50, 800);
				p.show_xy(to_wstring("Year: " + year), 50, 780);
				p.show_xy(to_wstring("===================================================================================="), 50, 760);
				p.show_xy(to_wstring("Trans. ID        Transaction Description                                                        Date                       Total Price (RM)"), 50, 740);
				p.show_xy(to_wstring("===================================================================================="), 50, 720);

				// Print out the Transaction Record
				int y = 700;

				while (db.result->next()) {
					wstring trans_id = to_wstring(to_string(db.result->getInt("Transaction_Id")));
					wstring description = to_wstring(db.result->getString("Description"));
					wstring date = to_wstring(db.result->getString("Date"));
					wstring total_price = format_price(db.result->getDouble("Total_Price"));

					// Format each column
					wstring trans_id_col = format_column(trans_id, 5);
					wstring description_col = format_column(description, 30);
					wstring date_col = format_column(date, 10);
					wstring total_price_col = format_column(total_price, 10);

					// Calculate positions for each column 
					int x = 50; p.show_xy(format_column(trans_id, 5).c_str(), x, y);
					x += 65;
					p.show_xy(format_column(description, 30).c_str(), x, y);
					x += 260;
					p.show_xy(format_column(date, 10).c_str(), x, y);
					x += 100;
					p.show_xy(format_column(total_price, 10).c_str(), x, y);

					y -= 20;

				}

				// Add the total
				y -= 20;
				p.show_xy(to_wstring("===================================================================================="), 50, y);
				y -= 20;
				p.show_xy(to_wstring("Total (RM)                                                                                                                                        ") + format_price(total_income - total_expenses), 50, y);

				p.end_page_ext(to_wstring(""));
				p.end_document(to_wstring(""));
			}
			catch (PDFlib::Exception& ex) {
				cerr << "PDFlib Exception occurred in file " << __FILE__ << " at line " << __LINE__ << ":" << endl;
				cerr << "[" << ex.get_errnum() << "] " << c_to_string(ex.get_apiname()) << ": "
					<< c_to_string(ex.get_errmsg()) << endl;
			}
			exit = true;
			break;

		case KEY_ESCAPE:
			exit = true;
			break;
		}
	} while (!exit);

}

void HR_Menu::Selected_GenerateBarChart(string year)
{
	system("cls");
	Sql_DB db;

	const int HEIGHT = 12, MONTHS = 12;
	double amount[12] = {};
	string months[MONTHS] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

	// Get business data
	db.PrepareStatement("SELECT MONTH(Date) AS Month, SUM(Total_Price) AS Total_Price FROM transaction_record WHERE Date LIKE '" + year + "%' GROUP BY MONTH(Date)");
	db.QueryResult();

	if (db.result)
	{
		int i = 0;
		while (db.result->next())
		{
			amount[i] = db.result->getDouble("Total_Price");
			i++;
		}
	}

	// Find the maximum absolute price to determine the chart height
	double maxPrice = 0;
	for (int i = 0; i < MONTHS; i++) {
		if (abs(amount[i]) > maxPrice) {
			maxPrice = abs(amount[i]);
		}
	}

	// Calculate the initial step size
	double stepSize = maxPrice / HEIGHT;

	// Find the appropriate magnitude for rounding, used to adjust the step size (Avoid uneven step value)
	int magnitude = pow(10, floor(log10(stepSize)));

	// Round the step size up to the nearest nice number
	stepSize = ceil(stepSize / (magnitude / 2.0)) * (magnitude / 2.0);

	// Recalculate maxPrice based on the rounded step size, which always slightly higher than the maxPrice
	maxPrice = stepSize * HEIGHT;


	//Chart Tittle
	cout << "\n\n";
	cout << "---------------------------------------YEARS TRANSACTION REPORT--------------------------------------\n\n"
		<< "=====================================================================================================\n"
		<< "Years: 2024\n\n";

	// Illustrate the bar chart from top to bottom
	cout << "  Price(RM)\n"
		<< "        ^ \n";

	for (int i = HEIGHT; i >= 1; i--)
	{
		int height = i * stepSize; // In this case, the initial i is 12, which the current height is the highest number
		cout << setw(7) << height << " |";

		for (int j = 0; j < MONTHS; j++)
		{
			if (abs(amount[j]) >= height)
			{
				cout << (amount[j] >= 0 ? "  " + GREEN_BG + "   " + RESET + "  " : "  " + RED_BG + "   " + RESET + "  "); // Print the bar chart when the current value is higher than the height
			}
			else if (amount[j] > 0 && i == 1 && amount[j] < stepSize)
			{
				cout << "  " + GREEN_BG + "   " + RESET + "  ";  // Show green for positive values less than stepSize in the lowest row
			}
			else if (amount[j] < 0 && i == 1 && abs(amount[j]) < stepSize)
			{
				cout << "  " + RED_BG + "   " + RESET + "  ";  // Show red for negative values less than stepSize in the lowest row
			}
			else { cout << "       "; } // Print nothing when it is empty
		}
		cout << endl;
	}

	// Print the x-axis
	cout << "      0 +";
	for (int i = 0; i < MONTHS; i++) {
		cout << "-------";
	}
	cout << "> Month";
	cout << endl;

	// Print the month labels
	cout << "         ";
	for (int i = 0; i < MONTHS; i++)
	{
		cout << setw(2) << left << "  ";
		cout << setw(3) << left << months[i] << "  ";
	}
	cout << endl;

	// Print the business data in column
	cout << "\n\n";
	cout << " <------------x---------x---------x---------x---------x---------x---------x---------x---------x---------x---------x---------x--------->\n";
	cout << " |" << WHITE_BG << BLACK_TEXT << " Month      " << RESET;
	for (int i = 0; i < MONTHS; i++)
	{
		cout << "| " << setw(8) << months[i];
	}

	cout << "|\n";
	cout << " |------------x---------x---------x---------x---------x---------x---------x---------x---------x---------x---------x---------x---------|\n";
	cout << " |" << WHITE_BG << BLACK_TEXT << " Amount(RM) " << RESET;
	for (int i = 0; i < MONTHS; i++)
	{
		cout << "| ";
		cout << (amount[i] > 0 ? GREEN_TEXT : RED_TEXT) << setw(8) << right << amount[i] << RESET;
	}
	cout << "|\n";
	cout << " <------------x---------x---------x---------x---------x---------x---------x---------x---------x---------x---------x---------x--------->\n";

	_getch();
}
/*====================================================================================================================*/



/**********************************************************************************************************************/
//Nurse Menu function
int Nurse_Menu::Operations_Menu(Nurse usedAccount)
{
	int option = 0, optionStart = 0, optionEnd = 5;
	string selected = BWHITE_TEXT + BOLD;
	string not_selected = GRAY_TEXT;
	string query; //Store the query for database operations


	cout << BRED_BG << YELLOW_TEXT << RAPID_BLINK << BOLD
		<< "                                                                                                                 " << endl
		<< "                                            NURSE OPERATIONS MENU                                                " << endl
		<< "                                                                                                                 " << endl
		<< RESET << endl;
	cout << "\n\n\n"; //6

	do
	{

		cout << (option == 0 ? selected + ">" : not_selected) << "         Manage Patient Registrations" << RESET << "\n\n"
			<< (option == 1 ? selected + ">" : not_selected) << "         Update Patient Information" << RESET << "\n\n"
			<< (option == 2 ? selected + ">" : not_selected) << "         View Patient List" << RESET << "\n\n"
			<< (option == 3 ? selected + ">" : not_selected) << "         Manage Ward" << RESET << "\n\n"
			<< (option == 4 ? selected + ">" : not_selected) << "         Patient Discharge Processing" << RESET << "\n\n\n\n\n" //15
			<< (option == 5 ? selected + ">" : not_selected) << "         Sign Out" << RESET << endl
			<< "\n\n\n" << BRED_BG
			<< "                                                                                                                 " << RESET << "\n\n"; //19

		cout << "Current Staff\n"
			<< "-----------------\n"
			<< "Name: " << usedAccount.Name << endl
			<< "Staff ID: " << usedAccount.Staff_Id << (usedAccount.Is_Manager() ? " (Manager)" : "") << endl;//23

		switch (_getch())
		{
		case KEY_UP:
			if (option > optionStart)
			{
				option--;
			}
			else
			{
				option = optionEnd;
			}
			break;

		case KEY_DOWN:
			if (option < optionEnd)
			{
				option++;
			}
			else
			{
				option = optionStart;
			}
			break;
		case KEY_ENTER:
			return option;
			break;
		}
		Erase_Lines(0, 23);

	} while (true);
}

string Nurse_Menu::Get_List(string query, string first_column, string third_column, string tittle, string additionalOption)
{
	system("cls");
	Sql_DB db;
	char search[31] = {};
	bool selecting = true;
	int option = 0, optionStart = 0, delete_line = 0, optionView = 0;
	string selected = BWHITE_TEXT + BOLD;
	string not_selected = GRAY_TEXT;
	string paid = RESET + GREEN_TEXT;
	string unpaid = RESET + RED_TEXT;

	cout << BRED_BG << YELLOW_TEXT << BOLD
		<< "                                                                                                                 " << endl
		<< tittle << endl
		<< "                                                                                                                 " << endl
		<< RESET << endl;
	cout << "\n"; //5

	cout << "------------------------------------------------------------------------------------------" << endl;
	cout << "|     " << first_column << blank(12, first_column) << "|          Name" << blank(40, "Name") << "|    " << third_column << blank(15, third_column) << "|          " << endl;
	cout << "|                 " << "|                                                  " << "|                   " << "|\n";
	cout << "------------------------------------------------------------------------------------------" << endl; //11


	do
	{
		int optionEnd = Get_RowCount(query);

		//List Menu
		string* id = new string[optionEnd];
		string* name = new string[optionEnd];
		string* third_column_showed = new string[optionEnd];

		//Get the patient's id, staff's name and status in patient
		db.PrepareStatement(query);
		db.QueryResult();

		delete_line = 0;

		if (db.statement)
		{
			int i = 0;
			while (db.result->next())
			{
				id[i] = db.result->getString(first_column);
				name[i] = db.result->isNull("Name") ? "N/A" : db.result->getString("Name");
				third_column_showed[i] = db.result->isNull(third_column) ? "N/A" : db.result->getString(third_column);
				i++;
				delete_line += 2;
			}
		}

		for (int i = 0; i < optionEnd; i++)
		{
			cout << (i == option ? selected : not_selected) << "|     " << id[i] << blank(12, id[i])
				<< "|          " << name[i] << blank(40, name[i]) << "|    "
				<< (third_column_showed[i] == "Y" ? paid : "") << (third_column_showed[i] == "N" ? unpaid : "") << third_column_showed[i] << RESET << blank(15, third_column_showed[i]) << (i == option ? selected : not_selected) << "|          \n";
			cout << "|                 " << "|                                                  " << "|                   " << "|\n";
		}
		cout << RESET << "==========================================================================================\n\n" << endl; //14 , erase_line where 9 - 2(print) = 7
		
		if (additionalOption == "Add new patient")
		{
			cout << (option == optionEnd ? selected + "> " : not_selected) << additionalOption << RESET << endl;
		}
		else if (additionalOption == "Search")
		{
			cout << (option == optionEnd ? selected : not_selected) << "      Search: " << search << RESET << endl;
		}
		else
		{
			cout << endl;
		}

		switch (_getch())
		{
			case KEY_UP:
				if (option > optionStart)
				{
					option--;
				}
				else
				{
					if (additionalOption.empty())
					{
						option = optionEnd - 1;
					}
					else
					{
						option = optionEnd;
					}
				}
				break;

			case KEY_DOWN:
				if (additionalOption.empty() ? option < optionEnd - 1 :  option < optionEnd)
				{
					option++;
				}
				else
				{
					option = optionStart;
				}
				break;
			case KEY_ESCAPE:
				delete[]id;
				delete[]name;
				delete[]third_column_showed;
				return "";
				break;
			case KEY_ENTER:

				if (additionalOption.empty())
				{
					delete[]name;
					delete[]third_column_showed;
					return id[option];
				}

				if (additionalOption == "Discharge") {
					// Check patient already have pharmacy record or not
					string q1 = "SELECT * FROM pharmacy_record WHERE Record_Id = '";
					string q2 = "'";
					if (Get_RowCount(q1 + id[option] + q2) == 0) {
						Erase_Lines(0, 1);
						cout << "This patient dont have pharmacy record yet!\n";
						_getch();
					}
					else {
						return id[option];
					}
				}

				if (additionalOption == "Add new patient")
				{
					if (third_column_showed[option] == "Admitted")
					{
						Erase_Lines(0, 1);
						cout << "This patient already admitted!\n";
						_getch();

					}
					else if (option == optionEnd)
					{
						delete[]id;
						delete[]name;
						delete[]third_column_showed;
						return "Add";
					}
					else
					{
						Erase_Lines(0, 1);
						cout << selected + "> " << "Admitted this patient to the hospital?\n";
						switch (_getch())
						{
						case KEY_ENTER:
							delete[]name;
							delete[]third_column_showed;
							return id[option];
							break;
						case KEY_ESCAPE:
							option = 0;
							break;
						}
					}
				}
				else if (additionalOption == "Search")
				{
					if (option == optionEnd)
					{
						int size = sizeof(search) / sizeof(search[0]);
						bool exit = false;

						while (!exit)
						{
							Erase_Lines(0, 4 + delete_line);
							for (int i = 0; i < optionEnd; i++)
							{
								cout << (i == option ? selected : not_selected) << "|     " << id[i] << blank(12, id[i])
									<< "|          " << name[i] << blank(40, name[i]) << "|    "
									<< third_column_showed[i] << RESET << blank(15, third_column_showed[i]) << (i == option ? selected : not_selected) << "|          \n";
								cout << "|                 " << "|                                                  " << "|                   " << "|\n";
							}
							cout << RESET << "==========================================================================================\n\n" << endl; //14 , erase_line where 9 - 2(print) = 7
							cout << (option == optionEnd ? selected + ">" : not_selected) << "      Search: " << search << endl;

							Selected_Input(exit, size, search);
						}

						if (search[0] != '\0')
						{
							query = "SELECT * FROM patient WHERE Name LIKE '" + string(search) + "%'";
							option = 0;
						}
						else
						{
							query = "SELECT * FROM patient";
							option = 0;
						}
					}
					else
					{
						Erase_Lines(0, 1);
						return id[option];
					}
				}
				break;
		}
		Erase_Lines(0, 4 + delete_line);
		delete[]id;
		delete[]name;
		delete[]third_column_showed;
	} while (selecting);
}

void Nurse_Menu::Patient_Menu(Patient patient)
{
	cout << "Patient Information\n";
	cout << "=======================================================================================\n\n";
	cout << "  Name             : " << patient.Name << RESET << endl << endl;
	cout << "  DOB              : " << patient.DOB << RESET << endl << endl;
	cout << "  Gender           : " << patient.Gender << RESET << endl << endl;
	cout << "  Race             : " << patient.Race << RESET << endl << endl;
	cout << "  Religion         : " << patient.Religion << RESET << endl << endl;
	cout << "  Address          : " << patient.Address << RESET << endl << endl;
	cout << "  IC               : " << patient.IC << RESET << endl << endl;
	cout << "  Contact Number   : " << patient.Contact_Number << RESET << endl << endl;
	cout << "  Email            : " << patient.Email << RESET << endl << endl;
	cout << "=======================================================================================\n\n";
}

void Nurse_Menu::PatientRecord_Menu(Patient patient)
{
	Sql_DB db;

	string record_no = "";

	do
	{
		string query = "SELECT PR.Record_Id, P.Name, PR.Payment_Status FROM patient P JOIN patient_record PR ON P.Patient_Id = PR.Patient_Id WHERE PR.Patient_Id = '" + string(patient.Patient_Id) + "'";
		record_no = Get_List(query, "Record_Id", "Payment_Status",
			                 "                                               PATIENT'S RECORDS                                                 ", "");

		Patient_Record pr;
		pr.Show_PatientRecord(record_no);

	} while (!record_no.empty());
}

void Nurse_Menu::PatientAdd_Option_Menu(int option, string selected, string not_selected, Patient& patient)
{
	Erase_Lines(0, 31);
	selected += "> ";

	cout << "New Patient Information\n";
	cout << "=======================================================================================\n\n";
	cout << (option == 0 ? selected : not_selected) << "  Name             : " << patient.Name << RESET << endl << endl;
	cout << (option == 1 ? selected : not_selected) << "  DOB              : " << patient.DOB << RESET << endl << endl;
	cout << (option == 2 ? selected : not_selected) << "  Gender           : " << patient.Gender << RESET << endl << endl;
	cout << (option == 3 ? selected : not_selected) << "  Race             : " << patient.Race << RESET << endl << endl;
	cout << (option == 4 ? selected : not_selected) << "  Religion         : " << patient.Religion << RESET << endl << endl;
	cout << (option == 5 ? selected : not_selected) << "  Address          : " << patient.Address << RESET << endl << endl;
	cout << (option == 6 ? selected : not_selected) << "  IC               : " << patient.IC << RESET << endl << endl;
	cout << (option == 7 ? selected : not_selected) << "  Contact Number   : " << patient.Contact_Number << RESET << endl << endl;
	cout << (option == 8 ? selected : not_selected) << "  Email            : " << patient.Email << RESET << endl << endl;
	cout << (option == 9 ? selected : not_selected) << "  Appointment with : " << patient.Appoint_With << RESET << endl << endl;
	cout << (option == 10 ? selected : not_selected) << "  Appointment date : " << patient.Appoint_Date << RESET << endl << endl;
	cout << "=======================================================================================\n\n";
	cout << (option == 11 ? selected : not_selected) << "  Add the Patient\n\n\n" << RESET;//31
}

void Nurse_Menu::Update_Option_Menu(int option, string selected, string not_selected, Patient& patient)
{
	Erase_Lines(0, 27);
	selected += "> ";

	cout << "Update Patient Information\n";
	cout << "=======================================================================================\n\n";
	cout << (option == 0 ? selected : not_selected) << "  Name             : " << patient.Name << RESET << endl << endl;
	cout << (option == 1 ? selected : not_selected) << "  DOB              : " << patient.DOB << RESET << endl << endl;
	cout << (option == 2 ? selected : not_selected) << "  Gender           : " << patient.Gender << RESET << endl << endl;
	cout << (option == 3 ? selected : not_selected) << "  Race             : " << patient.Race << RESET << endl << endl;
	cout << (option == 4 ? selected : not_selected) << "  Religion         : " << patient.Religion << RESET << endl << endl;
	cout << (option == 5 ? selected : not_selected) << "  Address          : " << patient.Address << RESET << endl << endl;
	cout << (option == 6 ? selected : not_selected) << "  IC               : " << patient.IC << RESET << endl << endl;
	cout << (option == 7 ? selected : not_selected) << "  Contact Number   : " << patient.Contact_Number << RESET << endl << endl;
	cout << (option == 8 ? selected : not_selected) << "  Email            : " << patient.Email << RESET << endl << endl;
	cout << "=======================================================================================\n\n";
	cout << (option == 10 ? selected : not_selected) << "  Confirm\n\n\n" << RESET;//27
}

void Nurse_Menu::ChangeOfNurse(string ward_no)
{
	Erase_Lines(0, 2);
	Sql_DB db;
	Nurse nr;
	string query = "SELECT * FROM staff WHERE Role = 'Nurse'";
	int option = 0;
	int optionEnd = Get_RowCount(query) + 1;
	bool selecting = true;

	string* id = new string[optionEnd];
	db.PrepareStatement(query);
	db.QueryResult();

	if (db.statement)
	{
		int i = 1;
		while (db.result->next())
		{
			id[i] = db.result->getString("Staff_Id");
			i++;
		}
	}

	do
	{
		cout << ">      Change To: " << "< " << id[option] << " >" << "\n\n";

		switch (_getch())
		{
			case KEY_LEFT:
			{
				if (option > 0) { option--; }
				break;
			}
			case KEY_RIGHT:
			{
				if (option < optionEnd - 1) { option++; }
				break;
			}
			case KEY_ESCAPE:
			{
				selecting = false;
				break;
			}
			case KEY_ENTER:
			{
				nr.UpdateNurseOnWard(id[option], ward_no);
				selecting = false;
				break;
			}
		}
		Erase_Lines(0, 2);


	} while (selecting);

}

void Nurse_Menu::ManageBedAdmission(string ward_no)
{
	Sql_DB db;
	Nurse nr;
	int option = 0, totalPatient = 0, optionPatient = 0;
	bool BedSelect = true;
	string selected = BWHITE_TEXT + BOLD;
	string not_selected = GRAY_TEXT;

	do
	{
		string bed_no = Get_List("SELECT B.Bed_Id, P.Name, B.Admission_Date FROM bed B LEFT JOIN patient P ON B.Patient_Id = P.Patient_Id WHERE B.Ward_Id = '" + ward_no + "'", "Bed_Id", "Admission_Date",
			"                                                 MANAGE BED ADMISSION                                            ", "");

		if (!bed_no.empty())
		{
			bool selecting = true;
			db.PrepareStatement("SELECT Availability FROM bed WHERE Bed_Id = '" + bed_no + "'");
			db.QueryResult();
			db.result->next();
			bool Available = (db.result->getString("Availability") == "Y") ? true : false;
			totalPatient = Get_RowCount("SELECT P.Patient_Id FROM patient P LEFT JOIN Bed B ON P.Patient_Id = B.Patient_Id WHERE P.Status = 'Admitted' AND B.patient_id IS NULL") + 1;
			string* id = new string[totalPatient];
			char date[11] = {};
			db.PrepareStatement("SELECT P.Patient_Id FROM patient P LEFT JOIN Bed B ON P.Patient_Id = B.Patient_Id WHERE P.Status = 'Admitted' AND B.patient_id IS NULL");
			db.QueryResult();
			string test;

			if (db.statement)
			{
				int i = 1;
				while (db.result->next())
				{
					id[i] = db.result->getString("Patient_Id");
					i++;
				}
			}

			do
			{
				if (!Available)
				{
					cout << "This bed is used!";
					_getch();
					selecting = false;
				}
				else
				{
					cout << (option == 0 ? selected : not_selected) << "      Patient_Id: " << id[optionPatient] << RESET << endl;
					cout << (option == 1 ? selected : not_selected) << "      Admission Date: " << date << RESET << endl;
					cout << (option == 2 ? selected + ">" : not_selected) << "      Confirm " << RESET << endl;

					switch (_getch())
					{
					case KEY_UP:
					{
						if (option > 0) { option--; }
						break;
					}
					case KEY_DOWN:
					{
						if (option < 2) { option++; }
						break;
					}
					case KEY_ESCAPE:
					{
						selecting = false;
						break;
					}
					case KEY_ENTER:
					{
						bool exit = false;

						if (option == 0)
						{
							do
							{
								Erase_Lines(0, 3);
								cout << (option == 0 ? selected + ">" : not_selected) << "      Patient_Id: " << "< " << id[optionPatient] << " >" << RESET << endl;
								cout << (option == 1 ? selected : not_selected) << "      Admission Date: " << date << RESET << endl;
								cout << (option == 2 ? selected : not_selected) << "      Confirm " << RESET << endl;

								switch (_getch())
								{
								case KEY_LEFT:
									if (optionPatient > 0) { optionPatient--; }
									break;
								case KEY_RIGHT:
									if (optionPatient < totalPatient - 1) { optionPatient++; }
									break;
								case KEY_ENTER:
									exit = true;
									break;
								}
							} while (!exit);

						}
						else if (option == 1)
						{
							int size = sizeof(date) / sizeof(date[0]);

							while (!exit)
							{
								Erase_Lines(0, 3);
								cout << (option == 0 ? selected : not_selected) << "      Patient_Id: " << id[optionPatient] << RESET << endl;
								cout << (option == 1 ? selected + ">" : not_selected) << "      Admission Date: " << date << RESET << endl;
								cout << (option == 2 ? selected : not_selected) << "      Confirm " << RESET << endl;
								Selected_Input(exit, size, date);
							}
						}
						else if (option == 2)
						{
							nr.BedAdmit(bed_no, id[optionPatient], date);
							_getch();
							option = 0;
							selecting = false;
						}
						break;
					}
					}
					Erase_Lines(0, 3);
				}
				
			} while (selecting);
			optionPatient = 0;
		}
		else
		{
			BedSelect = false;
		}

	} while (BedSelect);
	

}

void Nurse_Menu::Discharge_Menu(char date[], string payment_method, string card_no, int option)
{
	Erase_Lines(0, 7);
	string selected = BWHITE_TEXT + BOLD + ">";
	string not_selected = GRAY_TEXT;

	cout << (option == 0 ? selected : not_selected) << "  Discharge Date   : " << date << RESET << endl;
	cout << (option == 1 ? selected : not_selected) << "  Payment Method   : " << payment_method << RESET << endl;
	cout << (option == 2 ? selected : not_selected) << "  Card Number      : " << card_no << RESET << endl << endl;
	cout << (option == 3 ? selected + "> " : not_selected) << "  Confirm Discharge" << RESET << endl << endl;
	cout << endl; //24
}

void Nurse_Menu::Selected_Appointment_Time(string patient_no, string doctor_no) {
	system("cls");
	Sql_DB db;
	cout << "Appointment Time Schedule\n"
		<< "=======================================================================================\n\n";

	// Get time schedule for that doctor on that room
	db.PrepareStatement("SELECT Room_Id, Time_from, Time_to, CASE WHEN Time_from < Time_to THEN (HOUR(Time_to) * 60 + MINUTE(Time_to) - HOUR(Time_from) * 60 - MINUTE(Time_from)) / 30 ELSE ((HOUR(Time_to) * 60 + MINUTE(Time_to) + 24 * 60) - (HOUR(Time_from) * 60 + MINUTE(Time_from))) / 30 END AS num_appointments FROM room WHERE Staff_Id = ?");
	db.statement->setString(1, doctor_no);
	db.QueryResult();
	db.result->next();

	// printTimeSlots(db.result->getString("Time_from"), db.result->getString("Time_to"));

	struct Time {
		int hour;
		int minute;
	};

	auto parseTime = [](const std::string& timeStr) -> Time {
		int hour, minute;
		char colon;
		std::istringstream timeStream(timeStr);
		timeStream >> hour >> colon >> minute;
		return { hour, minute };
		};

	auto timeToMinutes = [](const Time& time) -> int {
		return time.hour * 60 + time.minute;
		};

	Time startTime = parseTime(db.result->getString("Time_from"));
	Time endTime = parseTime(db.result->getString("Time_to"));

	int startMinutes = timeToMinutes(startTime);
	int endMinutes = timeToMinutes(endTime);

	// Handle the case where end time is on the next day
	if (endMinutes < startMinutes) {
		endMinutes += 24 * 60;  // Add 24 hours
	}

	int slot = 0;
	string q1 = "[slot ";
	string q2 = "]";

	std::cout << "Time slots:" << std::endl;
	for (int currentMinutes = startMinutes; currentMinutes <= endMinutes; currentMinutes += 30) {
		int currentHour = (currentMinutes / 60) % 24;
		int currentMinute = currentMinutes % 60;
		// Corrected printing logic with proper formatting and alignment
		std::cout << "     " << q1 + std::to_string(slot) + q2 << "     "
			<< std::setfill('0') << std::setw(2) << currentHour << ":"
			<< std::setfill('0') << std::setw(2) << currentMinute << std::endl;
		slot++;
	}
}

void Nurse_Menu::Selected_PatientAdmit(string patient_no)
{
	Patient patient;
	Nurse nurse;
	Sql_DB db;
	bool selecting = true;
	Erase_Lines(0, 1);
	int option = 0, option_AppointWith = 0, appointment_slot = 0;
	char input[3] = {};
	bool isConfirm = false;
	string selected = BWHITE_TEXT + BOLD;
	string not_selected = GRAY_TEXT;


	do
	{
		cout << (option == 0 ? selected : not_selected) << "Have appoinment with : " << patient.Appoint_With << endl;
		cout << (option == 1 ? selected : not_selected) << "Appointment date     : " << patient.Appoint_Date << endl;
		cout << (option == 2 ? selected + "> " : not_selected) << "Confirm" << endl;

		switch (_getch())
		{
			case KEY_UP:
			{
				if (option > 0)
				{
					option--;
				}
				else
				{
					option = 2;
				}
				break;
			}
			case KEY_DOWN:
			{
				if (option < 2)
				{
					option++;
				}
				else
				{
					option = 0;
				}
				break;
			}
			case KEY_ESCAPE:
			{
				selecting = false;
				break;
			}
			case KEY_ENTER:
			{
				if (option == 0)
				{
					int size = sizeof(patient.Appoint_With) / sizeof(patient.Appoint_With[0]);
					bool exit = false;

					//Get how many doctor available in the database
					string query = "SELECT * FROM Staff WHERE Role = 'Doctor'";
					int row_count = Get_RowCount(query) + 1;
					string* Doctor_Id = new string[row_count]; //dynamic array
					string* Doctor_Name = new string[row_count];

					//Get the Doctor's id
					db.PrepareStatement(query);
					db.QueryResult();

					if (db.result)
					{
						int i = 1;
						while (db.result->next())
						{
							Doctor_Id[i] = db.result->getString("Staff_Id");
							Doctor_Name[i] = db.result->getString("Name");
							i++;
						}
					}

					while (!exit)
					{
						Erase_Lines(0, 3);
						string showed = "  < " + Doctor_Name[option_AppointWith] + " >";
						strcpy_s(patient.Appoint_With, size, showed.c_str());

						cout << (option == 0 ? selected : not_selected) << "Have appoinment with : " << showed << endl;
						cout << (option == 1 ? selected : not_selected) << "Appointment date     : " << patient.Appoint_Date << endl;
						cout << (option == 2 ? selected + "> " : not_selected) << "Confirm" << endl;

						inputOperations::Available_Input(exit, Doctor_Id, row_count, option_AppointWith, patient.Appoint_With, size);
					}
					strcpy_s(patient.Appoint_With, Doctor_Id[option_AppointWith].c_str()); //Insert manager's staff_id into the Register object

					//delete dynamic array
					delete[] Doctor_Id;
					delete[] Doctor_Name;
				}
				else if (option == 1) {
					int size = sizeof(patient.Appoint_Date) / sizeof(patient.Appoint_Date[0]);
					bool exit = false;

					while (!exit)
					{
						Erase_Lines(0, 3);
						cout << (option == 0 ? selected : not_selected) << "Have appoinment with : " << patient.Appoint_With << endl;
						cout << (option == 1 ? selected + "> " : not_selected) << "Appointment date     : " << patient.Appoint_Date << endl;
						cout << (option == 2 ? selected + "> " : not_selected) << "Confirm" << endl;
						inputOperations::Selected_Input(exit, size, patient.Appoint_Date);
					}
					break;
				}
				else if (option == 2)
				{
					auto isValidDate = [](const std::string& dateOfBirth) -> bool {
						const regex pattern(R"(^\d{4}-(0[1-9]|1[0-2])-(0[1-9]|[1-2][0-9]|3[0-1])$)");
						return regex_match(dateOfBirth, pattern);
						};


					bool valid = true;
					try
					{
						if (isValidDate(patient.Appoint_Date) && patient.Appoint_With[0] != '\0') {
							nurse.AdmitFormerPatient(patient_no, patient.Appoint_With, patient.Appoint_Date);
							selecting = false;
						}
						else if (patient.Appoint_With[0] == '\0') {
							cout << "\n\nMissing doctor id.\n";
							_getch();
							Erase_Lines(0, 3);
							valid = false;
						}
						else if(!isValidDate(patient.Appoint_Date)) {
							cout << "\n\nInvalid date format.\n";
							_getch();
							Erase_Lines(0, 3);
							valid = false;
						}
						
					}
					catch (sql::SQLException& e)
					{
						cout << "Invalid input.";
						valid = false;
						_getch();
					}
					catch (exception e) { valid = false; selecting = false;}

					if (valid == true)
					{
						selecting = false;	
						cout << "Successfully added the patient!";
						_getch();
					}
				}
				break;
			}
		}
		Erase_Lines(0, 3);
	} while (selecting);
}

void Nurse_Menu::Selected_PatientAdd(string patient_no)
{
	Nurse nurse;
	Patient patient;
	system("cls");
	Sql_DB db;
	int option = 0, optionStart = 0, optionEnd = 11, option_gender = 0, option_race = 0, option_religion = 0, option_AppointWith = 0;
	bool selecting = true;

	string selected = BWHITE_TEXT + BOLD;
	string not_selected = GRAY_TEXT;
	string message;


	do
	{
		cout << "New Patient Information\n";
		cout << "=======================================================================================\n\n";
		cout << (option == 0 ? selected : not_selected) << "  Name             : " << patient.Name << RESET << endl << endl;
		cout << (option == 1 ? selected : not_selected) << "  DOB              : " << patient.DOB << RESET << endl << endl;
		cout << (option == 2 ? selected : not_selected) << "  Gender           : " << patient.Gender << RESET << endl << endl;
		cout << (option == 3 ? selected : not_selected) << "  Race             : " << patient.Race << RESET << endl << endl;
		cout << (option == 4 ? selected : not_selected) << "  Religion         : " << patient.Religion << RESET << endl << endl;
		cout << (option == 5 ? selected : not_selected) << "  Address          : " << patient.Address << RESET << endl << endl;
		cout << (option == 6 ? selected : not_selected) << "  IC               : " << patient.IC << RESET << endl << endl;
		cout << (option == 7 ? selected : not_selected) << "  Contact Number   : " << patient.Contact_Number << RESET << endl << endl;
		cout << (option == 8 ? selected : not_selected) << "  Email            : " << patient.Email << RESET << endl << endl;
		cout << (option == 9 ? selected : not_selected) << "  Appointment with : " << patient.Appoint_With << RESET << endl << endl;
		cout << (option == 10 ? selected : not_selected) << "  Appointment date : " << patient.Appoint_Date << RESET << endl << endl;
		cout << "=======================================================================================\n\n";
		cout << (option == 11 ? selected : not_selected) << "  Add the Patient\n\n\n" << RESET;//27
		cout << message;

		message = "";


		switch (_getch())
		{
			case KEY_UP:
			{
				if (option > optionStart)
				{
					option--;
				}
				else
				{
					option = optionEnd;
				}
				break;
			}
			case KEY_DOWN:
			{
				if (option < optionEnd)
				{
					option++;
				}
				else
				{
					option = optionStart;
				}
				break;
			}
			case KEY_ESCAPE:
			{
				selecting = false;
				break;
			}
			case KEY_ENTER:
			{
				switch (option)
				{
					case 0:
					{
						int size = sizeof(patient.Name) / sizeof(patient.Name[0]);
						bool exit = false;

						while (!exit)
						{
							PatientAdd_Option_Menu(option, selected, not_selected, patient);
							inputOperations::Selected_Input(exit, size, patient.Name);
						}
						break;
					}
					case 1:
					{
						int size = sizeof(patient.DOB) / sizeof(patient.DOB[0]);
						bool exit = false;

						while (!exit)
						{
							PatientAdd_Option_Menu(option, selected, not_selected, patient);
							inputOperations::Selected_Input(exit, size, patient.DOB);
						}
						break;
					}
					case 2:
					{
						int size = sizeof(patient.Gender) / sizeof(patient.Gender[0]);
						bool exit = false;
						string input_option[2] = { "M", "F" };
						int size_option = sizeof(input_option) / sizeof(input_option[0]);

						while (!exit)
						{
							string showed = "  < " + input_option[option_gender] + " >";
							strcpy_s(patient.Gender, size, showed.c_str());
							PatientAdd_Option_Menu(option, selected, not_selected, patient);
							inputOperations::Available_Input(exit, input_option, size_option, option_gender, patient.Gender, size);
						}
						break;
					}
					case 3:
					{
						int size = sizeof(patient.Race) / sizeof(patient.Race[0]);
						bool exit = false;
						string input_option[4] = { "Chinese", "Malay", "Indian", "Other" };
						int size_option = sizeof(input_option) / sizeof(input_option[0]);

						while (!exit)
						{
							string showed = "  < " + input_option[option_race] + " >";
							strcpy_s(patient.Race, size, showed.c_str());
							PatientAdd_Option_Menu(option, selected, not_selected, patient);
							inputOperations::Available_Input(exit, input_option, size_option, option_race, patient.Race, size);
						}
						break;
					}
					case 4:
					{
						int size = sizeof(patient.Religion) / sizeof(patient.Religion[0]);
						bool exit = false;
						string input_option[4] = { "Buddhism", "Islam", "Hinduism", "Other" };
						int size_option = sizeof(input_option) / sizeof(input_option[0]);

						while (!exit)
						{
							string showed = "  < " + input_option[option_religion] + " >";
							strcpy_s(patient.Religion, size, showed.c_str());
							PatientAdd_Option_Menu(option, selected, not_selected, patient);
							inputOperations::Available_Input(exit, input_option, size_option, option_religion, patient.Religion, size);
						}
						break;
					}
					case 5:
					{
						int size = sizeof(patient.Address) / sizeof(patient.Address[0]);
						bool exit = false;

						while (!exit)
						{
							PatientAdd_Option_Menu(option, selected, not_selected, patient);
							inputOperations::Selected_Input(exit, size, patient.Address);
						}
						break;
					}
					case 6:
					{
						int size = sizeof(patient.IC) / sizeof(patient.IC[0]);
						bool exit = false;

						while (!exit)
						{
							PatientAdd_Option_Menu(option, selected, not_selected, patient);
							inputOperations::Selected_Input(exit, size, patient.IC);
						}
						break;
					}
					case 7:
					{
						int size = sizeof(patient.Contact_Number) / sizeof(patient.Contact_Number[0]);
						bool exit = false;

						while (!exit)
						{
							PatientAdd_Option_Menu(option, selected, not_selected, patient);
							inputOperations::Selected_Input(exit, size, patient.Contact_Number);
						}
						break;
					}
					case 8:
					{
						int size = sizeof(patient.Email) / sizeof(patient.Email[0]);
						bool exit = false;

						while (!exit)
						{
							PatientAdd_Option_Menu(option, selected, not_selected, patient);
							inputOperations::Selected_Input(exit, size, patient.Email);
						}
						break;
					}
					case 9:
					{
						int size = sizeof(patient.Appoint_With) / sizeof(patient.Appoint_With[0]);
						bool exit = false;

						//Get how many doctor available in the database
						string query = "SELECT * FROM Staff WHERE Role = 'Doctor'";
						int row_count = Get_RowCount(query) + 1;
						string* Doctor_Id = new string[row_count]; //dynamic array
						string* Doctor_Name = new string[row_count];

						//Get the Doctor's id
						db.PrepareStatement(query);
						db.QueryResult();

						if (db.result)
						{
							int i = 1;
							while (db.result->next())
							{
								Doctor_Id[i] = db.result->getString("Staff_Id");
								Doctor_Name[i] = db.result->getString("Name");
								i++;
							}
						}

						while (!exit)
						{
							string showed = "  < " + Doctor_Name[option_AppointWith] + " >";
							strcpy_s(patient.Appoint_With, size, showed.c_str());
							PatientAdd_Option_Menu(option, selected, not_selected, patient);
							inputOperations::Available_Input(exit, Doctor_Id, row_count, option_AppointWith, patient.Appoint_With, size);
						}
						strcpy_s(patient.Appoint_With, Doctor_Id[option_AppointWith].c_str()); //Insert manager's staff_id into the Register object

						//delete dynamic array
						delete[] Doctor_Id;
						delete[] Doctor_Name;
						break;
					}
					case 10: {
						int size = sizeof(patient.Appoint_Date) / sizeof(patient.Appoint_Date[0]);
						bool exit = false;

						while (!exit)
						{
							PatientAdd_Option_Menu(option, selected, not_selected, patient);
							inputOperations::Selected_Input(exit, size, patient.Appoint_Date);
						}
						break;
					}
					case 11:
					{
						bool valid = true;
						try
						{
							// Selected_Appointment_Time(patient_no, patient.Appoint_With);
							nurse.AddNewPatient(patient);
						}
						catch (sql::SQLException& e)
						{
							cout << "Invalid input.";
							valid = false;
							_getch();
						}
						catch (exception& e) {
							if (string(e.what()) == "Back") { valid = false; }
						}

						if (valid == true)
						{
							selecting = false;
							cout << "Successfully added the patient!";
							_getch();
						}
						break;
					}
				}
				break;
			}
			
		}

		Erase_Lines(0, 31);
	} while (selecting);

}

void Nurse_Menu::Selected_Update_Patient(string patient_no)
{
	system("cls");
	Nurse nurse;
	Patient patient;
	system("cls");
	Sql_DB db;
	int option = 0, optionStart = 0, optionEnd = 9, option_gender = 0, option_race = 0, option_religion = 0;
	bool selecting = true;

	string selected = BWHITE_TEXT + BOLD;
	string not_selected = GRAY_TEXT;
	string message;

	patient.Patient_Detail("SELECT * FROM patient WHERE Patient_Id = ?", patient_no);

	do
	{
		cout << "Update Patient Information\n";
		cout << "=======================================================================================\n\n";
		cout << (option == 0 ? selected : not_selected) << "  Name             : " << patient.Name << RESET << endl << endl;
		cout << (option == 1 ? selected : not_selected) << "  DOB              : " << patient.DOB << RESET << endl << endl;
		cout << (option == 2 ? selected : not_selected) << "  Gender           : " << patient.Gender << RESET << endl << endl;
		cout << (option == 3 ? selected : not_selected) << "  Race             : " << patient.Race << RESET << endl << endl;
		cout << (option == 4 ? selected : not_selected) << "  Religion         : " << patient.Religion << RESET << endl << endl;
		cout << (option == 5 ? selected : not_selected) << "  Address          : " << patient.Address << RESET << endl << endl;
		cout << (option == 6 ? selected : not_selected) << "  IC               : " << patient.IC << RESET << endl << endl;
		cout << (option == 7 ? selected : not_selected) << "  Contact Number   : " << patient.Contact_Number << RESET << endl << endl;
		cout << (option == 8 ? selected : not_selected) << "  Email            : " << patient.Email << RESET << endl << endl;
		cout << "=======================================================================================\n\n";
		cout << (option == 9 ? selected : not_selected) << "  Confirm\n\n\n" << RESET;//27
		cout << message;

		message = "";


		switch (_getch())
		{
			case KEY_UP:
			{
				if (option > optionStart)
				{
					option--;
				}
				else
				{
					option = optionEnd;
				}
				break;
			}
			case KEY_DOWN:
			{
				if (option < optionEnd)
				{
					option++;
				}
				else
				{
					option = optionStart;
				}
				break;
			}
			case KEY_ESCAPE:
			{
				selecting = false;
				break;
			}
			case KEY_ENTER:
			{
				switch (option)
				{
					case 0:
					{
						int size = sizeof(patient.Name) / sizeof(patient.Name[0]);
						bool exit = false;

						while (!exit)
						{
							Update_Option_Menu(option, selected, not_selected, patient);
							inputOperations::Selected_Input(exit, size, patient.Name);
						}
						break;
					}
					case 1:
					{
						int size = sizeof(patient.DOB) / sizeof(patient.DOB[0]);
						bool exit = false;

						while (!exit)
						{
							Update_Option_Menu(option, selected, not_selected, patient);
							inputOperations::Selected_Input(exit, size, patient.DOB);
						}
						break;
					}
					case 2:
					{
						int size = sizeof(patient.Gender) / sizeof(patient.Gender[0]);
						bool exit = false;
						string input_option[2] = { "M", "F" };
						int size_option = sizeof(input_option) / sizeof(input_option[0]);

						while (!exit)
						{
							string showed = "  < " + input_option[option_gender] + " >";
							strcpy_s(patient.Gender, size, showed.c_str());
							Update_Option_Menu(option, selected, not_selected, patient);
							inputOperations::Available_Input(exit, input_option, size_option, option_gender, patient.Gender, size);
						}
						break;
					}
					case 3:
					{
						int size = sizeof(patient.Race) / sizeof(patient.Race[0]);
						bool exit = false;
						string input_option[4] = { "Chinese", "Malay", "Indian", "Other" };
						int size_option = sizeof(input_option) / sizeof(input_option[0]);

						while (!exit)
						{
							string showed = "  < " + input_option[option_race] + " >";
							strcpy_s(patient.Race, size, showed.c_str());
							Update_Option_Menu(option, selected, not_selected, patient);
							inputOperations::Available_Input(exit, input_option, size_option, option_race, patient.Race, size);
						}
						break;
					}
					case 4:
					{
						int size = sizeof(patient.Religion) / sizeof(patient.Religion[0]);
						bool exit = false;
						string input_option[4] = { "Buddhism", "Islam", "Hinduism", "Other" };
						int size_option = sizeof(input_option) / sizeof(input_option[0]);

						while (!exit)
						{
							string showed = "  < " + input_option[option_religion] + " >";
							strcpy_s(patient.Religion, size, showed.c_str());
							Update_Option_Menu(option, selected, not_selected, patient);
							inputOperations::Available_Input(exit, input_option, size_option, option_religion, patient.Religion, size);
						}
						break;
					}
					case 5:
					{
						int size = sizeof(patient.Address) / sizeof(patient.Address[0]);
						bool exit = false;

						while (!exit)
						{
							Update_Option_Menu(option, selected, not_selected, patient);
							inputOperations::Selected_Input(exit, size, patient.Address);
						}
						break;
					}
					//case 6: // IC cnt be modified/update
					//{
					//	int size = sizeof(patient.IC) / sizeof(patient.IC[0]);
					//	bool exit = false;

					//	while (!exit)
					//	{
					//		Update_Option_Menu(option, selected, not_selected, patient);
					//		inputOperations::Selected_Input(exit, size, patient.IC);
					//	}
					//	break;
					//}
					case 7:
					{
						int size = sizeof(patient.Contact_Number) / sizeof(patient.Contact_Number[0]);
						bool exit = false;

						while (!exit)
						{
							Update_Option_Menu(option, selected, not_selected, patient);
							inputOperations::Selected_Input(exit, size, patient.Contact_Number);
						}
						break;
					}
					case 8:
					{
						int size = sizeof(patient.Email) / sizeof(patient.Email[0]);
						bool exit = false;

						while (!exit)
						{
							Update_Option_Menu(option, selected, not_selected, patient);
							inputOperations::Selected_Input(exit, size, patient.Email);
						}
						break;
					}
					case 9:
					{
						bool valid = true;
						try
						{
							nurse.UpdatePatientInfo(patient);
						}
						catch (sql::SQLException& e)
						{
							cout << "Invalid, something is wrong.";
							valid = false;
							_getch();
						}

						if (valid == true)
						{
							selecting = false;
							cout << "Successfully Update!";
							_getch();
						}
						break;
					}
				}
				break;
			}
		}

		Erase_Lines(0, 27);
	} while (selecting);
}

void Nurse_Menu::Selected_ManageWard(string ward_no)
{
	Sql_DB db;
	int option = 0, optionEnd = 1;
	bool selecting = true;
	string selected = BWHITE_TEXT + BOLD;
	string not_selected = GRAY_TEXT;

	do
	{
		cout << (option == 0 ? selected : not_selected) << "      Change of Nurse" << RESET << endl;
		cout << (option == 1 ? selected : not_selected) << "      Manage Admission" << RESET << endl;

		switch (_getch())
		{
			case KEY_UP:
			{
				if (option > 0){ option--; }
				else { option = optionEnd; }
				break;
			}
			case KEY_DOWN:
			{
				if (option < optionEnd) { option++; }
				else { option = 0; }
				break;
			}
			case KEY_ESCAPE:
			{
				selecting = false;
				break;
			}
			case KEY_ENTER:
			{
				if (option == 0) { ChangeOfNurse(ward_no); selecting = false; }
				else if (option == 1) { ManageBedAdmission(ward_no); selecting = false; }
				break;
			}
		}

		Erase_Lines(0, 2);
	} while (selecting);
}

void Nurse_Menu::Selected_Discharge(Nurse nr, string patientRecord_no)
{
	system("cls");
	Patient patient;
	Sql_DB db;
	char date[11] = {}, card_no[19] = {};
	int option = 0, optionStart = 0, optionEnd = 3, option_paymentMethod = 0, days = 0;
	bool selecting = true;
	string selected = BWHITE_TEXT + BOLD;
	string not_selected = GRAY_TEXT;
	string message;

	//Detail
	string patient_no, patient_name, admit_date;
	string payment_method[2] = {"Cash", "Credit Card"};
	float treatment_price = 0, doctor_charges = 0, bed_price = 0, ward_price = 0, medication_price = 0, total_price = 0;

	//Get patient detail
	db.PrepareStatement("SELECT PR.Patient_Id, P.Name FROM patient P LEFT JOIN patient_record PR ON P.Patient_Id = PR.Patient_Id WHERE PR.Record_Id = ?");
	db.statement->setString(1, patientRecord_no);
	db.QueryResult();
	db.result->next();
	patient_no = db.result->getString("Patient_Id");
	patient_name = db.result->getString("Name");

	//Get Treatment price and Doctor charges
	db.PrepareStatement("SELECT Treatment_Price, Doctor_Charges FROM patient_record WHERE Record_Id = '" + patientRecord_no + "'");
	db.QueryResult();
	db.result->next();
	treatment_price = db.result->getDouble("Treatment_Price");
	doctor_charges = db.result->getDouble("Doctor_Charges");

	//Get admit bed date and price
	bool AdmittedWard = (Get_RowCount("SELECT * FROM bed WHERE Patient_Id = '" + patient_no + "'") == 1) ? true : false ;
	if (AdmittedWard)
	{
		db.PrepareStatement("SELECT Admission_Date, Bed_Price FROM bed WHERE Patient_Id = '" + patient_no + "'");
		db.QueryResult();
		db.result->next();
		admit_date = db.result->getString("Admission_Date");
		bed_price = db.result->getDouble("Bed_Price");
	}
	else // If no admit date, then it is the patient record date
	{
		db.PrepareStatement("SELECT * FROM patient_record WHERE Patient_Id = '" + patient_no + "' AND Payment_Status = 'N'");
		db.QueryResult();
		db.result->next();
		admit_date = db.result->getString("Date");
	}

	//Get the total medication price
	db.PrepareStatement("SELECT Total_Medication_Price FROM pharmacy_record WHERE Record_Id = '" + patientRecord_no + "'");
	db.QueryResult();
	db.result->next();
	medication_price = db.result->getDouble("Total_Medication_Price");


	cout << "\n\n";
	cout << "Discharge Process\n\n"
		<< "Patient's Record\n"
		<< "===================================================================\n\n"
		<< "  Name             : " << patient_name << endl
		<< "  Admit Date       : " << admit_date << endl
	    << "  Treatment Price  : RM " << treatment_price << endl
		<< "  Doctor Charges   : RM " << doctor_charges << endl; //11

	do
	{
		bool isCash = (payment_method[option_paymentMethod] == "Cash" ? true : false);
		total_price = treatment_price + doctor_charges + ward_price + medication_price;

		cout << "  Ward Price       : RM " << ward_price << endl;
		cout << "  Medication Price : RM " << medication_price << endl;
		cout << YELLOW_TEXT << "  Total Price      : RM " << total_price << RESET <<endl << endl;
		cout << "===================================================================\n\n";

		cout << (option == 0 ? selected : not_selected) << "  Discharge Date   : " << date << RESET << endl;
		cout << (option == 1 ? selected : not_selected) << "  Payment Method   : " << payment_method[option_paymentMethod] << RESET << endl;
		cout << (option == 2 ? selected : not_selected) << "  Card Number      : " << card_no << RESET << endl << endl;
		cout << (option == 3 ? selected + "> " : not_selected) << "  Confirm Discharge" << RESET << endl << endl;
		cout << message << endl; //24

		message = "";

		switch (_getch())
		{
			case KEY_UP:
				if (option > 0)
				{ 
					if (option == 3 && isCash) { option = 1; }
					else{ option--; }
				}
				else { option = optionEnd; }
				break;
			case KEY_DOWN:
				if (option < optionEnd) 
				{ 
					if (option == 1 && isCash) { option = 3; }
					else{ option++; }
				}
				else { option = 0; }
				break;
			case KEY_ESCAPE:
				selecting = false;
				break;
			case KEY_ENTER:
			{
				bool exit = false;
				switch (option)
				{
					case 0: //Let user key in discharge date
					{
						int size = sizeof(date) / sizeof(date[0]);
						while (!exit)
						{
							Discharge_Menu(date, payment_method[option_paymentMethod], card_no, option);
							Selected_Input(exit, size, date);
						}

						if (AdmittedWard && date[0] != '\0' && date[9] != '\0')
						{
							db.PrepareStatement("SELECT DATEDIFF(?, ?) AS days_diff");
							db.statement->setString(1, date);
							db.statement->setString(2, admit_date);
							db.QueryResult();
							db.result->next();
							days = stoi(db.result->getString("days_diff"));

							if (days < 0)
							{ 
								for (int i = 0; i < sizeof(date); i++) { date[i] = '\0'; } //Empty the whole char array
								message = "Discharge data cnt early than Admit date.";
							}
							else{ ward_price = bed_price * days; }
						}
						break;
					}
					case 1: //Choose the payment method
					{
						do
						{
							string payment = "< " + payment_method[option_paymentMethod] + " >";
							Discharge_Menu(date, payment, card_no, option);

							switch (_getch())
							{
							case KEY_LEFT:
								if (option_paymentMethod > 0) { option_paymentMethod--; }
								break;
							case KEY_RIGHT:
								if (option_paymentMethod < 1) { option_paymentMethod++; }
								break;
							case KEY_ENTER:
								exit = true;
								break;
							}
						} while (!exit);
					}
					case 2: //Let user key in card_no
					{
						int size = sizeof(card_no) / sizeof(card_no[0]);
						while (!exit)
						{
							Discharge_Menu(date, payment_method[option_paymentMethod], card_no, option);
							Selected_Input(exit, size, card_no);
						}
						break;
					}
					case 3: //Confirm discharge
					{
						try
						{
							//Discharge patient for the total_payment table
							int payment_no = Get_RowCount("SELECT * FROM total_payment") + 1000 + 1;
							string payment_id = "TP" + to_string(payment_no);

							db.PrepareStatement("INSERT INTO total_payment VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, DEFAULT)");
							db.statement->setString(1, payment_id);
							db.statement->setString(2, patientRecord_no);
							db.statement->setString(3, date);
							db.statement->setDouble(4, treatment_price);
							db.statement->setDouble(5, doctor_charges);
							db.statement->setDouble(6, ward_price);
							db.statement->setDouble(7, medication_price);
							db.statement->setDouble(8, total_price);
							db.statement->setString(9, payment_method[option_paymentMethod]);
							isCash ? db.statement->setNull(10, sql::DataType::VARCHAR) : db.statement->setString(10, card_no);
							db.statement->setString(11, nr.Staff_Id);
							db.statement->executeUpdate();

							//Update patient's record on the payment status
							db.PrepareStatement("UPDATE patient_record SET Payment_Status = 'Y' WHERE Record_Id = '" + patientRecord_no + "'");
							db.QueryStatement();

							//Update pharmacy's record on the payment status
							db.PrepareStatement("UPDATE pharmacy_record SET Payment_Status = 'Y' WHERE Record_Id = '" + patientRecord_no + "'");
							db.QueryStatement();

							//If the patient stay in the ward, update the bed to empty
							if (AdmittedWard)
							{
								db.PrepareStatement("UPDATE bed SET Patient_Id = NULL, Admission_Date = NULL, Availability = 'Y' WHERE Patient_Id = '" + patient_no + "'");
								db.QueryStatement();
							}

							//Update the patient status (patient table)
							db.PrepareStatement("UPDATE patient SET Status = 'Discharged' WHERE Patient_Id = '" + patient_no + "'");
							db.QueryStatement();

							//Create transaction record
							int transaction_no = Get_RowCount("SELECT * FROM transaction_record") + 1;
							db.PrepareStatement("INSERT INTO transaction_record VALUES(?, 'Discharge Patient', ?, ?)");
							db.statement->setInt(1, transaction_no);
							db.statement->setString(2, date);
							db.statement->setDouble(3, total_price);
							db.statement->executeUpdate();

							//Update total_payment table for Transaction_Id part, traceable purpose
							db.PrepareStatement("UPDATE total_payment SET Transaction_Id = ? WHERE Payment_Id = ?");
							db.statement->setInt(1, transaction_no);
							db.statement->setString(2, payment_id);
							db.statement->executeUpdate();

							message = "Discharge success!";
							selecting = false;
						}
						catch (sql::SQLException& e)
						{
							message = "Discharge failed.";
						}
						break;
					}
				}
			}
		}
		Erase_Lines(0, 13);
	} while (selecting);


}
/*====================================================================================================================*/



/**********************************************************************************************************************/
//Pharmacist Menu function
int Pharmacist_Menu::Operations_Menu(Pharmacist usedAccount)
{
	int option = 0, optionStart = 0, optionEnd;
	usedAccount.Is_Manager() == true ? optionEnd = 4 : optionEnd = 3;
	string selected = BWHITE_TEXT + BOLD;
	string not_selected = GRAY_TEXT;
	string query; //Store the query for database operations


	cout << BMAGENTA_BG << CYAN_TEXT << RAPID_BLINK << BOLD
		<< "                                                                                                                 " << endl
		<< "                                           PHARMACIST OPERATIONS MENU                                            " << endl
		<< "                                                                                                                 " << endl
		<< RESET << endl;
	cout << "\n\n\n"; //6

	do
	{
		if (usedAccount.Is_Manager())
		{
			cout << (option == 0 ? selected + ">" : not_selected) << "         View Medication Stock" << RESET << "\n\n" //Check the current Medication Stock
				<< (option == 1 ? selected + ">" : not_selected) << "         Create Medication Record" << RESET << "\n\n" //Check the unpaid Patient Record, create medication record to it
				<< (option == 2 ? selected + ">" : not_selected) << "         Modify Medication Record" << RESET << "\n\n" //Modify the unpaid Medication Record
				<< (option == 3 ? selected + ">" : not_selected) << "         Create/Refill Medication Stock" << RESET << "\n\n" //Only for manager, create or refill the medication stonks
				<< "\n\n\n\n\n" //15
				<< (option == 4 ? selected + ">" : not_selected) << "         Sign Out" << RESET << endl
				<< "\n\n\n" << BMAGENTA_BG
				<< "                                                                                                                 " << RESET << "\n\n"; //19
		}
		else
		{
			cout << (option == 0 ? selected + ">" : not_selected) << "         View Medication Stock" << RESET << "\n\n" //Check the current Medication Stock
				<< (option == 1 ? selected + ">" : not_selected) << "         Provide Medication Record" << RESET << "\n\n" //Check the unpaid Patient Record, provide medication to it
				<< (option == 2 ? selected + ">" : not_selected) << "         Modify Medication Record" << RESET << "\n\n" //Modify the unpaid Medication Record
				<< "\n\n" //Only for manager, refill the medication stonks
				<< "\n\n\n\n\n" //15
				<< (option == 3 ? selected + ">" : not_selected) << "         Sign Out" << RESET << endl
				<< "\n\n\n" << BMAGENTA_BG
				<< "                                                                                                                 " << RESET << "\n\n"; //19
		}


		cout << "Current Staff\n"
			<< "-----------------\n"
			<< "Name: " << usedAccount.Name << endl
			<< "Staff ID: " << usedAccount.Staff_Id << (usedAccount.Is_Manager() ? " (Manager)" : "") << endl;//23

		switch (_getch())
		{
		case KEY_UP:
			if (option > optionStart)
			{
				option--;
			}
			else
			{
				option = optionEnd;
			}
			break;

		case KEY_DOWN:
			if (option < optionEnd)
			{
				option++;
			}
			else
			{
				option = optionStart;
			}
			break;
		case KEY_ENTER:
			if (option == optionEnd)
			{
				return 4; //Sign out
			}
			else
			{
				return option; //Return operations
			}
			break;
		}
		Erase_Lines(0, 23);

	} while (true);
}

string Pharmacist_Menu::Get_List(string query, string first_column, string second_column, string third_column, string tittle, string additionalOption)
{
	system("cls");
	Sql_DB db;
	Pharmacist pmc;
	char search[31] = {}, date[11] = {};
	bool selecting = true;
	int option = 0, optionStart = 0, delete_line = 0, optionView = 0;
	string selected = BWHITE_TEXT + BOLD;
	string not_selected = GRAY_TEXT;
	string paid = RESET + GREEN_TEXT;
	string unpaid = RESET + RED_TEXT;

	cout << BMAGENTA_BG << CYAN_TEXT << BOLD
		<< "                                                                                                                 " << endl
		<< tittle << endl
		<< "                                                                                                                 " << endl
		<< RESET << endl;
	cout << "\n"; //5

	cout << "------------------------------------------------------------------------------------------" << endl;
	cout << "|     " << "No." << blank(12, "No.") << "|          Name" << blank(40, "Name") << "|    " << third_column << blank(15, third_column) << "|          " << endl;
	cout << "|                 " << "|                                                  " << "|                   " << "|\n";
	cout << "------------------------------------------------------------------------------------------" << endl; //11


	do
	{
		int optionEnd = Get_RowCount(query);

		//List Menu
		string* id = new string[optionEnd];
		string* name = new string[optionEnd];
		string* third_column_showed = new string[optionEnd];

		//Get the patient's id, staff's name and status in patient
		db.PrepareStatement(query);
		db.QueryResult();

		delete_line = 0;

		if (db.statement)
		{
			int i = 0;
			while (db.result->next())
			{
				id[i] = db.result->getString(first_column);
				name[i] = db.result->isNull(second_column) ? "N/A" : db.result->getString(second_column);
				third_column_showed[i] = db.result->isNull(third_column) ? "N/A" : db.result->getString(third_column);

				if (third_column == "Total_Price")
				{
					// Get the position of the decimal point
					int dotPosition = third_column_showed[i].find('.');

					// Copy the string start from '.' to 2 decimal point which is (1 + 2 = 3)
					third_column_showed[i] = third_column_showed[i].substr(0, dotPosition + 3);
				}

				i++;
				delete_line += 2;
			}
		}

		for (int i = 0; i < optionEnd; i++)
		{
			cout << (i == option ? selected : not_selected) << "|     " << id[i] << blank(12, id[i])
				<< "|          " << name[i] << blank(40, name[i]) << "|    "
				<< (third_column_showed[i] == "Y" ? paid : "") << (third_column_showed[i] == "N" ? unpaid : "") << third_column_showed[i] << RESET << blank(15, third_column_showed[i]) << (i == option ? selected : not_selected) << "|          \n";
			cout << "|                 " << "|                                                  " << "|                   " << "|\n";
		}
		cout << RESET << "==========================================================================================\n\n" << endl; //14 , erase_line where 9 - 2(print) = 7

		if (additionalOption == "Create new pharmacy record" || additionalOption == "Create new medication record")
		{
			cout << (option == optionEnd ? selected + "> " : not_selected) << additionalOption << RESET << endl;
		}
		else if (additionalOption == "Search")
		{
			cout << (option == optionEnd ? selected : not_selected) << "      Search: " << search << RESET << endl;
		}
		else if (additionalOption == "Refill medication")
		{
			cout << (option == optionEnd ? selected + "> " : not_selected) << "      Create New Medication " << RESET << endl;
		}
		else
		{
			cout << endl;
		}

		switch (_getch())
		{
		case KEY_UP:
			if (option > optionStart) { option--; }
			else
			{
				if (additionalOption.empty() || additionalOption == "Modify medication record") { option = optionEnd - 1; }
				else { option = optionEnd; }
			}
			break;
		case KEY_DOWN:
			if ((additionalOption.empty() || additionalOption == "Modify medication record") ? option < optionEnd - 1 : option < optionEnd) { option++; }
			else { option = optionStart; }
			break;
		case KEY_ESCAPE:
			delete[]id;
			delete[]name;
			delete[]third_column_showed;
			return "";
			break;
		case KEY_ENTER:
			if (additionalOption.empty()) //Patient Record Menu
			{
				if (third_column_showed[option] == "Y") //At patient record menu, if the record is paid it will block the user
				{
					Erase_Lines(0, 1);
					cout << "The patient record already paid!\n";
					_getch();

				}
				else
				{
					delete[]name;
					delete[]third_column_showed;
					return id[option];
				}
			}

			if (additionalOption == "Create new pharmacy record") //Pharmacy Record Menu
			{
				if (option == optionEnd)
				{
					delete[]id;
					delete[]name;
					delete[]third_column_showed;
					return "Create pharmacy record";
				}
				else //If having additionalOption, but not using it
				{
					Erase_Lines(0, 1);
					delete[]name;
					delete[]third_column_showed;
					return id[option];
				}
			}
			else if (additionalOption == "Create new medication record")
			{
				if (option == optionEnd)
				{
					delete[]id;
					delete[]name;
					delete[]third_column_showed;
					return "Create medication record";
				}
				else //If having additionalOption, but not using it
				{
					Erase_Lines(0, 1);
					delete[]name;
					delete[]third_column_showed;
					return id[option];
				}
			}
			else if (additionalOption == "Modify medication record")
			{
				int optionSelect = 0;
				bool optionSelecting = true;
				bool paid = false;

				//Check whether the medication record is paid
				int optionPaid = Get_RowCount("SELECT * FROM pharmacy_record WHERE Payment_Status = 'Y'");
				string* paid_Id = new string[optionPaid];
				db.PrepareStatement("SELECT * FROM pharmacy_record WHERE Payment_Status = 'Y'");
				db.QueryResult();

				if (db.result)
				{
					int i = 0;
					while (db.result->next())
					{
						paid_Id[i] = db.result->getString("PharmacyRecord_Id");
						i++;
					}
				}

				for (int i = 0; i < optionPaid; i++)
				{
					if (id[option] == paid_Id[i]) { paid = true; }
				}

				if (!paid)
				{
					do
					{
						cout << (optionSelect == 0 ? selected + "> " : not_selected) << "      Update medication record" << RESET << endl;
						cout << (optionSelect == 1 ? selected + "> " : not_selected) << "      Delete medication record" << RESET << endl;

						switch (_getch())
						{
						case KEY_UP:
							if (optionSelect > 0) { optionSelect--; }
							break;
						case KEY_DOWN:
							if (optionSelect < 1) { optionSelect++; }
							break;
						case KEY_ESCAPE:
							optionSelecting = false;
							break;
						case KEY_ENTER:
							if (optionSelect == 0) { return "Update medication record" + id[option]; }
							else if (optionSelect == 1) { return "Delete medication record" + id[option]; }
							break;
						}
						Erase_Lines(0, 2);
					} while (optionSelecting);
				}
				else
				{
					cout << "This medication record already paid!" << endl;
					_getch();
					Erase_Lines(0, 1);
				}
				
			}
			else if (additionalOption == "Refill medication")
			{
				char refill[20] = {};
				int refill_quantity = 0, refill_option = 0;
				bool refill_confirm = false;

				Erase_Lines(0, 1);
				if (option == optionEnd) { return "Create new medication"; }
				cout << "\n";

				do
				{
					cout << (refill_option == 0 ? selected : not_selected) << "      Restock quantity: " << refill_quantity << RESET << endl;
					cout << (refill_option == 1 ? selected : not_selected) << "      Date            : " << date << RESET << endl;
					cout << (refill_option == 2 ? selected + "> " : not_selected) << "      Confirm" << RESET << endl;

					switch (_getch())
					{
						case KEY_UP:
						{
							if (refill_option > 0) { refill_option--; }
							break;
						}
						case KEY_DOWN:
						{
							if (refill_option < 2) { refill_option++; }
							break;
						}
						case KEY_ENTER:
						{
							bool exit = false;
							if (refill_option == 0)
							{
								int size = sizeof(refill) / sizeof(refill[0]);
								while (!exit)
								{
									Erase_Lines(0, 3);
									cout << (refill_option == 0 ? selected + "> " : not_selected) << "      Restock quantity: " << refill_quantity << RESET << endl;
									cout << (refill_option == 1 ? selected + "> " : not_selected) << "      Date            : " << date << RESET << endl;
									cout << (refill_option == 2 ? selected + "> " : not_selected) << "      Confirm" << RESET << endl;
									Selected_Input(exit, size, refill, refill_quantity);
								}
							}
							else if (refill_option == 1)
							{
								int size = sizeof(date) / sizeof(date[0]);
								while (!exit)
								{
									Erase_Lines(0, 3);
									cout << (refill_option == 0 ? selected + "> " : not_selected) << "      Restock quantity: " << refill_quantity << RESET << endl;
									cout << (refill_option == 1 ? selected + "> " : not_selected) << "      Date            : " << date << RESET << endl;
									cout << (refill_option == 2 ? selected + "> " : not_selected) << "      Confirm" << RESET << endl;
									Selected_Input(exit, size, date);
								}
							}
							else if (refill_option == 2)
							{
								string message = pmc.RefillMedication(id[option], refill_quantity, date);
								cout << "\n\n" << message << endl;
								_getch();
								Erase_Lines(0, 3);
								if(message == "Restock successfully!"){ refill_confirm = true; }

							}
							break;
						}
						case KEY_ESCAPE:
						{
							refill_confirm = true;
							break;
						}

					}
					Erase_Lines(0, 3);
				} while (!refill_confirm);
			}
			break;
		}
		Erase_Lines(0, 4 + delete_line);
		delete[]id;
		delete[]name;
		delete[]third_column_showed;
	} while (selecting);
}

void Pharmacist_Menu::CreateMedication_Menu(int option, char date[], char medication_Name[], float medication_price, float buy_price, float quantity)
{
	string selected = BWHITE_TEXT + BOLD + "> ";
	string not_selected = GRAY_TEXT;

	cout << (option == 0 ? selected : not_selected) << "     Date                  : " << date << RESET << "\n\n";
	cout << (option == 1 ? selected : not_selected) << "     Medication Name       : " << medication_Name << RESET << "\n\n";
	cout << (option == 2 ? selected : not_selected) << "     Medication Price (RM) : " << medication_price << RESET << "\n\n";
	cout << (option == 3 ? selected : not_selected) << "     Buy Price (RM)        : " << buy_price << RESET << "\n\n";
	cout << (option == 4 ? selected : not_selected) << "     Quantity              : " << quantity << RESET << "\n\n\n";
	cout << (option == 5 ? selected : not_selected) << "     Confirm" << RESET << endl;
}

void Pharmacist_Menu::Selected_AddMedicationRecord(Pharmacist pmc, string pharmacyRecord_no)
{
	system("cls");
	Sql_DB db;
	char quantity_sell[20] = {};
	int option = 0, option_Medication = 0;
	int quantity = 0;
	int optionEnd = Get_RowCount("SELECT * FROM medication") + 1;
	string selected = BWHITE_TEXT + BOLD;
	string not_selected = GRAY_TEXT;
	string* medication_id = new string[optionEnd];
	string* medication_name = new string[optionEnd];
	bool selecting = true;

	//Get the room's id, doctor name and doctor's staff id
	db.PrepareStatement("SELECT * FROM medication");
	db.QueryResult();

	if (db.statement)
	{
		int i = 1;
		while (db.result->next())
		{
			medication_id[i] = db.result->getString("Medication_Id");
			medication_name[i] = db.result->getString("Medication_Name");
			i++;
		}
	}

	try {
		db.PrepareStatement("SELECT p.Name, pr.* FROM pharmacy_record phr JOIN patient_record pr ON phr.Record_Id = pr.Record_Id JOIN patient p ON pr.Patient_Id = p.Patient_Id WHERE phr.PharmacyRecord_Id = ?");
		db.statement->setString(1, pharmacyRecord_no);
		db.QueryResult();
		db.result->next();

		cout << "Patient Record" << endl
			<< "=======================================================================================\n\n"
			<< "Name: " << db.result->getString("Name") << "\n\n"
			<< "Date: " << db.result->getString("Date") << "\n\n"
			<< "Diagnosis: " << db.result->getString("Diagnosis") << "\n\n"
			<< "Treatment: " << db.result->getString("Treatment") << "\n\n"
			<< "Description: " << db.result->getString("Description") << "\n\n"
			<< "=======================================================================================\n\n\n\n";
	}
	catch (sql::SQLException e) {
		cout << "#ERR: SQLException in " << __FILE__
			<< "(" << __FUNCTION__ << ") on file " << __LINE__ << endl
			<< "# ERR: " << e.what()
			<< " (MySQL error code: " << e.getErrorCode()
			<< ", SQLState: " << e.getSQLState() << ")" << endl;
		_getch();
	}

	cout << "Create medication record" << endl
		<< "=======================================================================================\n\n";

	do
	{
		cout << (option == 0 ? selected : not_selected) << "Medication Name: " << medication_name[option_Medication] << RESET << endl
		     << (option == 1 ? selected : not_selected) << "Quantity       : " << quantity << endl;
		cout << "\n\n" << (option == 2 ? selected + ">" : not_selected) << "Confirm" << RESET << endl;

		switch (_getch())
		{
		case KEY_UP:
			if (option > 0){option--;}
			else { option = 2; }
			break;
		case KEY_DOWN:
			if (option < 2) { option++; }
			else { option = 0; }
			break;
		case KEY_ESCAPE:
			selecting = false;
			break;
		case KEY_ENTER:
			bool exit = false;
			switch (option)
			{
				case 0:
				{
					while (!exit)
					{
						Erase_Lines(0, 5);
						cout << (option == 0 ? selected + "> " : not_selected) << "Medication Name: " << (option == 0 ? "< " : "") << medication_name[option_Medication] << (option == 0 ? " >" : "") << RESET << endl
							 << (option == 1 ? selected : not_selected) << "Quantity       : " << quantity << endl;
						cout << "\n\n" << (option == 2 ? selected : not_selected) << "Confirm" << RESET << endl;

						switch (_getch())
						{
						case KEY_LEFT:
							if (option_Medication > 0) { option_Medication--; }
							break;
						case KEY_RIGHT:
							if (option_Medication < optionEnd - 1) { option_Medication++; }
							break;
						case KEY_ENTER:
							exit = true;
							break;
						}
					}
					break;
				}
				case 1:
				{
					int size = sizeof(quantity_sell) / sizeof(quantity_sell[0]);
					while (!exit)
					{
						Erase_Lines(0, 5);
						cout << (option == 0 ? selected : not_selected) << "Medication Name: " << (option == 0 ? "< " : "") << medication_name[option_Medication] << (option == 0 ? " >" : "") << RESET << endl
							<< (option == 1 ? selected + "> " : not_selected) << "Quantity       : " << quantity << endl;
						cout << "\n\n" << (option == 2 ? selected : not_selected) << "Confirm" << RESET << endl;
						Selected_Input(exit, size, quantity_sell, quantity);
					}
					break;
				}
				case 2:
				{
					string message = pmc.AddMedicationRecord(std::stoi(medication_id[option_Medication]), quantity, pharmacyRecord_no);
					Erase_Lines(0, 1);
					message == "Input successfull." ? selecting = false : selecting = true;
					break;
				}
			}
			break;
		}

		Erase_Lines(0, 5);
	} while (selecting);
}

void Pharmacist_Menu::Selected_MedicationRecord(Pharmacist pmc, string pharmacyRecord_no)
{
	bool selecting = true;

	do
	{
		string medicationRecord_no = Get_List("SELECT MedicationRecord_Id, Medication_Name, MR.Quantity FROM Medication_Record MR JOIN Medication M ON MR.Medication_Id = M.Medication_Id WHERE MR.PharmacyRecord_Id = '" + pharmacyRecord_no + "'",
			"MedicationRecord_Id", "Medication_Name", "Quantity",
			"                                                 MEDICATION RECORD                                               ", "Create new medication record");

		if (medicationRecord_no == "Create medication record")
		{
			Selected_AddMedicationRecord(pmc, pharmacyRecord_no);
		}

		if (medicationRecord_no.empty())
		{
			selecting = false;
		}
	} while (selecting);

}

void Pharmacist_Menu::Selected_PharmacyRecord(Pharmacist pmc, string record_no)
{
	bool selecting = true;

	do
	{
		string pharmacyRecord_no = Get_List("SELECT PharmacyRecord_Id, P.Name, PHR.Payment_Status FROM pharmacy_record PHR JOIN patient_record PR ON PHR.Record_Id = PR.Record_Id JOIN patient P ON PR.Patient_Id = P.Patient_Id WHERE PR.Record_Id = '" + record_no + "'",
			"PharmacyRecord_Id", "Name", "Payment_Status",
			"                                                  PHARMACY RECORD                                                ", "Create new pharmacy record");

		if (pharmacyRecord_no == "Create pharmacy record")
		{
			bool exist = true;
			Get_RowCount("SELECT * FROM pharmacy_record WHERE Record_Id = '" + record_no + "'") == 1 ? exist = true : exist = false; //See whether there's already have pharmacy record or not

			if (exist)
			{
				Erase_Lines(0, 1);
				cout << "It already have pharmacy record!";
				_getch();
			}
			else
			{
				pmc.AddPharmacyRecord(record_no);
			}
		}
		else
		{
			if (pharmacyRecord_no.empty())
			{
				selecting = false;
			}
			else
			{
				Selected_MedicationRecord(pmc, pharmacyRecord_no);
			}
		}

	} while (selecting);

}

void Pharmacist_Menu::Selected_UpdateMedicationRecord(Pharmacist pmc, string medicationRecord_no)
{
	system("cls");
	Sql_DB db;
	char quantity_sell[20] = {};
	int option = 0, option_Medication = 0;
	float quantity = 0;
	int optionEnd = Get_RowCount("SELECT * FROM medication") + 1;
	string selected = BWHITE_TEXT + BOLD;
	string not_selected = GRAY_TEXT;
	string* medication_id = new string[optionEnd];
	string* medication_name = new string[optionEnd];
	bool selecting = true;

	//Get the medication's name
	db.PrepareStatement("SELECT * FROM medication");
	db.QueryResult();
	if (db.result)
	{
		int i = 1;
		while (db.result->next())
		{
			medication_id[i] = db.result->getString("Medication_Id");
			medication_name[i] = db.result->getString("Medication_Name");
			i++;
		}
	}

	try
	{
		//Get the current medication's name and quantity
		db.PrepareStatement("SELECT * FROM medication_record WHERE MedicationRecord_Id = '" + medicationRecord_no + "'");
		db.QueryResult();
		if (db.result)
		{
			db.result->next();
			string current_id = db.result->getString("Medication_Id");
			int i = 1;

			while (i < optionEnd)
			{
				if (medication_id[i] == db.result->getString("Medication_Id"))
				{
					option_Medication = i;
					quantity = db.result->getInt("Quantity");
				}
				i++;
			}

		}
	}
	catch (sql::SQLException& e)
	{
		cout << "#ERR: SQLException in " << __FILE__
			<< "(" << __FUNCTION__ << ") on file " << __LINE__ << endl
			<< "# ERR: " << e.what()
			<< " (MySQL error code: " << e.getErrorCode()
			<< ", SQLState: " << e.getSQLState() << ")" << endl;
		cout << "Second" << endl;
		_getch();
	}

	


	cout << "Update medication report" << endl
		<< "=======================================================================================\n\n";

	do
	{
		cout << (option == 0 ? selected : not_selected) << "Medication Name: " << medication_name[option_Medication] << RESET << endl
			<< (option == 1 ? selected : not_selected) << "Quantity       : " << quantity << endl;
		cout << "\n\n" << (option == 2 ? selected + "> " : not_selected) << "Confirm" << RESET << endl;

		switch (_getch())
		{
		case KEY_UP:
			if (option > 0) { option--; }
			else { option = 2; }
			break;
		case KEY_DOWN:
			if (option < 2) { option++; }
			else { option = 0; }
			break;
		case KEY_ESCAPE:
			selecting = false;
			break;
		case KEY_ENTER:
			bool exit = false;
			switch (option)
			{
			case 0:
			{
				while (!exit)
				{
					Erase_Lines(0, 5);
					cout << (option == 0 ? selected + "> " : not_selected) << "Medication Name: " << (option == 0 ? "< " : "") << medication_name[option_Medication] << (option == 0 ? " >" : "") << RESET << endl
						<< (option == 1 ? selected : not_selected) << "Quantity       : " << quantity << endl;
					cout << "\n\n" << (option == 2 ? selected : not_selected) << "Confirm" << RESET << endl;

					switch (_getch())
					{
					case KEY_LEFT:
						if (option_Medication > 0) { option_Medication--; }
						break;
					case KEY_RIGHT:
						if (option_Medication < optionEnd - 1) { option_Medication++; }
						break;
					case KEY_ENTER:
						exit = true;
						break;
					}
				}
				break;
			}
			case 1:
			{
				int size = sizeof(quantity_sell) / sizeof(quantity_sell[0]);
				while (!exit)
				{
					Erase_Lines(0, 5);
					cout << (option == 0 ? selected : not_selected) << "Medication Name: " << (option == 0 ? "< " : "") << medication_name[option_Medication] << (option == 0 ? " >" : "") << RESET << endl
						<< (option == 1 ? selected + "> " : not_selected) << "Quantity       : " << quantity << endl;
					cout << "\n\n" << (option == 2 ? selected : not_selected) << "Confirm" << RESET << endl;
					Selected_Input(exit, size, quantity_sell, quantity);
				}
				break;
			}
			case 2:
			{
				string message = pmc.UpdateMedicationRecord(option_Medication, quantity, medicationRecord_no);
				Erase_Lines(0, 1);
				message == "Update successfull." ? selecting = false : selecting = true;
				break;
			}
			}
			break;
		}

		Erase_Lines(0, 5);
	} while (selecting);
}

void Pharmacist_Menu::Selected_CreateMedication(Pharmacist pmc)
{
	system("cls");
	Sql_DB db;
	char date[11] = {}, medication_Name[41] = {}, Medication_price[20] = {}, Buy_price[20] = {}, Quantity[20] = {};
	float medication_price = 0, buy_price = 0, quantity = 0;
	int option = 0, option_Medication = 0;
	int stock_refill = 0;
	int optionEnd = Get_RowCount("SELECT * FROM medication") + 1;
	string selected = BWHITE_TEXT + BOLD;
	string not_selected = GRAY_TEXT;
	bool selecting = true;

	cout << "\n\n"
		<< "Create New Medication" << endl
		<< "=======================================================================================\n\n";

	do
	{
		cout << (option == 0 ? selected : not_selected) << "     Date                  : " << date << RESET << "\n\n";
		cout << (option == 1 ? selected : not_selected) << "     Medication Name       : " << medication_Name << RESET << "\n\n";
		cout << (option == 2 ? selected : not_selected) << "     Medication Price (RM) : " << medication_price << RESET << "\n\n";
		cout << (option == 3 ? selected : not_selected) << "     Buy Price (RM)        : " << buy_price << RESET << "\n\n";
		cout << (option == 4 ? selected : not_selected) << "     Quantity              : " << quantity << RESET << "\n\n\n";
		cout << (option == 5 ? selected + "> " : not_selected) << "     Confirm" << RESET << endl;

		switch (_getch())
		{
			case KEY_UP:
				if (option > 0) { option--; }
				break;
			case KEY_DOWN:
				if (option < 5) { option++; }
				break;
			case KEY_ESCAPE:
				selecting = false;
				break;
			case KEY_ENTER:
			{
				bool exit = false;
				switch (option)
				{
					case 0:
					{
						int size = sizeof(date) / sizeof(date[0]);
						while (!exit)
						{
							Erase_Lines(0, 12);
							CreateMedication_Menu(option, date, medication_Name, medication_price, buy_price, quantity);
							Selected_Input(exit, size, date);
						}
						break;
					}
					case 1:
					{
						int size = sizeof(medication_Name) / sizeof(medication_Name[0]);
						while (!exit)
						{
							Erase_Lines(0, 12);
							CreateMedication_Menu(option, date, medication_Name, medication_price, buy_price, quantity);
							Selected_Input(exit, size, medication_Name);
						}
						break;
					}
					case 2:
					{
						int size = sizeof(Medication_price) / sizeof(Medication_price[0]);
						while (!exit)
						{
							Erase_Lines(0, 12);
							CreateMedication_Menu(option, date, medication_Name, medication_price, buy_price, quantity);
							Selected_Input(exit, size, Medication_price, medication_price);
						}
						break;
					}
					case 3:
					{
						int size = sizeof(Buy_price) / sizeof(Buy_price[0]);
						while (!exit)
						{
							Erase_Lines(0, 12);
							CreateMedication_Menu(option, date, medication_Name, medication_price, buy_price, quantity);
							Selected_Input(exit, size, Buy_price, buy_price);
						}
						break;
					}
					case 4:
					{
						int size = sizeof(Quantity) / sizeof(Quantity[0]);
						while (!exit)
						{
							Erase_Lines(0, 12);
							CreateMedication_Menu(option, date, medication_Name, medication_price, buy_price, quantity);
							Selected_Input(exit, size, Quantity, quantity);
						}
						break;
					}
					case 5:
					{
						Erase_Lines(0, 1);
						string message = pmc.Create_NewMedication(date, medication_Name, medication_price, buy_price, quantity);
						cout << message << endl;
						if (message == "Create successfully!") { selecting = false; }
						_getch();
						break;
					}
				}
				break;
			}
		}
		Erase_Lines(0, 12);
	} while (selecting);
}
/*====================================================================================================================*/