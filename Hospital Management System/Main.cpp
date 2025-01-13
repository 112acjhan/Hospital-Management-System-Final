#include <iostream>
#include <conio.h>
#include <mysql/jdbc.h>
#include "Sql_DB.h"
#include "Menu.h"
#include "Account.h"
#include "ANSI.h"
#include <Windows.h>
#define KEY_UP 72    
#define KEY_DOWN 80  
#define KEY_ENTER '\r'

using namespace std;

enum CleanerOperations
{
	VIEW_CLEANING_SCHEDULE = 0,
	MANAGE_ROOM_SCHEDULE,
	MANAGE_WARD_SCHEDULE,
	CLEANER_SIGN_OUT
};

enum DoctorOperations
{
	APPOINTMENT_WITH_PATIENT = 0,
	UPDATE_PATIENT_RECORD,
	VIEW_PATIENT_RECORD,
	MANAGE_CONSULTATION_ROOM,
	DOCTOR_SIGN_OUT
};

enum HrOperations
{
	REGISTRATION_REQUEST = 0,
	UPDATE_EMPLOYEE,
	DELETE_EMPLOYEE,
	VIEW_CURRENT_EMPLOYEE,
	GENERATE_REPORT,
	HR_SIGN_OUT
};

enum NurseOperations
{
	MANAGE_PATIENT_REGISTRATION = 0,
	UPDATE_PATIENT,
	VIEW_PATIENT,
	MANAGE_WARD,
	PATIENT_DISCHARGE,
	NURSE_SIGN_OUT
};

enum PharmacistOperations
{
	VIEW_MEDICATION_STOCK = 0,
	CREATE_MEDICATION_RECORD,
	MODIFY_MEDICATION_RECORD,
	CREATE_RESTOCK_MEDICATION_STOCK,
	PHARMACIST_SIGN_OUT
};

