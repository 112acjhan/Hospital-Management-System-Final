#include <iostream>
#include <string>
#include <conio.h>
#include <regex>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <vector>
#include "Account.h"

/**********************************************************************************************************************/
// User defined function
string XOR_Cipher(const string input) //Encrypt and decrypt by using XOR operation
{
    string output = input;
    char key = 'H'; //Can be replace by any character
    for (size_t i = 0; i < input.size(); ++i) {
        output[i] = input[i] ^ key;
    }

    return output;
}

string Encrypt_CaesarCipher(const string text) //Encrypt by using Caesar Cipher
{
    std::string result = "";
    int shift = 9; //Can be replace by any number
    for (char c : text) {
        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            c = (c - base + shift) % 26 + base;
        }
        else if (isdigit(c)) {
            c = (c - '0' + shift) % 10 + '0';
        }
        result += c;
    }
    return result;
}

string Decrypt_CaesarCipher(const string text) //Decrypt the password that been encrypted using Caesar Cipher
{
    std::string result = "";
    int shift = 9; //Can be replace by any number
    for (char c : text) {
        if (isalpha(c)) {
            char base = islower(c) ? 'a' : 'A';
            c = (c - base - shift + 26) % 26 + base;
        }
        else if (isdigit(c)) {
            c = (c - '0' - shift + 10) % 10 + '0';
        }
        result += c;
    }
    return result;
}

bool IsEmpty(char input[])
{
    return input[0] == '\0';
}

bool isValidDateOfBirth(const std::string& dateOfBirth) {
    const regex pattern(R"(^\d{4}-(0[1-9]|1[0-2])-(0[1-9]|[1-2][0-9]|3[0-1])$)");
    return regex_match(dateOfBirth, pattern);
}

bool IsValidEmail(const string& email) {
    const regex pattern(R"(([\w\.-]+)@([\w\.-]+)\.([a-zA-Z]{2,}))");
    return regex_match(email, pattern);
}

bool IsValidContactNumber(const string& contactNumber) {
    const regex pattern(R"(^(01\d-\d{7,8})$)");
    return regex_match(contactNumber, pattern);
}

int Get_Row_Count(string query)
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
/*====================================================================================================================*/



/**********************************************************************************************************************/
// Account Function
Account::Account(){}

Account::~Account(){}

void Account::Registration(string& message)
{
    Sql_DB db;
    int Registration_Id = 1;
    string errorMessage = "# Invalid registration";
    string correctMessage = "# Successfully registered";
    string errorType;
    bool isValid = true, correctFormat = true;

    if (!isValidDateOfBirth(DOB)) {
        correctFormat = false;
        errorType = ": Invalid Date of Birth Format.";
    }
    else if (!IsValidEmail(Email)) {
        correctFormat = false;
        errorType = ": Invalid Email Format.";
    }
    else if (!IsValidContactNumber(Contact_Number)){
        correctFormat = false;
        errorType = ": Invalid Contact Number Format.";
    }

    db.PrepareStatement("SELECT Registration_Id FROM registration");
    db.QueryResult();
    while (db.result->next())
    {
        Registration_Id = db.result->getInt("Registration_Id") + 1;
    }

    // Construct the SQL query dynamically
    string query = "INSERT INTO registration VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ";
    if (Manage_By[0] == '\0')
    {
        query += "NULL, DEFAULT, DEFAULT)";
    }
    else 
    {
        query += "?, DEFAULT, DEFAULT)";
    }
    db.PrepareStatement(query);

    if (correctFormat)
    {
        db.statement->setInt(1, Registration_Id);
        IsEmpty(Name) ? db.statement->setNull(2, sql::DataType::VARCHAR) : db.statement->setString(2, Name);
        db.statement->setString(3, DOB);
        IsEmpty(Gender) ? db.statement->setNull(4, sql::DataType::VARCHAR) : db.statement->setString(4, Gender);
        IsEmpty(Race) ? db.statement->setNull(5, sql::DataType::VARCHAR) : db.statement->setString(5, Race);
        IsEmpty(Religion) ? db.statement->setNull(6, sql::DataType::VARCHAR) : db.statement->setString(6, Religion);
        IsEmpty(Address) ? db.statement->setNull(7, sql::DataType::VARCHAR) : db.statement->setString(7, Address);
        IsEmpty(IC) ? db.statement->setNull(8, sql::DataType::VARCHAR) : db.statement->setString(8, IC);
        db.statement->setDouble(9, Salary);
        IsEmpty(Role) ? db.statement->setNull(10, sql::DataType::VARCHAR) : db.statement->setString(10, Role);
        IsEmpty(Contact_Number) ? db.statement->setNull(11, sql::DataType::VARCHAR) : db.statement->setString(11, Contact_Number);
        IsEmpty(Email) ? db.statement->setNull(12, sql::DataType::VARCHAR) : db.statement->setString(12, Email);
        IsEmpty(Password) ? db.statement->setNull(13, sql::DataType::VARCHAR) : db.statement->setString(13, Password);

        // Only set the Manage_By parameter if it's not empty
        if (Manage_By[0] != '\0')
        {
            db.statement->setString(14, Manage_By);
        }

        try
        {
            db.statement->executeUpdate(); //For execute Insert query
        }
        catch (sql::SQLException& e)
        {
            isValid = false;
        }
    }
    else { isValid = false; }

    if (isValid == true)
    {
        message = correctMessage;
    }
    else
    {
        message = errorMessage + errorType;
    }
}

bool Account::Login()
{
	Sql_DB db;
    bool IsActivate = true;

	db.PrepareStatement("SELECT * FROM account WHERE IC = ? AND password = ?");
	db.statement->setString(1, IC); //first ? index
	db.statement->setString(2, encrypt(Password)); //second ? index
	db.QueryResult(); //Execute prepare statement


    if (db.result->rowsCount() == 1) //How many rows return, if it is one means it found the data
    {
        // Check the account is activate or not
        db.PrepareStatement("SELECT Status FROM staff WHERE IC = ?");
        db.statement->setString(1, IC);
        db.QueryResult();
        db.result->next();
        IsActivate = (db.result->getString("Status") == "Activate" ? true : false);

        if (IsActivate) { return true; }
        else { return false; }
    }
    else
    {
    return false;
    }
}

