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
	cmd.reset( new TADOCommand(url->Owner) );
	cmd->Connection=url;
	cmd->ParamCheck=false;
	try
	{
		url->Connected=true;
	} catch (...)
	{}
}
cSQL::~cSQL()
{
}

bool cSQL::TestConnectoin() const
{
	#ifdef NODB
	return true;
	#endif
	std::unique_ptr<TADOQuery> test( new TADOQuery(url->Owner) );
	test->Connection=url;
	try
	{
		test->Active=false;
		test->SQL->Clear();
		test->SQL->Add("select 1");
		test->Active=true;
		return true;
	}
	catch (...)
	{
		url->Connected=false;
		return false;
	}
}
bool cSQL::CheckConnection()
{
	#ifdef NODB
	return true;
	#endif
	if(!TestConnectoin())
		return false;
	return true;
}
std::unique_ptr<TADOQuery> cSQL::SendSQL(const String &request)
{
	std::unique_ptr<TADOQuery> query;
	#ifdef NODB
	return query;
	#endif
	try
	{
		query.reset( new TADOQuery(url->Owner) );
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
		query.reset();
		return query;
	}
}

bool cSQL::SendCommand(const String &request)
{
	#ifdef NODB
	return true;
	#endif
	if (!CheckConnection())
	{
		return false;
	}
	try
	{
		cmd->CommandText=request;
		cmd->Execute();
		return true;
   	}
	catch (Exception &exception)
	{
		return false;
	}
}
