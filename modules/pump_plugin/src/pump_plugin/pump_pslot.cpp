#include "pump_macro/pump_pre.h"
#include <string>
#include <iostream>
#if (PUMP_PLUGIN_LUA_MODE == 1)
#include "SLB3/namespace.h"
#include "SLB3/internal/class.h"
#include "SLB3/implementation.h"
#else
#include "pump_core/pump_core_dllso.h"
#endif // PUMP_PLUGIN_LUA_MODE
#include "pump_core/pump_core_types.h"
#include "pump_core/file/pump_core_file.h"
#include "pump_core/pump_core_api.h"
#include "pump_plugin/pump_pslot.h"
#include "pump_plugin/__pump_plugin_inner_interface.h"
#include "pump_plugin/pump_plugin_lua_context.h"

PUMP_PLUGIN_API int PUMP_PSLOT_Init()
{
#if (PUMP_PLUGIN_LUA_MODE == 1)
    SLB3::Register<::Pump::Core::Thread::CThread>(::Pump::Slot::GetGlobalLuaContext().L());
    SLB3::Register<::Pump::Core::Thread::CMutex>(::Pump::Slot::GetGlobalLuaContext().L());
    SLB3::Register<::Pump::Slot::CLuaThreadData>(::Pump::Slot::GetGlobalLuaContext().L());
    SLB3::Register<::Pump::Slot::CLuaThread>(::Pump::Slot::GetGlobalLuaContext().L());
#endif // PUMP_PLUGIN_LUA_MODE
    return PUMP_OK;
}

PUMP_PLUGIN_API int PUMP_PSLOT_Cleanup()
{
    return PUMP_OK;
}

PUMP_PLUGIN_API  int PUMP_PSLOT_Config(const PUMP_PSLOT_CONF * pConf, pump_uint32_t nSize)
{
    return PUMP_OK;
}

PUMP_PLUGIN_API int PUMP_PSLOT_PluginInstall(const char* szPkgPath)
{
    return PUMP_OK;
}

PUMP_PLUGIN_API int PUMP_PSLOT_PluginUninstall(const char* szName, const char* szVersion)
{
    return PUMP_OK;
}

PUMP_PLUGIN_API int PUMP_PSLOT_GetInstalledPluginsInfo(char * pBuf, pump_uint32_t nSize)
{
    return PUMP_OK;
}

