#ifndef SQLH
#define SQLH
#include <ADODB.hpp>
#include <memory>

//#define NODB;
class cSQL
{
public:
	cSQL(TADOConnection* Connection);
	~cSQL();
	std::unique_ptr<TADOQuery> SendSQL(const String &request);
	bool SendCommand(const String &request);
private:
	bool CheckConnection();
	bool TestConnectoin() const;
	TADOConnection* url;
	std::unique_ptr<TADOCommand> cmd;
	int ReconnectCount;
};
#endif