void Account::Update(string& message)
{
    Sql_DB db;
    bool isValid = true, correctFormat = true;
    string errorMessage = "# Invalid update";
    string correctMessage = "# Updated successfull";
    string errorType;

    if (!isValidDateOfBirth(DOB)) {
        correctFormat = false;
        errorType = ": Invalid Date of Birth Format.";
    }
    else if (!IsValidEmail(Email)) {
        correctFormat = false;
        errorType = ": Invalid Email Format.";
    }
    else if (!IsValidContactNumber(Contact_Number)) {
        correctFormat = false;
        errorType = ": Invalid Contact Number Format.";
    }

    // Construct the SQL query dynamically
    string query = "UPDATE staff SET Name = ?, DOB = ?, Gender = ?, Race = ?, Religion = ?, Address = ?, Salary = ?, Role = ?, Contact_Number = ?, Email = ?, Manage_By = ";
    if (Manage_By[0] == '\0')
    {
        query += " DEFAULT WHERE Staff_Id = ?";
    }
    else
    {
        query += " ? WHERE Staff_Id = ?";
    }
    db.PrepareStatement(query);

    if (correctFormat)
    {
        IsEmpty(Name) ? db.statement->setNull(1, sql::DataType::VARCHAR) : db.statement->setString(1, Name);
        IsEmpty(DOB) ? db.statement->setNull(2, sql::DataType::VARCHAR) : db.statement->setString(2, DOB);
        IsEmpty(Gender) ? db.statement->setNull(3, sql::DataType::VARCHAR) : db.statement->setString(3, Gender);
        IsEmpty(Race) ? db.statement->setNull(4, sql::DataType::VARCHAR) : db.statement->setString(4, Race);
        IsEmpty(Religion) ? db.statement->setNull(5, sql::DataType::VARCHAR) : db.statement->setString(5, Religion);
        IsEmpty(Address) ? db.statement->setNull(6, sql::DataType::VARCHAR) : db.statement->setString(6, Address);
        db.statement->setDouble(7, Salary);
        IsEmpty(Role) ? db.statement->setNull(8, sql::DataType::VARCHAR) : db.statement->setString(8, Role);
        IsEmpty(Contact_Number) ? db.statement->setNull(9, sql::DataType::VARCHAR) : db.statement->setString(9, Contact_Number);
        IsEmpty(Email) ? db.statement->setNull(10, sql::DataType::VARCHAR) : db.statement->setString(10, Email);

        if (Manage_By[0] == '\0')
        {
            db.statement->setString(11, Staff_Id);
        }
        else
        {
            db.statement->setString(11, Manage_By);
            db.statement->setString(12, Staff_Id);
        }
        

        try
        {
            db.statement->executeUpdate(); //For execute Insert query
        }
        catch (sql::SQLException& e)
        {
            isValid = false;
        }
    }
    else { isValid = false; }

    if (isValid == true)
    {
        message = correctMessage;
    }
    else
    {
        message = errorMessage + errorType;
    }
}

void Account::Remove(string& message)
{
    Sql_DB db;
    bool isValid = true;
    string errorMessage = "# Invalid remove, manager cannot be removed";

    //Check if it is manager
    //Get how many manager available in the database
    string query = "SELECT * FROM Staff WHERE Manage_By IS NULL";
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
    string* Manager_IC = new string[row_count]; //dynamic array

    //Get the Manager's name and staff_id
    db.PrepareStatement(query);
    db.QueryResult();

    if (db.result)
    {
        int i = 1;
        while (db.result->next())
        {
            Manager_IC[i] = db.result->getString("IC");
            if (IC == Manager_IC[i])
            {
                isValid = false;
            }
            i++;
        }
    }
    
    if (isValid == false)
    {
        message = errorMessage;
    }
    else
    {
        try
        {
            // Update the Status for the staff table from 'Activate' to 'Inactivate'
            db.PrepareStatement("UPDATE staff SET Status = 'Inactivate' WHERE IC = ?");
            db.statement->setString(1, IC);
            db.QueryStatement();

        }
        catch (sql::SQLException& e)
        {
            message = errorMessage;
        }
    }
}

string Account::encrypt(const string text)
{
    string caesarEncrypted = Encrypt_CaesarCipher(text);
    return XOR_Cipher(caesarEncrypted);
}

string Account::decrypt(const string text)
{
    string xorDecrypted = XOR_Cipher(text);
    return Decrypt_CaesarCipher(xorDecrypted);
}

string Account::Get_Id()
{
    return Staff_Id;
}

string Account::Get_Role()
{
    return Role;
}

