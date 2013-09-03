#ifndef SQLH
#define SQLH
#include <ADODB.hpp>
#include <vector.h>
//#define NODB;
class cSQL
	{
	public:
			cSQL(TADOConnection * Connection);
			cSQL(cSQL * r);
			~cSQL();
		void SetConnection (TADOConnection * Connection){url=Connection;};
		void SetReconnectCount(int count){ReconnectCount=count;};
		int GetReconnectCount(){return ReconnectCount;};
		TADOConnection* GetConnect()const {return url;};
		bool CheckConnection();
		bool Connect ();
		TADOQuery * SendSQL(const String &request);
		bool SendCommand(const String &request);
		void AddTable(TADOTable *table);
		void DelTable(TADOTable *table);
		vector <String> Get_log(void){return log;};
	private:
		bool TestConnectoin() const;
		TADOConnection *url;
		TADOCommand *cmd;
		vector <TADOTable *> tables;
		int ReconnectCount;
		static vector <String> log;
	};
vector <String> cSQL::log;
#endif
