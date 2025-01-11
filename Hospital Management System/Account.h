#include <iostream>
#include <string>
#include "Sql_DB.h"
using namespace std;

#ifndef ACCOUNT_H
#define ACCOUNT_H

class Account
{
protected:
	char Staff_Id[12] = {};
	char Name[31] = {};
	char DOB[11] = {};
	char Gender[10] = {};
	char Race[31] = {};
	char Religion[31] = {};
	char Address[101] = {};
	char IC[13] = {};
	float Salary = 0.0f;
	char Role[31] = {};
	char Contact_Number[21] = {};
	char Email[46] = {};
	char Password[46] = {};
	char Manage_By[31] = {};
	char Approved_By[10] = {};
	char Status[20] = {};

public:
	Account();
	~Account();
	void Registration(string& message);
	bool Login();
	void Update(string& message);
	void Remove(string& message);
	string encrypt(const string text);
	string decrypt(const string text);
	string Get_Id();
	string Get_Role();
	bool Is_Manager();

	//Friend class declaration
	friend class Menu;
	friend class HR_Menu;
	friend class Cleaner_Menu;
	friend class Nurse_Menu;
	friend class Doctor_Menu;
};
#endif // !ACCOUNT_H


#ifndef Patient_H
#define Patient_H
class Patient : public Account
{
protected:
	char Patient_Id[12] = {};
	char Appoint_With[20] = {};

public:
	friend class Nurse;
	friend class Nurse_Menu;
	friend class Doctor;
	friend class Doctor_Menu;
	Patient() {}
	~Patient() {}
	void Patient_Detail(string query, string patient_no);
	string Get_PatientId() { return Patient_Id; }

};
#endif // !Patient_H


#ifndef PATIENT_RECORD_H
#define PATIENT_RECORD_H
class Patient_Record : public Patient
{
protected:
	char date[11] = {}, diagnosis[41] = {}, treatment[41] = {}, Description[101] = {}, payment_status[2] = {};
	float treatment_price = 0, doctor_charges = 0;
	void PatientRecord_Detail(string record_no);
	void Show_PatientRecord(string record_no);

public:
	friend class Nurse;
	friend class Nurse_Menu;
	friend class Doctor;
	friend class Doctor_Menu;
	Patient_Record() {}
	~Patient_Record() {}
};
#endif // !PATIENT_RECORD_H


#ifndef CLEANER_H
#define CLEANER_H
class Cleaner : public Account
{
	public:
		Cleaner() {}
		Cleaner(const Account& acc) : Account(acc) {}
		~Cleaner() {}
};
#endif // !CLEANER_H


#ifndef DOCTOR_H
#define DOCTOR_H
class Doctor : public Account
{
	protected:
		void AddPatientRecord(char doctor_id[], char patient_id[], string appointment_no, Patient_Record pr, string& message);
		void UpdatePatientRecord(char doctor_id[], char patient_id[], string record_no, Patient_Record pr, string& message);
		void UpdateConsultationRoom(string room_no, string doctor_no);

	public:
		friend class Doctor_Menu;
		Doctor() {}
		Doctor(const Account& acc) : Account(acc) {}
		~Doctor(){}
};
#endif // !DOCTOR_H


#ifndef HR_H
#define HR_H
class HR : public Account
{
	private:
		void Get_Detailed_Information(Account registered_acc);

	public:
		HR() {}
		HR(const Account& acc) : Account(acc) {}
		~HR(){}
};
#endif // !HR_H


#ifndef NURSE_H
#define NURSE_H
class Nurse : public Account
{
	protected:
		void AddNewPatient(Patient patient);
		void AdmitFormerPatient(string patient_no, char appointment_with[]);
		void UpdatePatientInfo(Patient patient);
		void UpdateNurseOnWard(string staff_no, string ward_no);
		void BedAdmit(string bed_no, string patient_no, char date[]);

	public:
		friend class Nurse_Menu;
		Nurse() {}
		Nurse(const Account& acc) : Account(acc) {}
		~Nurse() {}
};
#endif // !NURSE_H


#ifndef PHARMACIST_H
#define PHARMACIST_H
class Pharmacist : public Account
{
	protected:
		void Update_TotalMedicationPrice(string pharmacyRecord_no);
		void AddPharmacyRecord(string record_no);
		string AddMedicationRecord(int medication_no, int quantity, string pharmacyRecord_no);
		string UpdateMedicationRecord(int medication_no, int quantity, string medicationRecord_no);
		string RefillMedication(string medication_no, int refill_quantity, string date);
		string Create_NewMedication(char date[], char medication_Name[], float medication_price, float buy_price, float quantity);

	public:
		friend class Pharmacist_Menu;
		Pharmacist() {}
		Pharmacist(const Account& acc) : Account(acc) {}
		~Pharmacist() {}
		void DeleteMedicationRecord(string medicationRecord_no);
};
#endif // !PHARMACIST_H