bool Account::Is_Manager()
{
    if (Manage_By[0] == '\0')
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*====================================================================================================================*/



/**********************************************************************************************************************/
// Patient and Patient_Record Function
void Patient::Patient_Detail(string query, string patient_no)
{
    Sql_DB db;
    db.PrepareStatement(query);
    db.statement->setString(1, patient_no);
    db.QueryResult();
    if (db.result != nullptr)
    {
        while (db.result->next())
        {
            strcpy_s(Patient_Id, db.result->getString("Patient_Id").c_str());
            strcpy_s(Name, db.result->getString("Name").c_str());
            strcpy_s(DOB, db.result->getString("DOB").c_str());
            strcpy_s(Gender, db.result->getString("Gender").c_str());
            strcpy_s(Race, db.result->getString("Race").c_str());
            strcpy_s(Religion, db.result->getString("Religion").c_str());
            strcpy_s(Address, db.result->getString("Address").c_str());
            strcpy_s(IC, db.result->getString("IC").c_str());
            strcpy_s(Contact_Number, db.result->getString("Contact_Number").c_str());
            strcpy_s(Email, db.result->getString("Email").c_str());
        }
    }
}

void Patient_Record::PatientRecord_Detail(string record_no)
{
    Sql_DB db;
    string query = "SELECT * FROM patient_record WHERE Record_Id = ?";
    db.PrepareStatement(query);
    db.statement->setString(1, record_no);
    db.QueryResult();
    if (db.result != nullptr)
    {
        while (db.result->next())
        {
            strcpy_s(Patient_Id, db.result->getString("Patient_Id").c_str());
            strcpy_s(Staff_Id, db.result->getString("Staff_Id").c_str());
            strcpy_s(date, db.result->getString("Date").c_str());
            strcpy_s(diagnosis, db.result->getString("Diagnosis").c_str());
            strcpy_s(treatment, db.result->getString("Treatment").c_str());
            strcpy_s(Description, db.result->getString("Description").c_str());
            strcpy_s(payment_status, db.result->getString("Payment_Status").c_str());
            treatment_price = db.result->getDouble("Treatment_Price");
            doctor_charges = db.result->getDouble("Doctor_Charges");
        }
    }
}

void Patient_Record::Show_PatientRecord(string record_no)
{
    PatientRecord_Detail(record_no);

    system("cls");

    if (!record_no.empty())
    {
        cout << "Patient Record\n";
        cout << "=======================================================================================\n\n";
        cout << "  Record No            : " << record_no << endl << endl;
        cout << "  Date(YYYY-MM-DD)     : " << date << endl << endl;
        cout << "  Diagnosis            : " << diagnosis << endl << endl;
        cout << "  Treatment            : " << treatment << endl << endl;
        cout << "  Treatment Price (RM) : " << treatment_price << endl << endl;
        cout << "  Doctor Charges (RM)  : " << doctor_charges << endl << endl;
        cout << "  Description*         : " << Description << endl << endl;
        cout << "  Record By            : " << Staff_Id << endl << endl;
        cout << "=======================================================================================\n\n";
        _getch();
    }
}
/*====================================================================================================================*/



/**********************************************************************************************************************/
// HR Function
void HR::Get_Detailed_Information(Account registered_acc)
{

}
/*====================================================================================================================*/



/**********************************************************************************************************************/
// Nurse Function
void Nurse::AddNewPatient(Patient patient)
{
    Sql_DB db;
    int patient_id = 0, appointment_id = 0;
    bool isExist = false, correctFormat = true, detectEmpty = false;
    string q1 = "SELECT * FROM patient WHERE IC = '";
    string q2 = "'";
    string errorType;
    Get_Row_Count(q1 + patient.IC + q2) > 0 ? isExist = true : isExist = false;

    if (!isValidDateOfBirth(patient.DOB) || !isValidDateOfBirth(patient.Appoint_Date)) {
        correctFormat = false;
        errorType = "Invalid Date Format. ";
    }
    else if (!IsValidEmail(patient.Email)) {
        correctFormat = false;
        errorType = "Invalid Email Format. ";
    }
    else if (!IsValidContactNumber(patient.Contact_Number)) {
        correctFormat = false;
        errorType = "Invalid Contact Number Format. ";
    }

    db.PrepareStatement("SELECT MAX(Patient_Id) AS patientCount FROM patient");
    db.QueryResult();

    while (db.result->next())
    {
        patient_id = stoi(db.result->getString("patientCount")) + 1; //Get the digit from patient_id, +1 for adding new patient into the database
    }

    db.PrepareStatement("SELECT MAX(Appointment_Id) AS appointmentCount FROM appointment");
    db.QueryResult();

    while (db.result->next())
    {
        //Get the digit from appointment_id, +1 for adding new appointment into the database
        try
        {
            appointment_id = stoi(db.result->getString("appointmentCount")) + 1;
        }
        catch (const std::exception&)
        {
            appointment_id = 1;
        }
    }

    if (IsEmpty(patient.Name) || IsEmpty(patient.DOB) || IsEmpty(patient.Gender) || IsEmpty(patient.Race) || IsEmpty(patient.Religion) || IsEmpty(patient.Address) || IsEmpty(patient.IC) || IsEmpty(patient.Appoint_With)) {
        detectEmpty = true;
    }
    

    if (correctFormat && !detectEmpty) {

        // Appointment time schedule
        system("cls");
        Sql_DB db;
        cout << "Appointment Time Schedule\n"
            << "=======================================================================================\n\n";

        // Get time schedule for that doctor on that room
        db.PrepareStatement("SELECT Room_Id, Time_from, Time_to, CASE WHEN Time_from < Time_to THEN (HOUR(Time_to) * 60 + MINUTE(Time_to) - HOUR(Time_from) * 60 - MINUTE(Time_from)) / 30 ELSE ((HOUR(Time_to) * 60 + MINUTE(Time_to) + 24 * 60) - (HOUR(Time_from) * 60 + MINUTE(Time_from))) / 30 END AS num_appointments FROM room WHERE Staff_Id = ?");
        db.statement->setString(1, patient.Appoint_With);
        db.QueryResult();
        db.result->next();

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
        vector<string> timeSlots;
        string q1 = "[slot ";
        string q2 = "]";

        std::cout << "Time slots:" << std::endl;
        for (int currentMinutes = startMinutes; currentMinutes < endMinutes; currentMinutes += 30) {
            int currentHour = (currentMinutes / 60) % 24;
            int currentMinute = currentMinutes % 60;
            //// Corrected printing logic with proper formatting and alignment
            //std::cout << "     " << q1 + std::to_string(slot) + q2 << "     "
            //    << std::setfill('0') << std::setw(2) << currentHour << ":"
            //    << std::setfill('0') << std::setw(2) << currentMinute
            //    << ":00" << std::endl;

            std::stringstream ss;
            ss << std::setfill('0') << std::setw(2) << currentHour << ":" << std::setfill('0') << std::setw(2) << currentMinute << ":00";
            timeSlots.push_back(ss.str());
            slot++;
        }

        // Check is the time slots empty
        // Find the target date in the vector
        db.PrepareStatement("SELECT * FROM appointment WHERE Date = ?");
        db.statement->setString(1, patient.Appoint_Date);
        db.QueryResult();

        while (db.result->next()) {
            auto it = std::find(timeSlots.begin(), timeSlots.end(), db.result->getString("Time"));

            // If the date is found, erase it from the vector
            if (it != timeSlots.end()) {
                timeSlots.erase(it);
                slot--;
            }
        }

        for (size_t i = 0; i < timeSlots.size(); i++) {
            cout << "     " << q1 + to_string(i) + q2 << "     "
                << timeSlots[i] << endl;
        }

        cout << "\n\n\n\n";

        char input[3] = {};
        bool isConfirm = false;
        int option = 0, appointment_slot = 0;
        string selected = BWHITE_TEXT + BOLD;
        string not_selected = GRAY_TEXT;

        auto erase_line = [](int start, int end) -> void{
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
        };

        auto Input = [](const int size, int& currentIndex, char input[], char keyIn) -> void {
            if (currentIndex < size - 1)
            {
                input[currentIndex] = keyIn;
                currentIndex++;
            }
        };

        auto Erase_Elements = [](char input[], int& currentIndex) -> void {
            if (currentIndex > 0)
            {
                currentIndex--;
                input[currentIndex] = '\0';
            }
        };

        auto Selected_Input = [Input,Erase_Elements](bool& exit, const int size, char input[], int& salary) -> void {
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

        };


        while (!isConfirm) {
            cout << (option == 0 ? selected : not_selected) << "     Appointment Slots: " << input << RESET << "\n\n";
            cout << (option == 1 ? selected + ">" : not_selected) << "     Confirm Appointment" << RESET << "\n\n\n";


            switch (_getch()) {
            case KEY_UP:
                if (option > 0) { option--; }
                break;

            case KEY_DOWN:
                if (option < 1) { option++; }
                break;

            case KEY_ENTER:
                switch (option)
                {
                    case 0: {
                        int size = sizeof(input) / sizeof(input[0]);
                        bool exit = false;

                        while (!exit) {
                            erase_line(0, 5);
                            cout << (option == 0 ? selected + ">" : not_selected) << "     Appointment Slots: " << appointment_slot << RESET << "\n\n";
                            cout << (option == 1 ? selected + ">" : not_selected) << "     Confirm Appointment" << RESET << "\n\n\n";
                            Selected_Input(exit, size, input, appointment_slot);
                        }
                        break;
                    }
                    
                    case 1: {
                        if (appointment_slot >= 0 && appointment_slot < slot) {
                            //Insert patient
                            db.PrepareStatement("INSERT INTO patient VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, DEFAULT)");
                            db.statement->setString(1, to_string(patient_id));
                            IsEmpty(patient.Name) ? db.statement->setNull(2, sql::DataType::VARCHAR) : db.statement->setString(2, patient.Name);
                            db.statement->setString(3, patient.DOB);
                            IsEmpty(patient.Gender) ? db.statement->setNull(4, sql::DataType::VARCHAR) : db.statement->setString(4, patient.Gender);
                            IsEmpty(patient.Race) ? db.statement->setNull(5, sql::DataType::VARCHAR) : db.statement->setString(5, patient.Race);
                            IsEmpty(patient.Religion) ? db.statement->setNull(6, sql::DataType::VARCHAR) : db.statement->setString(6, patient.Religion);
                            IsEmpty(patient.Address) ? db.statement->setNull(7, sql::DataType::VARCHAR) : db.statement->setString(7, patient.Address);
                            // IsEmpty(patient.IC) ? db.statement->setNull(8, sql::DataType::VARCHAR) : db.statement->setString(8, patient.IC);

                            if (IsEmpty(patient.IC)) {
                                db.statement->setNull(8, sql::DataType::VARCHAR);
                            }
                            else if (isExist) {
                                db.statement->setNull(8, sql::DataType::VARCHAR);
                                cout << "Patient already exist. ";
                            }
                            else
                            {
                                db.statement->setString(8, patient.IC);
                            }

                            IsEmpty(patient.Contact_Number) ? db.statement->setNull(9, sql::DataType::VARCHAR) : db.statement->setString(9, patient.Contact_Number);
                            IsEmpty(patient.Email) ? db.statement->setNull(10, sql::DataType::VARCHAR) : db.statement->setString(10, patient.Email);

                            //Prevent run the Add new patient only
                            if (IsEmpty(patient.Appoint_With))
                            {
                                db.statement->setNull(10, sql::DataType::VARCHAR);
                            }

                            db.statement->executeUpdate();

                            db.PrepareStatement("INSERT INTO appointment VALUES(?, ?, ?, ?, ?, DEFAULT)");
                            db.statement->setString(1, to_string(appointment_id));
                            db.statement->setString(2, to_string(patient_id));
                            IsEmpty(patient.Appoint_With) ? db.statement->setNull(3, sql::DataType::VARCHAR) : db.statement->setString(3, patient.Appoint_With);
                            db.statement->setString(4, patient.Appoint_Date);
                            db.statement->setString(5, timeSlots.at(appointment_slot));
                            db.statement->executeUpdate();

                            isConfirm = true;
                        }
                        else
                        {
                            cout << "Invalid input.";
                            _getch();
                        }
                        break;
                    }
                }
                break;

            case KEY_ESCAPE:
                isConfirm = true;
                throw exception("Back");
                break;
            }
            erase_line(0, 5);
        }
    }
    else
    {
        cout << errorType;
        throw sql::SQLException();
    }

}

void Nurse::AdmitFormerPatient(string patient_no, char appointment_with[], char appointment_date[])
{
    Sql_DB db;
    string query;
    int appointment_id = 0;

    db.PrepareStatement("SELECT MAX(Appointment_Id) AS appointmentCount FROM appointment");
    db.QueryResult();

    while (db.result->next())
    {
        //Get the digit from appointment_id, +1 for adding new appointment into the database
        try
        {
            appointment_id = stoi(db.result->getString("appointmentCount")) + 1;
        }
        catch (const std::exception&)
        {
            appointment_id = 1;
        }
    }

    // Appointment time schedule
    system("cls");
    cout << "Appointment Time Schedule\n"
        << "=======================================================================================\n\n";

    // Get time schedule for that doctor on that room
    db.PrepareStatement("SELECT Room_Id, Time_from, Time_to, CASE WHEN Time_from < Time_to THEN (HOUR(Time_to) * 60 + MINUTE(Time_to) - HOUR(Time_from) * 60 - MINUTE(Time_from)) / 30 ELSE ((HOUR(Time_to) * 60 + MINUTE(Time_to) + 24 * 60) - (HOUR(Time_from) * 60 + MINUTE(Time_from))) / 30 END AS num_appointments FROM room WHERE Staff_Id = ?");
    db.statement->setString(1, appointment_with);
    db.QueryResult();
    db.result->next();

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
    vector<string> timeSlots;
    string q1 = "[slot ";
    string q2 = "]";

    std::cout << "Time slots:" << std::endl;
    for (int currentMinutes = startMinutes; currentMinutes < endMinutes; currentMinutes += 30) {
        int currentHour = (currentMinutes / 60) % 24;
        int currentMinute = currentMinutes % 60;

        std::stringstream ss;
        ss << std::setfill('0') << std::setw(2) << currentHour << ":" << std::setfill('0') << std::setw(2) << currentMinute << ":00";
        timeSlots.push_back(ss.str());
        slot++;
    }

    // Check is the time slots empty
    // Find the target date in the vector
    db.PrepareStatement("SELECT * FROM appointment WHERE Date = ?");
    db.statement->setString(1, appointment_date);
    db.QueryResult();

    while (db.result->next()) {
        auto it = std::find(timeSlots.begin(), timeSlots.end(), db.result->getString("Time"));

        // If the date is found, erase it from the vector
        if (it != timeSlots.end()) {
            timeSlots.erase(it);
            slot--;
        }
    }

    for (size_t i = 0; i < timeSlots.size(); i++) {
        cout << "     " << q1 + to_string(i) + q2 << "     "
            << timeSlots[i] << endl;
    }

    cout << "\n\n\n\n";

    char input[3] = {};
    bool isConfirm = false;
    int option = 0, appointment_slot = 0;
    string selected = BWHITE_TEXT + BOLD;
    string not_selected = GRAY_TEXT;

    auto erase_line = [](int start, int end) -> void {
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
        };

    auto Input = [](const int size, int& currentIndex, char input[], char keyIn) -> void {
        if (currentIndex < size - 1)
        {
            input[currentIndex] = keyIn;
            currentIndex++;
        }
        };

    auto Erase_Elements = [](char input[], int& currentIndex) -> void {
        if (currentIndex > 0)
        {
            currentIndex--;
            input[currentIndex] = '\0';
        }
        };

    auto Selected_Input = [Input, Erase_Elements](bool& exit, const int size, char input[], int& salary) -> void {
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

        };

    while (!isConfirm) {
        cout << (option == 0 ? selected : not_selected) << "     Appointment Slots: " << input << RESET << "\n\n";
        cout << (option == 1 ? selected + ">" : not_selected) << "     Confirm Appointment" << RESET << "\n\n\n";


        switch (_getch()) {
        case KEY_UP:
            if (option > 0) { option--; }
            break;

        case KEY_DOWN:
            if (option < 1) { option++; }
            break;

        case KEY_ENTER:
            switch (option)
            {
            case 0: {
                int size = sizeof(input) / sizeof(input[0]);
                bool exit = false;

                while (!exit) {
                    erase_line(0, 5);
                    cout << (option == 0 ? selected + ">" : not_selected) << "     Appointment Slots: " << appointment_slot << RESET << "\n\n";
                    cout << (option == 1 ? selected + ">" : not_selected) << "     Confirm Appointment" << RESET << "\n\n\n";
                    Selected_Input(exit, size, input, appointment_slot);
                }
                break;
            }

            case 1: {
                if (appointment_slot >= 0 && appointment_slot < slot) {
                    
                    //Create new appointment
                    db.PrepareStatement("INSERT INTO appointment VALUES(?, ?, ?, ?, ?, DEFAULT)");
                    db.statement->setString(1, to_string(appointment_id));
                    db.statement->setString(2, patient_no);
                    db.statement->setString(3, appointment_with);
                    db.statement->setString(4, appointment_date);
                    db.statement->setString(5, timeSlots.at(appointment_slot));
                    db.statement->executeUpdate();


                    //Update the patient's status
                    query = "UPDATE patient SET Status = 'Admitted' WHERE patient_id = '" + patient_no + "'";
                    db.PrepareStatement(query);
                    db.statement->executeUpdate();

                    isConfirm = true;
                }
                else
                {
                    cout << "Invalid input.";
                    _getch();
                }
                break;
            }
            }
            break;

        case KEY_ESCAPE:
            isConfirm = true;
            throw exception("Back");
            break;
        }
        erase_line(0, 5);
    }




    

}

void Nurse::UpdatePatientInfo(Patient patient)
{
    Sql_DB db;
    bool isValid = true;

    // Construct the SQL query dynamically
    string query = "UPDATE patient SET Name = ?, DOB = ?, Gender = ?, Race = ?, Religion = ?, Address = ?, Contact_Number = ?, Email = ? WHERE Patient_Id = '" + patient.Get_PatientId() + "'"; //IC cnt be modified

    db.PrepareStatement(query);

    if (db.statement)
    {
        IsEmpty(patient.Name) ? db.statement->setNull(1, sql::DataType::VARCHAR) : db.statement->setString(1, patient.Name);
        IsEmpty(patient.DOB) ? db.statement->setNull(2, sql::DataType::VARCHAR) : db.statement->setString(2, patient.DOB);
        IsEmpty(patient.Gender) ? db.statement->setNull(3, sql::DataType::VARCHAR) : db.statement->setString(3, patient.Gender);
        IsEmpty(patient.Race) ? db.statement->setNull(4, sql::DataType::VARCHAR) : db.statement->setString(4, patient.Race);
        IsEmpty(patient.Religion) ? db.statement->setNull(5, sql::DataType::VARCHAR) : db.statement->setString(5, patient.Religion);
        IsEmpty(patient.Address) ? db.statement->setNull(6, sql::DataType::VARCHAR) : db.statement->setString(6, patient.Address);
        IsEmpty(patient.Contact_Number) ? db.statement->setNull(7, sql::DataType::VARCHAR) : db.statement->setString(7, patient.Contact_Number);
        IsEmpty(patient.Email) ? db.statement->setNull(8, sql::DataType::VARCHAR) : db.statement->setString(8, patient.Email);
        db.statement->executeUpdate();
    }

}

void Nurse::UpdateNurseOnWard(string staff_no, string ward_no)
{
    Sql_DB db;
    string query = "UPDATE ward SET Staff_Id = ? WHERE Ward_Id = ?";
    db.PrepareStatement(query);
    staff_no.empty() ? db.statement->setNull(1, sql::DataType::VARCHAR) : db.statement->setString(1, staff_no);
    db.statement->setString(2, ward_no);
    db.QueryStatement();
}

void Nurse::BedAdmit(string bed_no, string patient_no, char date[])
{
    Sql_DB db;
    string query = "UPDATE bed SET Patient_Id = ?, Admission_Date = ?, Availability = 'N' WHERE Bed_Id = ?";
    bool valid = true;
    string message;

    if (patient_no.empty() || IsEmpty(date))
    {
        valid = false;
    }
    else
    {
        try
        {
            db.PrepareStatement(query);
            patient_no.empty() ? db.statement->setNull(1, sql::DataType::VARCHAR) : db.statement->setString(1, patient_no);
            patient_no.empty() ? db.statement->setNull(2, sql::DataType::DATE) : db.statement->setString(2, string(date));
            db.statement->setString(3, bed_no);
            db.statement->executeUpdate();
        }
        catch (sql::SQLException& e)
        {
            valid = false;
        }
    }

    valid ? message = "Input successful" : message = "Invalid Input";
    cout << message;
}
/*====================================================================================================================*/



/**********************************************************************************************************************/
// Doctor Function
void Doctor::AddPatientRecord(char doctor_id[], char patient_id[], string appointment_no, Patient_Record pr, string& message)
{
    Sql_DB db;
    int row = 1;
    db.PrepareStatement("SELECT * FROM patient_record");
    db.QueryResult();

    if (db.result != nullptr) //Avoid nullptr
    {
        while (db.result->next())
        {
            row++;
        }
    }

    string query;
    bool valid = true;

    IsEmpty(pr.Description) ? query = "INSERT INTO patient_record VALUES(?, ?, ?, ? ,? ,? ,? ,? ,DEFAULT ,DEFAULT)" 
                            : query = "INSERT INTO patient_record VALUES(?, ?, ?, ? ,? ,? ,? ,? ,? ,DEFAULT)";

    try
    {
        db.PrepareStatement(query);
        db.statement->setString(1, to_string(row));
        db.statement->setString(2, patient_id);
        db.statement->setString(3, doctor_id);
        IsEmpty(pr.date) ? db.statement->setNull(4, sql::DataType::VARCHAR) : db.statement->setString(4, pr.date);
        IsEmpty(pr.diagnosis) ? db.statement->setNull(5, sql::DataType::VARCHAR) : db.statement->setString(5, pr.diagnosis);
        IsEmpty(pr.treatment) ? db.statement->setNull(6, sql::DataType::VARCHAR) : db.statement->setString(6, pr.treatment);
        db.statement->setDouble(7, pr.treatment_price);
        db.statement->setDouble(8, pr.doctor_charges);

        if (!IsEmpty(pr.Description))
        {
            db.statement->setString(9, pr.Description);
        }
        db.statement->executeUpdate();
    }
    catch (sql::SQLException& e)
    {
        valid = false;
    }


    if (valid)
    {
        message = "Successfully added Patient's Record";

        //Changes the appointment status from "Scheduled" to "Completed"
        db.PrepareStatement("UPDATE appointment SET Status = 'Completed' WHERE Appointment_Id = ?");
        db.statement->setString(1, appointment_no);
        db.statement->executeUpdate();

    }
    else
    {
        message = "Invalid Input";
    }
}

void Doctor::UpdatePatientRecord(char doctor_id[], char patient_id[], string record_no, Patient_Record pr, string& message)
{
    Sql_DB db;

    string query;
    bool valid = true;

    (pr.payment_status[0] == 'N') ? query = "UPDATE patient_record SET Date = ?, Diagnosis = ?, Treatment = ?, Treatment_Price = ?, Doctor_Charges = ?, Description = ? WHERE Record_Id = ?"
                                  : query = "UPDATE patient_record SET Date = ?, Diagnosis = ?, Treatment = ?, Description = ? WHERE Record_Id = ?"; //If the bill is paid, price cnt be changed

    try
    {
        db.PrepareStatement(query);
        IsEmpty(pr.date) ? db.statement->setNull(1, sql::DataType::VARCHAR) : db.statement->setString(1, pr.date);
        IsEmpty(pr.diagnosis) ? db.statement->setNull(2, sql::DataType::VARCHAR) : db.statement->setString(2, pr.diagnosis);
        IsEmpty(pr.treatment) ? db.statement->setNull(3, sql::DataType::VARCHAR) : db.statement->setString(3, pr.treatment);

        if (pr.payment_status[0] == 'N')
        {
            db.statement->setDouble(4, pr.treatment_price);
            db.statement->setDouble(5, pr.doctor_charges);
            IsEmpty(pr.Description) ? db.statement->setNull(6, sql::DataType::VARCHAR) : db.statement->setString(6, pr.Description);
            db.statement->setString(7, record_no);
        }
        else
        {
            IsEmpty(pr.Description) ? db.statement->setNull(4, sql::DataType::VARCHAR) : db.statement->setString(4, pr.Description);
            db.statement->setString(5, record_no);
        }

        db.statement->executeUpdate();
    }
    catch (sql::SQLException& e)
    {
        valid = false;
    }

    valid ? message = "Successfully update Patient's Record"
          : message = "Invalid Input";

}

void Doctor::UpdateConsultationRoom(string room_no, string doctor_no)
{
    Sql_DB db;
    string query = "UPDATE room SET Staff_id = ? WHERE Room_Id = ?";

    db.PrepareStatement(query);
    doctor_no.empty() ? db.statement->setNull(1, sql::DataType::VARCHAR) : db.statement->setString(1, doctor_no);
    db.statement->setString(2, room_no);
    db.statement->executeUpdate();
}
/*====================================================================================================================*/



/**********************************************************************************************************************/
// Pharmacist Function
void Pharmacist::Update_TotalMedicationPrice(string pharmacyRecord_no)
{
    Sql_DB db;
    float total_price = 0;
    db.PrepareStatement("SELECT MR.Quantity, M.Medication_Price FROM medication_record MR LEFT JOIN medication M ON MR.Medication_Id = M.Medication_Id WHERE PharmacyRecord_Id = ?");
    db.statement->setString(1, pharmacyRecord_no);
    db.QueryResult();

    if (db.result)
    {
        while (db.result->next())
        {
            total_price += (db.result->getDouble("Medication_Price") * db.result->getInt("Quantity"));
        }
    }

    db.PrepareStatement("UPDATE pharmacy_record SET Total_Medication_Price = ? WHERE PharmacyRecord_Id = ?");
    db.statement->setDouble(1, total_price);
    db.statement->setString(2, pharmacyRecord_no);
    db.QueryStatement();
}

void Pharmacist::AddPharmacyRecord(string record_no)
{
    Sql_DB db;
    string patient_no;
    db.PrepareStatement("SELECT Patient_Id from patient_record WHERE Record_Id = '" + record_no + "'");
    db.QueryResult();
    db.result->next();
    patient_no = db.result->getString("Patient_Id");

    string query = "INSERT INTO pharmacy_record VALUES(?, ?, NULL, ?, ?, 'N')";
    db.PrepareStatement(query);
    db.statement->setString(1, to_string(Get_Row_Count("SELECT * FROM pharmacy_record") + 1));
    db.statement->setString(2, patient_no);
    db.statement->setString(3, Staff_Id);
    db.statement->setString(4, record_no);
    db.QueryStatement();
}

string Pharmacist::AddMedicationRecord(int medication_no, int quantity ,string pharmacyRecord_no)
{
    Sql_DB db;
    string message;
    bool valid = true;
    int stock;

    if (medication_no > 0) //Check whether the medication_no is inserted
    {
        db.PrepareStatement("SELECT Quantity FROM medication WHERE Medication_Id = '" + to_string(medication_no) + "'");
        db.QueryResult();
        db.result->next();
        stock = db.result->getInt("Quantity");

        if (quantity > 0 && stock - quantity >= 0) //Check whether the quantity is out of stock
        {
            string query = "INSERT INTO medication_record VALUES(?, ?, ?, ?)";
            db.PrepareStatement(query);
            db.statement->setString(1, to_string(Get_Row_Count("SELECT * FROM medication_record") + 1));
            db.statement->setString(2, to_string(medication_no));
            db.statement->setInt(3, quantity);
            db.statement->setString(4, pharmacyRecord_no);
            db.QueryStatement();

            //Update the stock
            db.PrepareStatement("UPDATE medication SET quantity = '" + to_string(stock - quantity) + "'" + " WHERE Medication_Id = '" + to_string(medication_no) + "'");
            db.QueryStatement();

            message = "Input successfull.";
            Update_TotalMedicationPrice(pharmacyRecord_no);
        }
        else
        {
            if(quantity == 0){ message = "Invalid input. Please insert quantity."; }
            else if(stock - quantity < 0){ message = "Invalid input. Not enough medication stock"; }
           
        }
    }
    else
    {
        message = "Invalid input. Please insert medication";
    }
    
    cout << message << endl;
    _getch();
    
    return message;
}

string Pharmacist::UpdateMedicationRecord(int medication_no, int quantity, string medicationRecord_no)
{
    Sql_DB db;
    string message;
    string old_medication_no, pharmacyRecord_no;
    bool Is_Same;
    int old_stock, stock, old_quantity;

    if (medication_no > 0) //Check whether the medication_no is inserted
    {
        //Get old data
        db.PrepareStatement("SELECT Medication_Id, Quantity FROM medication_record WHERE MedicationRecord_Id = '" + medicationRecord_no + "'");
        db.QueryResult();
        db.result->next();
        old_medication_no = db.result->getString("Medication_Id");
        old_quantity = db.result->getInt("Quantity");
        old_medication_no == to_string(medication_no) ? Is_Same = true : Is_Same = false;

        if (Is_Same)
        {
            //Get current quantity
            db.PrepareStatement("SELECT * FROM medication WHERE Medication_Id = '" + to_string(medication_no) + "'");
            db.QueryResult();
            db.result->next();
            stock = db.result->getInt("Quantity");
            stock += old_quantity; //Add the old sold quantity of medication to the stock
        }
        else
        {
            //Get old quantity
            db.PrepareStatement("SELECT * FROM medication WHERE Medication_Id = '" + old_medication_no + "'");
            db.QueryResult();
            db.result->next();
            old_stock = db.result->getInt("Quantity");
            old_stock += old_quantity; //Add the old sold quantity of medication to the stock

            //Get new quantity
            db.PrepareStatement("SELECT * FROM medication WHERE Medication_Id = '" + to_string(medication_no) + "'");
            db.QueryResult();
            db.result->next();
            stock = db.result->getInt("Quantity");
        }


        if (quantity > 0 && stock - quantity >= 0) //Check whether the quantity is out of stock
        {
            //Update medication record with newest information
            string query = "UPDATE medication_record SET Medication_Id = ?,Quantity = ? WHERE MedicationRecord_Id = ?;";
            db.PrepareStatement(query);
            db.statement->setString(1, to_string(medication_no));
            db.statement->setInt(2, quantity);
            db.statement->setString(3, medicationRecord_no);
            db.QueryStatement();

            //Update the stock
            if (Is_Same)
            {
                db.PrepareStatement("UPDATE medication SET quantity = '" + to_string(stock - quantity) + "'" + " WHERE Medication_Id = '" + to_string(medication_no) + "'");
                db.QueryStatement();
            }
            else
            {
                //Give back the old medication quantity to the stock
                db.PrepareStatement("UPDATE medication SET quantity = '" + to_string(old_stock) + "'" + " WHERE Medication_Id = '" + old_medication_no + "'");
                db.QueryStatement();

                //Update the stock for the newest medication
                db.PrepareStatement("UPDATE medication SET quantity = '" + to_string(stock - quantity) + "'" + " WHERE Medication_Id = '" + to_string(medication_no) + "'");
                db.QueryStatement();
            }

            message = "Update successfull.";

            //Get pharmacy record
            db.PrepareStatement("SELECT PharmacyRecord_Id FROM medication_record WHERE MedicationRecord_Id = ?");
            db.statement->setString(1, medicationRecord_no);
            db.QueryResult();
            db.result->next();
            pharmacyRecord_no = db.result->getString("PharmacyRecord_Id");
            Update_TotalMedicationPrice(pharmacyRecord_no);
        }
        else
        {
            if (quantity == 0) { message = "Invalid update. Please insert quantity."; }
            else if (stock - quantity < 0) { message = "Invalid update. Not enough medication stock"; }

        }
    }
    else
    {
        message = "Invalid update. Please insert medication";
    }

    cout << message << endl;
    _getch();

    return message;
}

void Pharmacist::DeleteMedicationRecord(string medicationRecord_no)
{
    Sql_DB db;
    string pharmacyRecord_no, medication_no;
    int quantity;

    //Get the PharmacyRecord_Id
    db.PrepareStatement("SELECT PharmacyRecord_Id, Quantity, Medication_Id FROM medication_record WHERE MedicationRecord_Id = ?");
    db.statement->setString(1, medicationRecord_no);
    db.QueryResult();
    db.result->next();
    pharmacyRecord_no = db.result->getString("PharmacyRecord_Id");
    medication_no = db.result->getString("Medication_Id");
    quantity = db.result->getInt("Quantity");

    //Get back the medication to the stock
    db.PrepareStatement("UPDATE medication SET Quantity = Quantity + ? WHERE Medication_Id = ?");
    db.statement->setInt(1, quantity);
    db.statement->setString(2, medication_no);
    db.QueryStatement();

    //Delete the medication record
    db.PrepareStatement("DELETE FROM medication_record WHERE MedicationRecord_Id = ?");
    db.statement->setString(1, medicationRecord_no);
    db.QueryStatement();

    Update_TotalMedicationPrice(pharmacyRecord_no);
}

string Pharmacist::RefillMedication(string medication_no, int refill_quantity, string date) // NEED CHANGE
{
    Sql_DB db;
    int stock, transaction_no;
    double buy_price;
    string medication_name;
    
    try
    {
        //Get current medication's stock
        db.PrepareStatement("SELECT Medication_Name, Buy_Price, Quantity FROM Medication WHERE Medication_Id = ?");
        db.statement->setString(1, medication_no);
        db.QueryResult();
        db.result->next();
        medication_name = db.result->getString("Medication_Name");
        buy_price = db.result->getDouble("Buy_Price");
        stock = db.result->getInt("Quantity");

        //Get current transaction_record no
        transaction_no = Get_Row_Count("SELECT * FROM transaction_record") + 1;

        //Add transaction record based on refill
        db.PrepareStatement("INSERT INTO transaction_record VALUES(?, ?, ?, ?)");
        db.statement->setInt(1, transaction_no);
        db.statement->setString(2, "Restock " + medication_name);
        db.statement->setString(3, date);
        db.statement->setDouble(4, buy_price * refill_quantity);
        db.statement->executeUpdate();

        //Add the refill quantity with the stock
        stock += refill_quantity;
        db.PrepareStatement("UPDATE medication SET Quantity = ? WHERE Medication_Id = ?;");
        db.statement->setInt(1, stock);
        db.statement->setString(2, medication_no);
        db.statement->executeUpdate();
    }
    catch (sql::SQLException& e)
    {
        return "Failed to refill";
    }

    return "Refill successfully!";
}

string Pharmacist::Create_NewMedication(char date[], char medication_Name[], float medication_price, float buy_price, float quantity)
{
    Sql_DB db;
    int transaction_no, medication_no;
    buy_price = (buy_price > 0) ? -buy_price : buy_price; // Make sure the buy price is negative
    
    // Get current transaction_record no and medication_no
    transaction_no = Get_Row_Count("SELECT * FROM transaction_record") + 1;
    medication_no = Get_Row_Count("SELECT * FROM medication") + 1;

    try
    {
        // Add new medication
        db.PrepareStatement("INSERT INTO medication VALUES(?, ?, ?, ?, ?)");
        db.statement->setString(1, to_string(medication_no));
        IsEmpty(medication_Name) ? db.statement->setNull(2, sql::DataType::VARCHAR) : db.statement->setString(2, medication_Name);
        db.statement->setDouble(3, medication_price);
        db.statement->setDouble(4, buy_price);
        db.statement->setInt(5, quantity);
        db.statement->executeUpdate();

        // Add transaction record
        db.PrepareStatement("INSERT INTO transaction_record VALUES(?, ?, ?, ?)");
        db.statement->setInt(1, transaction_no);
        db.statement->setString(2, "Add Medication " + string(medication_Name));
        db.statement->setString(3, date);
        db.statement->setDouble(4, buy_price * quantity);
        db.statement->executeUpdate();
    }
    catch (sql::SQLException& e)
    {
        return "Failed to create!";
    }

    return "Create successfully!";
}
/*====================================================================================================================*/