#ifndef SQLH
#define SQLH
#include <ADODB.hpp>
#include <memory>
#include <vector>

//#define NODB;
class cSQL
{
public:
   cSQL(TADOConnection* Connection, bool enable_log = true);
   ~cSQL();
   std::unique_ptr<TADOQuery> SendSQL(const String &request);
   bool SendCommand(const String &request);
   std::vector <String>& Get_log(void);
   void clearLog(void);
private:
   bool CheckConnection();
   bool TestConnectoin() const;
   TADOConnection* url;
   std::unique_ptr<TADOCommand> cmd;
   bool mEnableLog;
   static std::vector <String> log;
};
#endif
