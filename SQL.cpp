//---------------------------------------------------------------------------
#pragma hdrstop
#include "SQL.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
cSQL::cSQL(TADOConnection * Connection):url(Connection),ReconnectCount(5),cmd(0)
{
#ifdef NODB
return;
#endif
//if (cmd) {delete cmd; cmd=0;}
cmd=new TADOCommand(url->Owner);
cmd->Connection=url;
cmd->ParamCheck=false;
try{url->Connected=true;} catch (...){}
tables.clear();
}
cSQL::cSQL(cSQL * r):url(r->GetConnect()),ReconnectCount(r->GetReconnectCount())
{
#ifdef NODB
return;
#endif
if (cmd) {delete cmd; cmd=0;}
cmd=new TADOCommand(url->Owner);
cmd->Connection=url;
cmd->ParamCheck=false;
tables.clear();
}
cSQL::~cSQL()
{
delete cmd;
}
void cSQL::AddTable(TADOTable *table)
{
#ifdef NODB
return;
#endif
tables.push_back(table);     
}
void cSQL::DelTable(TADOTable *table)
{
#ifdef NODB
return;
#endif
for (int i = 0; i <tables.size(); i++) 
	{
	if (tables[i]==table) 
		{
		tables.erase(tables.begin()+i);
		}
	}
}
bool cSQL::Connect()
{
#ifdef NODB
return true;
#endif
if (cmd) {delete cmd; cmd=0;}
if (!url->Connected)
	{
	TComponent *tmp=url->Owner;
	delete url;
	url=new TADOConnection(tmp);
	url->LoginPrompt=false;
	}
cmd=new TADOCommand(url->Owner);
cmd->Connection=url;
cmd->ParamCheck=false;
try{url->Connected=true;} catch (...){}
try
{
for (int i = 0; i <tables.size(); i++) 
	{
	tables[i]->Active=false;
	tables[i]->Connection=url;
	tables[i]->Active=true;
	}
}
catch(...){}
return TestConnectoin();
}
bool cSQL::TestConnectoin() const
{
#ifdef NODB
return true;
#endif
TADOQuery *test;
test= new TADOQuery(url->Owner);
test->Connection=url;
try
	{
	test->Active=false;
	test->SQL->Clear();
	test->SQL->Add("select 1");
	test->Active=true;
	delete test;
	return true;
	}
catch (...) {delete test;url->Connected=false;return false;}
}
bool cSQL::CheckConnection()
{
#ifdef NODB
return true;
#endif
if(!TestConnectoin())
	{
	for (int i=0; i<=ReconnectCount; i++)
		{
		if (Connect()){return true;}
		}
	return false;
	}
return true;
}
TADOQuery * cSQL::SendSQL(const String &request)
{
TADOQuery *query=0;
#ifdef NODB
return 0;
#endif
//if (!CheckConnection()) {delete query;return 0;}   //fox pro не дружит с select 1
try
	{
	log.push_back(Time().TimeString()+"---"+request);
	query=new TADOQuery(url->Owner);
	query->ParamCheck=false;
	query->Connection=url;
	query->Active=false;
	query->SQL->Clear();
	query->SQL->Add(request);
	query->Active=true;
	return query;
	}
catch (Exception &exception)
{
	log.push_back("ERROR:"+Time().TimeString()+"---"+exception.ToString() );
	delete query;return 0;
}
}
bool cSQL::SendCommand(const String &request)
{
#ifdef NODB
return true;
#endif
if (!CheckConnection())
{return false;}
try
   {
	log.push_back(Time().TimeString()+"---"+request);
	cmd->CommandText=request;
	cmd->Execute();
	return true;
   }
catch (Exception &exception)
{
	log.push_back("ERROR:"+Time().TimeString()+"---"+exception.ToString() );
	return false;
}
}
/*дописать куски для работы с таблицами*/
