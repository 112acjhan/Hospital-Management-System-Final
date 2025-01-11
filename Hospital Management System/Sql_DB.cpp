#include "Sql_DB.h"
#include <mysql/jdbc.h>
#include <conio.h>

using namespace sql;

Sql_DB::Sql_DB()
{
	try
	{
		//sql::mysql::MySQL_Driver * driver = sql::mysql::get_mysql_driver_instance();
		mysql::MySQL_Driver* driver = mysql::get_mysql_driver_instance();

		MySqlConnection = driver->connect("localhost", "root", "9556"); //database address, username, password
		MySqlConnection->setSchema("hospital_management_system"); //Schema Name
		statement = nullptr;
		result = nullptr;
	}
	catch (sql::SQLException& e)
	{
		if (e.getErrorCode() == 0)
		{
			system("cls");
			cout << "Unable to connect to database";
			_getch();
			exit(0);
			return;
		}
		cout << "#ERR: SQLException in " << __FILE__
			 << "(" << __FUNCTION__ << ") on file " << __LINE__ << endl
			 << "# ERR: " << e.what()
			 << " (MySQL error code: " << e.getErrorCode()
			 << ", SQLState: " << e.getSQLState() << ")" << endl;
		_getch();

	}
}

Sql_DB::~Sql_DB()
{
	try
	{
		if (MySqlConnection)
		{
			MySqlConnection->close();
			delete MySqlConnection;
			MySqlConnection = nullptr;
		}

		if (statement)
		{
			statement->close();
			delete statement;
			statement = nullptr;
		}

		if (result)
		{
			result->close();
			delete result;
			result = nullptr;
		}
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
}

void Sql_DB::PrepareStatement(string query)
{
	statement = MySqlConnection->prepareStatement(query);
	
}

void Sql_DB::QueryStatement()
{
	try
	{
		statement->executeUpdate();
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
}

void Sql_DB::QueryResult()
{
	try
	{
		result = statement->executeQuery();
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
}