PUMP_PLUGIN_API pump_plugin_id PUMP_PSLOT_PluginLoad(const char* szName, const char* szVersion)
{
    std::string strPluginDir = "E:/VMware/YZ/Pump/modules/pump_plugin/test/";
    strPluginDir += szName;
    strPluginDir += "@";
    strPluginDir += szVersion;
    // 1.���������װ·��
    pump_handle_t hDir = PUMP_CORE_DirOpen(strPluginDir.c_str());
    if (hDir == PUMP_INVALID_HANDLE)
    {
        printf("plugin not installed\n");
        return -1;
    }
    PUMP_CORE_DirClose(hDir);
    // 2.����������
#if (PUMP_PLUGIN_LUA_MODE == 1) // ʹ��luaģʽ
    std::string strEntry = strPluginDir + "/script/entry.lua";
#else
    std::string strEntry = strPluginDir + "/bin/entry.dll";
#endif // (PUMP_PLUGIN_LUA_MODE == 1)(PUMP_PLUGIN_LUA_MODE == 1)
    if (!PUMP_CORE_FileIsExist(strEntry.c_str()))
    {
        printf("Invalid plug-in\n");
        return -1;
    }
#if (PUMP_PLUGIN_LUA_MODE == 1) // ʹ��luaģʽ
    ::Pump::Slot::CLSLoadEntry loadTestPlugin(szName, szVersion);
    ::Pump::Slot::GetGlobalLuaContext().RunLuaScriptObject(loadTestPlugin);
    std::cout << loadTestPlugin.GetOut() << std::endl;
    try 
    {
        printf("%s\n", ::Pump::Slot::GetGlobalLuaContext().RunLuaScriptString("SLB." + std::string(szName) + ".Check();").c_str());
    } catch (std::exception e) 
    {
        printf("%s\n", e.what());
    }

    printf("%s\n", ::Pump::Slot::GetGlobalLuaContext().RunLuaScriptString("SLB." + std::string(szName) + ".Init();").c_str());
    printf("%s\n", ::Pump::Slot::GetGlobalLuaContext().RunLuaScriptString("SLB." + std::string(szName) + ".Load();").c_str());
    //printf("%s\n", ::Pump::Slot::GetGlobalLuaContext().RunLuaFunction("SLB." + std::string(szName) /*+ ".Post"*/, 4).c_str());
    //PUMP_CORE_Sleep(3000);
    printf("%s\n", ::Pump::Slot::GetGlobalLuaContext().RunLuaScriptString("SLB." + std::string(szName) + ":Post(5);").c_str());
    const char* ss = lua_tostring(::Pump::Slot::GetGlobalLuaContext().L(), 1);
    //while (1)
    //{
    //    PUMP_CORE_Sleep(300);
    //    printf("%s\n", ::Pump::Slot::GetGlobalLuaContext().RunLuaScriptString("SLB." + std::string(szName) + ":Post(6);").c_str());
    //    printf("%s\n", ::Pump::Slot::GetGlobalLuaContext().RunLuaScriptString("SLB." + std::string(szName) + ":Post(4);").c_str());
    //}
#else // cģʽ
    // 1. load plugin entry.dll
    pump_module_t hModule = PUMP_CORE_LoadDSo(strEntry.c_str());
    if (hModule==PUMP_INVALID_MODULE)
    {
        printf("Load plug-in entry.dll failed\n");
        return -1;
    }
    // 2. get and call plugin hello api
    PUMP_PLUGIN_CB_Hello pfnPUMP_PLUGIN_Hello = (PUMP_PLUGIN_CB_Hello)PUMP_CORE_GetDsoSym(hModule, "PUMP_PLUGIN_Hello");
    if (pfnPUMP_PLUGIN_Hello==NULL)
    {
        printf("Get plug-in PUMP_PLUGIN_Hello() failed\n");
        return -1;
    }
    // 3.call hello get plugin entry APIs.
    PUMP_PLUGIN_ENTRY_API struPCb;
    memset(&struPCb, 0, sizeof(struPCb));
    pfnPUMP_PLUGIN_Hello(&struPCb);
#endif // PUMP_PLUGIN_LUA_MODE
    return 0;
}

PUMP_PLUGIN_API int PUMP_PSLOT_PluginUnload(pump_plugin_id iPID)
{
    return PUMP_OK;
}

PUMP_PLUGIN_API int PUMP_PSLOT_GetPluginInfo(pump_plugin_id iPID, char* pBuf, pump_uint32_t nSize)
{
    return PUMP_OK;
}

PUMP_PLUGIN_API int PUMP_PSLOT_PluginConfig(pump_plugin_id iPID, const char * szInfo)
{
    return PUMP_OK;
}

PUMP_PLUGIN_API void PUMP_PSLOT_SetPluginResponceCallback(int iPID, PUMP_PSLOT_CB_PluginResponce cb)
{
}

PUMP_PLUGIN_API int PUMP_PSLOT_PluginRequest(int iPID, const PUMP_PLUGIN_REQUEST * pRequest)
{
    return PUMP_OK;
}

namespace Pump
{
namespace Slot
{

class CPluginClient;

PUMP_PLUGIN_CXXAPI CPluginClient  * PUMP_PSLOT_INNER_GetPluginClientByIDLocked(pump_plugin_id pid)
{
    return NULL;
}


PUMP_PLUGIN_CXXAPI void PUMP_PSLOT_INNER_PluginClientByIDUnLock(pump_plugin_id pid)
{
}

}
}