#ifndef PUMP_PSLOT_H
#define PUMP_PSLOT_H

#include "pump_macro/pump_pre.h"
#include "pump_core/pump_core_types.h"
#include "pump_plugin/pump_plugin_config.h"
#include "pump_plugin/pump_plugin.h"

enum PUMP_PSLOT_LOG_LEVEL 
{
    
};

typedef pump_int32_t pump_plugin_id;

/**
* ��־�ص������û�ʵ�֣�������SDK
* @param emLogLevel ��־����
* @param szFileName Դ�ļ���
* @param nLineNo �к�
* @param szFuncName ������
* @param szModular ģ����
* @param szMessage ��־��������
* @param nMsgLen ��־���ݳ���
*/
typedef void(*PUMP_PSLOT_CB_WriteLog) (
    PUMP_PSLOT_LOG_LEVEL emLogLevel,
    const char *szFileName,
    pump_uint32_t nLineNo,
    const char *szModularName,
    const char *szMessage,
    pump_uint32_t nMsgLen);

/**�����첽�ص�ԭ��*/
typedef void(*PUMP_PSLOT_CB_PluginResponce)(pump_plugin_id, const PUMP_PLUGIN_RESPONSE*);


typedef struct tagPUMP_PSLOT_CONF
{
    PUMP_PSLOT_CB_WriteLog cbLog;
    const char* szInstallPath;
} PUMP_PSLOT_CONF, *LPPUMP_PSLOT_CONF;

/**��ʼ���ӿ�*/
PUMP_PLUGIN_API int PUMP_PSLOT_Init();

/**����ʼ���ӿ�*/
PUMP_PLUGIN_API int PUMP_PSLOT_Cleanup();

/**
* ȫ�����ýӿڣ�ʵ����־���������·��������
* @param pConf slot���ýṹ��
*/
PUMP_PLUGIN_API int PUMP_PSLOT_Config(const PUMP_PSLOT_CONF * pConf, pump_uint32_t nSize);

/**
* �����װ�ӿڣ�չ�������װ��������ָ��·��
* @param szPkgPath ��װ��·��
*/
PUMP_PLUGIN_API int PUMP_PSLOT_PluginInstall(const char* szPkgPath);

/**ɾ�����*/
PUMP_PLUGIN_API int PUMP_PSLOT_PluginUninstall(const char* szName, const char* szVersion = NULL);

/**
* ��ȡ�Ѱ�װ�����Ϣ��������װ�����·��������json��ʽ�İ�װ�����Ϣ
* @param pBuf ������Ϣ�Ļ��������ɵ������ṩ����Ҫ�Ǳ��������̬����
* @param nSize ����������
* @return 0(����ִ�гɹ�)��-1(����ʧ�ܣ����ݴ����뻺�������������µ���)
*/
PUMP_PLUGIN_API int PUMP_PSLOT_GetInstalledPluginsInfo(char * pBuf, pump_uint32_t nSize);

/**
* ���ز���ӿ�
* @param szName �����
* @param szVersion ����汾
* @return -1(ʧ��)��>=0(�����ԴID)������ж�ز��
*/
PUMP_PLUGIN_API pump_plugin_id PUMP_PSLOT_PluginLoad(const char* szName, const char* szVersion = NULL);

/**
* ж�ز���ӿ�
* @param iPID �����ԴID
*/
PUMP_PLUGIN_API int PUMP_PSLOT_PluginUnload(pump_plugin_id iPID);

/**
* ��ȡ�Ѽ��ز����Ϣ
* @param iPID �����Դ��
* @param szBuf ������Ϣ�Ļ��������ɵ������ṩ����Ҫ�Ǳ��������̬����
* @param nSize ����������
* @return 0(����ִ�гɹ�)��-1(����ʧ�ܣ����ݴ����뻺�������������µ���)
*/
PUMP_PLUGIN_API int PUMP_PSLOT_GetPluginInfo(pump_plugin_id iPID, char* pBuf, pump_uint32_t nSize);

/**�����Ѽ��ز��*/
PUMP_PLUGIN_API int PUMP_PSLOT_PluginConfig(pump_plugin_id iPID, const char * szInfo);

/**�첽�ص�����*/
PUMP_PLUGIN_API void PUMP_PSLOT_SetPluginResponceCallback(pump_plugin_id iPID, PUMP_PSLOT_CB_PluginResponce cb);

/**
* �����·������첽��
* @param pRequest ������
* @param nSize ���ݳ���
* @param cb ��Ӧ�ص�
*/
PUMP_PLUGIN_API int PUMP_PSLOT_PluginRequest(pump_plugin_id iPID, const PUMP_PLUGIN_REQUEST * pRequest);

#endif // PUMP_PSLOT_H