int main()
{
	bool exit = false, login = false;

	Sql_DB db;
	Account usedAcc;
	Cleaner cln;
	Doctor doc;
	HR hr;
	Nurse nr;
	Pharmacist pmc;

	Menu menu;
	Cleaner_Menu cleaner_menu;
	HR_Menu hr_menu;
	Doctor_Menu doctor_menu;
	Nurse_Menu nurse_menu;
	Pharmacist_Menu pharmacist_menu;


	do
	{
		//Login and Registration Menu
		system("cls");

		switch (menu.Home_Menu())
		{
		case 0:
			try
			{
				if (menu.Login_Menu(usedAcc)) { login = true; }
			}
			catch (sql::SQLException& e)
			{
				cout << "#ERR: SQLException in " << __FILE__
					<< "(" << __FUNCTION__ << ") on file " << __LINE__ << endl
					<< "# ERR: " << e.what()
					<< " (MySQL error code: " << e.getErrorCode()
					<< ", SQLState: " << e.getSQLState() << ")" << endl;
				_getch();
			}
			
			break;
		case 1:
			menu.Registration_Menu();
			break;
		case 2:
			exit = true;
			break;
		}

		//Staff Menu
		int role = -1;

		if (usedAcc.Get_Role() == "Cleaner") { role = 0; }
		else if (usedAcc.Get_Role() == "Doctor") { role = 1; }
		else if (usedAcc.Get_Role() == "HR") { role = 2; }
		else if (usedAcc.Get_Role() == "Nurse") { role = 3; }
		else if (usedAcc.Get_Role() == "Pharmacist") { role = 4; }

		if (login)
		{
			Account account_detailed;
			system("cls");
			bool sign_out = false;
			do
			{
				system("cls");
				switch (role)
				{
					case 0: //Cleaner Operations Menu
					{
						cln = Cleaner(usedAcc);
						bool selecting = true;

						// Define the number of rooms and time slots
						db.PrepareStatement("SELECT MAX(Room_Id) as roomCount FROM room");
						db.QueryResult();
						db.result->next();
						int rooms = stoi(db.result->getString("roomCount"));

						db.PrepareStatement("SELECT MAX(Ward_Id) as wardCount FROM ward");
						db.QueryResult();
						db.result->next();
						int wards = stoi(db.result->getString("wardCount"));

						const int timeSlots = 12;

						switch (cleaner_menu.Operations_Menu(cln))
						{
							case VIEW_CLEANING_SCHEDULE: //View cleaning schedule for both room and ward
							{
								//Initialize 2d array
								string (*room_schedule)[timeSlots] = new string[rooms][timeSlots];
								string (*ward_schedule)[timeSlots] = new string[wards][timeSlots];

								cleaner_menu.Room_Schedule(room_schedule, rooms, timeSlots);
								cleaner_menu.Ward_Schedule(ward_schedule, wards, timeSlots);
								cleaner_menu.Selected_ViewSchedule(cln, room_schedule, ward_schedule, rooms, wards, timeSlots);
								_getch();

								delete[] room_schedule;
								delete[] ward_schedule;
								break;
							}
							case MANAGE_ROOM_SCHEDULE: //Manage and adjust the room schedule
							{

								string(*room_schedule)[timeSlots] = new string[rooms][timeSlots];
								string(*ward_schedule)[timeSlots] = new string[wards][timeSlots];
								cleaner_menu.Selected_Manage(cln, room_schedule, ward_schedule, rooms, wards,timeSlots, "Room");

								delete[] room_schedule;
								delete[] ward_schedule;
								break;
							}
							case MANAGE_WARD_SCHEDULE: //Manage and adjust the ward schedule
							{
								string(*room_schedule)[timeSlots] = new string[rooms][timeSlots];
								string(*ward_schedule)[timeSlots] = new string[wards][timeSlots];
								cleaner_menu.Selected_Manage(cln, room_schedule, ward_schedule, rooms, wards,timeSlots, "Ward");

								delete[] room_schedule;
								delete[] ward_schedule;
								break;
							}
							case CLEANER_SIGN_OUT: //Sign out the staff menu
							{
								sign_out = true;
								break;
							}
						}

						break;
					}

					case 1: //Doctor Operations Menu
					{
						doc = Doctor(usedAcc);
						bool selecting = true;
						switch (doctor_menu.Operations_Menu(doc))
						{
							case APPOINTMENT_WITH_PATIENT: //Check current scheduled patient's record
							{
								do
								{
									//Join 2 table (Appointment and Patient table)
									string query = "SELECT A.Appointment_Id, P.Name AS 'Name', A.Status FROM appointment A JOIN patient P ON A.Patient_Id = P.Patient_Id WHERE A.Status = 'Scheduled' AND A.Staff_Id = '";

									string appointment_no = doctor_menu.Get_List(query + doc.Get_Id() + "'",
										"Appointment_Id", "Status",
										"                                            APPOINTMENT WITH PATIENT                                             ", "");

									if (appointment_no == "-1")
									{
										selecting = false;
									}
									else
									{
										doctor_menu.Selected_Appointment(doc, appointment_no);
									}

								} while (selecting);
								break;
							}
							case UPDATE_PATIENT_RECORD: //Modify completed appointment patient's record
							{
								do
								{
									string query = "SELECT PR.Record_Id, P.Name, PR.Payment_Status FROM patient P JOIN patient_record PR ON P.Patient_Id = PR.Patient_Id WHERE PR.Staff_Id =  '";

									string record_no = doctor_menu.Get_List(query + doc.Get_Id() + "'",
										"Record_Id", "Payment_Status",
										"                                             UPDATE PATIENT'S RECORD                                             ", "");

									if (record_no == "-1")
									{
										selecting = false;
									}
									else
									{
										doctor_menu.Selected_Update_PatientRecord(doc, record_no);
									}
								} while (selecting);
								break;
							}
							case VIEW_PATIENT_RECORD: //View all the patient's record that doctor did
							{
								do
								{
									string query = "SELECT PR.Record_Id, P.Name, PR.Payment_Status FROM patient P JOIN patient_record PR ON P.Patient_Id = PR.Patient_Id";

									string record_no = doctor_menu.Get_List(query,
										"Record_Id", "Payment_Status",
										"                                                 PATIENT'S RECORDS                                               ", "Search");

									if (record_no == "-1")
									{
										selecting = false;
									}
									else
									{
										doctor_menu.Selected_View_PatientRecord(doc, record_no);
									}

								} while (selecting);
								break;
							}
							case MANAGE_CONSULTATION_ROOM: //Only for Doctor who is manager, allow to see and add Room for Doctors
							{
								do
								{
									string query = "SELECT R.Room_Id, S.Name, R.Staff_Id FROM room R LEFT JOIN staff S ON R.Staff_Id = S.Staff_Id";

									string room_no = doctor_menu.Get_List(query,
										"Room_Id", "Staff_Id",
										"                                                 CONSULTATION ROOM                                               ", "Search");

									if (room_no == "-1")
									{
										selecting = false;
									}
									else
									{
										doctor_menu.Selected_Consultation_Room(doc, room_no);
									}

								} while (selecting);
								break;
							}
							case DOCTOR_SIGN_OUT:
							{
								sign_out = true;
								break;
							}
						}
						break;
					}

					case 2: //HR Operations Menu
					{
						hr = HR(usedAcc);
						bool selecting = true;
						switch (hr_menu.Operations_Menu(hr))
						{
							case REGISTRATION_REQUEST: //Manage the Registration Request, decide on Accept or Reject for the account registration
							{
								do
								{
									int registration_no = stoi(hr_menu.Get_List("SELECT * FROM registration", "Registration_Id", "Name", "Status",
										"                                            MANAGE REGISTRATION REQUEST                                          "));
									if (registration_no + 1 != 0)
									{
										hr_menu.Selected_Registration_Request(hr, registration_no);
									}
									else
									{
										selecting = false;
									}
								} while (selecting);
								selecting = true; //Reset the trigger
								break;
							}
							case UPDATE_EMPLOYEE: //Update employee information
							{
								do
								{
									string staff_no = hr_menu.Get_List("SELECT * FROM staff WHERE Status = 'Activate'", "Staff_Id", "Name", "Role",
										"                                             UPDATE EMPLOYEE LIST                                                ");

									if (staff_no == "-1") //Check if it return -1, if it is then change to empty string
									{
										staff_no = "";
									}

									if (!staff_no.empty())
									{
										hr_menu.Selected_Update_Employee(hr, staff_no);
									}
									else
									{
										selecting = false;
									}
								} while (selecting);
								selecting = true; //Reset the trigger
								break;
							}
							case DELETE_EMPLOYEE: //Delete Employee from the database
							{
								do
								{
									string staff_no = hr_menu.Get_List("SELECT * FROM staff WHERE Status = 'Activate'", "Staff_Id", "Name", "Role",
										"                                             DELETE EMPLOYEE LIST                                                ");

									if (staff_no == "-1") //Check if it return -1, if it is then change to empty string
									{
										staff_no = "";
									}

									if (!staff_no.empty())
									{
										hr_menu.Selected_Delete_Employee(hr, staff_no);
									}
									else
									{
										selecting = false;
									}
								} while (selecting);
								selecting = true; //Reset the trigger
								break;
							}
							case VIEW_CURRENT_EMPLOYEE: //View current employee
							{
								do
								{
									hr_menu.Selected_View_Employee(hr, selecting);
								} while (selecting);
								selecting = true; //Reset the trigger
								break;
							}
							case GENERATE_REPORT:
							{
								do
								{
									int option = hr_menu.Report_Menu(hr);
									string date = "";
									if (option == -1) { selecting = false; }
									else { date = hr_menu.Report_Select_Menu(option); }

									if (!date.empty())
									{
										switch (option)
										{
										case 0:
										{
											/*string transaction_no = hr_menu.Get_List("SELECT * FROM transaction_record WHERE Date LIKE '" + date + "%'",
												"Transaction_Id", "Description", "Total_Price",
												"                                             VIEW TRANSACTION RECORD                                             ");*/
											hr_menu.Selected_Transaction_Record(date);
											break;
										}
										case 1:
										{
											hr_menu.Selected_BusinessReport(date);
											break;
										}
										case 2:
											break;
										}
									}

								} while (selecting);
								break;
							}
							case HR_SIGN_OUT: //Sign out the staff menu
							{
								sign_out = true;
								break;
							}
						}
						break;
					}

					case 3: //Nurse Operations Menu
					{
						nr = Nurse(usedAcc);
						bool selecting = true;
						switch (nurse_menu.Operations_Menu(nr))
						{
							case MANAGE_PATIENT_REGISTRATION: //Manage Patient's Registration, included Add New Patient and Admit patient to the hospital
							{
								do
								{
									string patient_no = nurse_menu.Get_List("SELECT * FROM patient", "Patient_Id", "Status",
										"                                          MANAGE PATIENT REGISTRATIONS                                           ", "Add new patient");

									if (patient_no == "Add")
									{
										nurse_menu.Selected_PatientAdd(patient_no);
									}
									else if (!patient_no.empty())
									{
										nurse_menu.Selected_PatientAdmit(patient_no);
									}
									else
									{
										selecting = false;
									}

								} while (selecting);
								break;
							}
							case UPDATE_PATIENT: //Update the patient's information which recorded in the database
							{
								do
								{
									string patient_no = nurse_menu.Get_List("SELECT * FROM patient", "Patient_Id", "Status",
										"                                           UPDATE PATIENT INFORMATIONS                                           ", "");

									if (!patient_no.empty())
									{
										nurse_menu.Selected_Update_Patient(patient_no);
									}
									else
									{
										selecting = false;
									}

								} while (selecting);
								break;
							}
							case VIEW_PATIENT: //View Patient List
							{
								do
								{
									string patient_no = nurse_menu.Get_List("SELECT * FROM patient", "Patient_Id", "Status",
										"                                                 PATIENT LIST                                                    ", "Search");

									if (!patient_no.empty())
									{
										Patient patient;
										patient.Patient_Detail("SELECT * FROM patient WHERE Patient_Id = ?", patient_no);
										int optionView = 0;
										string selected = BWHITE_TEXT + BOLD;
										string not_selected = GRAY_TEXT;
										bool exit = false;

										do //Select certain patient, let user decided to view Patient's Information or Record
										{
											cout << (optionView == 0 ? selected + "> " : not_selected) << "View Patient's Information" << RESET << endl;
											cout << (optionView == 1 ? selected + "> " : not_selected) << "View Patient's Record" << RESET << endl;

											switch (_getch())
											{
												case KEY_UP:
												{
													if (optionView > 0)
													{
														optionView--;
													}
													break;
												}
												case KEY_DOWN:
												{
													if (optionView < 1)
													{
														optionView++;
													}
													break;
												}
												case KEY_ESCAPE:
												{
													exit = true;
													break;
												}
												case KEY_ENTER:
												{
													if (optionView == 0) //View Patient's Information
													{
														system("cls");
														exit = true;
														nurse_menu.Patient_Menu(patient);
														_getch();
													}
													else if (optionView == 1) //View Patient's Record
													{
														system("cls");
														exit = true;
														nurse_menu.PatientRecord_Menu(patient);
													}
												}
											}

											//Erase_Lines(0, 2);
											if (0 < 2)
											{
												for (int i = 0; i <= 2; ++i)
												{
													cout << "\x1b[2K"; // Delete current line
													if (i < 2)
													{
														cout << "\x1b[A"; // Move up
													}
												}
											}
										} while (!exit);

									}
									else
									{
										selecting = false;
									}
								} while (selecting);
								break;
							}
							case MANAGE_WARD:
							{
								do
								{
									string ward_no = nurse_menu.Get_List("SELECT W.Ward_Id, S.Name, W.Capacity FROM ward W LEFT JOIN staff S ON W.Staff_Id = S.Staff_Id",
									"Ward_Id", "Capacity",
									"                                                   MANAGE WARD                                                   ", "");
									
									if (!ward_no.empty())
									{
										nurse_menu.Selected_ManageWard(ward_no);
									}
									else
									{
										selecting = false;
									}

								} while (selecting);
								break;
							}
							case PATIENT_DISCHARGE:
							{
								do
								{
									string patientRecord_no = nurse_menu.Get_List("SELECT Record_Id AS PR_Id, Name, Date FROM patient_record PR LEFT JOIN patient P ON PR.Patient_Id = P.Patient_Id WHERE Payment_Status = 'N'",
										"PR_Id", "Date",
										"                                                   PATIENT RECORD                                                ", "Discharge");

									if (!patientRecord_no.empty()) { nurse_menu.Selected_Discharge(nr, patientRecord_no); }
									else { selecting = false; }

								} while (selecting);
								break;
							}
							case NURSE_SIGN_OUT:
							{
								sign_out = true;
								break;
							}
						}
						break;
					}

					case 4: //Pharmacist Operations Menu
					{
						pmc = Pharmacist(usedAcc);
						bool selecting = true;

						switch (pharmacist_menu.Operations_Menu(pmc))
						{
							case VIEW_MEDICATION_STOCK:
							{
								do
								{
									string medicine_no = pharmacist_menu.Get_List("SELECT * FROM medication", "Medication_Id", "Medication_Name", "Quantity",
										"                                              VIEW MEDICATION STOCK                                              ", "");
									
									if (medicine_no.empty())
									{
										selecting = false;
									}

								} while (selecting);
								break;
							}
							case CREATE_MEDICATION_RECORD:
							{
								do
								{
									string record_no = pharmacist_menu.Get_List("SELECT PR.Record_Id, P.Name, PR.Payment_Status FROM patient_record PR LEFT JOIN patient P ON PR.Patient_Id = P.Patient_Id"
										                                          , "Record_Id", "Name", "Payment_Status",
										"                                                 PATIENT'S RECORD                                                ", "");

									if (!record_no.empty())
									{
										pharmacist_menu.Selected_PharmacyRecord(pmc, record_no);
									}
									else
									{
										selecting = false;
									}
								} while (selecting);
								break;
							}
							case MODIFY_MEDICATION_RECORD:
							{
								do
								{
									string pharmacyRecord_no = pharmacist_menu.Get_List("SELECT PharmacyRecord_Id, P.Name, Total_Medication_Price AS Total_Price FROM pharmacy_record PHR JOIN patient_record PR ON PHR.Record_Id = PR.Record_Id JOIN patient P ON PR.Patient_Id = P.Patient_Id",
										"PharmacyRecord_Id", "Name", "Total_Price",
										"                                                  PHARMACY RECORD                                                ", "");

									if (pharmacyRecord_no.empty())
									{
										selecting = false;
									}
									else
									{
										string operations = pharmacist_menu.Get_List("SELECT MedicationRecord_Id, Medication_Name, PR.Payment_Status FROM Medication_Record MR JOIN Medication M ON MR.Medication_Id = M.Medication_Id JOIN Pharmacy_Record PR ON MR.PharmacyRecord_Id = PR.PharmacyRecord_Id WHERE MR.PharmacyRecord_Id = '" + pharmacyRecord_no + "'",
											"MedicationRecord_Id", "Medication_Name", "Payment_Status",
											"                                                 MEDICATION RECORD                                               ", "Modify medication record");

										string medicationRecord_no;
										int pos = operations.find_first_of("0123456789"); //Get the position of the first digit

										if (pos != string::npos)
										{
											medicationRecord_no = operations.substr(pos); //Get the medicationRecord_Id
											operations = operations.substr(0, pos); //Get the operations
										}

										if (operations == "Update medication record")
										{
											pharmacist_menu.Selected_UpdateMedicationRecord(pmc, medicationRecord_no);

										}
										else if (operations == "Delete medication record")
										{
											pmc.DeleteMedicationRecord(medicationRecord_no);
										}
									}
								} while (selecting);
								break;
							}
							case CREATE_RESTOCK_MEDICATION_STOCK:
							{
								do
								{
									string medication_no = pharmacist_menu.Get_List("SELECT * FROM medication", "Medication_Id", "Medication_Name", "Quantity",
										"                                         CREATE & RESTOCK MEDICATION STOCK                                       ", "Refill medication");

									if (medication_no.empty()) { selecting = false; }
									else { pharmacist_menu.Selected_CreateMedication(pmc); }
								} while (selecting);
								break;
							}
							case PHARMACIST_SIGN_OUT:
							{
								sign_out = true;
								break;
							}
						}

						break;
					}
				}

			} while (!sign_out);
			login = false; //Set login to default value
		}

	} while (!exit);

	return 0;
}