#ifndef PUMP_AC_PARSER
#define PUMP_AC_PARSER

#include "pump_ac/jsoncpp/json.h"
#include "pump_ac/pump_ac_document.h"
#include "pump_ac/pump_ac_node.h"

namespace Pump
{
namespace Ac
{

class CParserBase
{
public:
    CParserBase();
    ~CParserBase();
    virtual pump_int32_t parse(const char * szBuff, size_t iSize) = 0;
    virtual CNode * root() = 0;
};

class CJsonParser
    : public CParserBase
{
public:
    virtual pump_int32_t parse(const char * szBuff, size_t iSize);
    virtual CNode * root();
private:
    Json::Reader m_jsonReader;
    Json::Value m_jsonRoot;
};

}
}

#endif // PUMP_AC_PARSER
