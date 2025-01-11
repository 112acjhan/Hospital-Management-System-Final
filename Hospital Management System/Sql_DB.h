#include <mysql/jdbc.h>
#include <string>

#ifndef Sql_DB_H
#define Sql_DB_H


using namespace std;

class Sql_DB
{
	private:
		sql::Connection* MySqlConnection;
		
	public:
		Sql_DB();
		~Sql_DB();
		void PrepareStatement(string query);
		sql::PreparedStatement* statement;
		sql::ResultSet* result;
		void QueryStatement();
		void QueryResult();
};
#endif // !Sql_DB_H
