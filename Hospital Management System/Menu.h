#include <iostream>
#include <iterator>
#include "Account.h"
#include "ANSI.h"
#define KEY_UP 72    
#define KEY_DOWN 80  
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_DELETE 8
#define KEY_ENTER '\r'
#define KEY_ESCAPE 27

using namespace std;

#pragma once
class Menu
{
	public:
		Menu();
		~Menu();

		int Home_Menu();
		bool Login_Menu(Account& acc);
		void Registration_Menu();
		void Registration_Option_Menu(int option, string selected, string not_selected, Account& reg);

};

class Cleaner_Menu
{
	private:
		void Show_Room_Schedule(string room_schedule[][12], const int rooms, const int slots);
		void Show_Ward_Schedule(string ward_schedule[][12], const int rooms, const int wards);

	public:
		Cleaner_Menu() {}
		~Cleaner_Menu() {}
		int Operations_Menu(Cleaner usedAccount);
		void Room_Schedule(string room_schedule[][12], const int rooms, const int slots);
		void Ward_Schedule(string ward_schedule[][12], const int rooms, const int slots);
		void Selected_ViewSchedule(Cleaner cln, string room_schedule[][12], string ward_schedule[][12],const int rooms, const int wards,const int slots);
		void Selected_Manage(Cleaner cln, string room_schedule[][12], string ward_schedule[][12], const int rooms, const int wards,const int slots, string RoomOrWard);
		void AddRoom(Account account_detailed, string timeSet[], int currentRow, int currentColumn, bool& addingSelecting, bool& adding);
		void AddWard(Account account_detailed, string timeSet[], int currentRow, int currentColumn, bool& addingSelecting, bool& adding);
};

class HR_Menu
{
	private:
		void Register_Detail(string query, int number, Account& account_detailed);
		void Employee_Detail(string query, string number, Account& account_detailed);
		void Update_Option_Menu(int option, string selected, string not_selected, Account& reg);

	public:
		HR_Menu(){}
		~HR_Menu() {}
		int Operations_Menu(HR usedAccount);
		int Report_Menu(HR usedAccount);
		string Report_Select_Menu(int option);
		string Get_List(string query, string first_column, string second_column, string third_column, string tittle);
		void Selected_Registration_Request(HR hr, int registration_no);
		void Selected_Update_Employee(HR hr, string staff_no);
		void Selected_Delete_Employee(HR hr, string staff_no);
		void Selected_View_Employee(HR hr, bool& exit);
		void Selected_Transaction_Record(string year);
		void Selected_BusinessReport(string year);
		void Selected_GenerateBarChart(string year);
};

class Doctor_Menu
{
	private:
		void Patient_Record_Menu(Patient_Record pr, string message, int option, bool keyIn);

	public:
		Doctor_Menu() {}
		~Doctor_Menu() {}
		int Operations_Menu(Doctor usedAccount);
		string Get_List(string query, string first_column, string third_column, string tittle, string additionalOption);
		void Selected_Appointment(Doctor doc, string appointment_no);
		void Selected_Update_PatientRecord(Doctor doc, string record_no);
		void Selected_View_PatientRecord(Doctor doc, string record_no);
		void Selected_Consultation_Room(Doctor doc, string room_no);
};

class Nurse_Menu
{
	private:
		void PatientAdd_Option_Menu(int option, string selected, string not_selected, Patient& patient);
		void Update_Option_Menu(int option, string selected, string not_selected, Patient& patient);
		void ChangeOfNurse(string ward_no);
		void ManageBedAdmission(string ward_no);
		void Discharge_Menu(char date[], string payment_method, string card_no, int option);

	public:
		Nurse_Menu() {}
		~Nurse_Menu() {}
		int Operations_Menu(Nurse usedAccount);
		string Get_List(string query, string first_column, string third_column, string tittle, string additionalOption);
		void Patient_Menu(Patient patient);
		void PatientRecord_Menu(Patient patient);
		void Selected_PatientAdmit(string patient_no);
		void Selected_PatientAdd(string patient_no);
		void Selected_Update_Patient(string patient_no);
		void Selected_ManageWard(string ward_no);
		void Selected_Discharge(Nurse nr, string patientRecord_no);
};

class Pharmacist_Menu
{
	private:
		void Selected_AddMedicationRecord(Pharmacist pmc, string pharmacyRecord_no);
		void CreateMedication_Menu(int option, char date[], char medication_Name[], float medication_price, float buy_price, float quantity);

	public:
		Pharmacist_Menu(){}
		~Pharmacist_Menu(){}
		int Operations_Menu(Pharmacist usedAccount);
		string Get_List(string query, string first_column, string second_column, string third_column, string tittle, string additionalOption);
		void Selected_PharmacyRecord(Pharmacist pmc, string record_no);
		void Selected_MedicationRecord(Pharmacist pmc, string pharmacyRecord_no);
		void Selected_UpdateMedicationRecord(Pharmacist pmc, string medicationRecord_no);
		void Selected_CreateMedication(Pharmacist pmc);
};
