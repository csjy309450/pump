#ifndef PUMP_TEST_H
#define PUMP_TEST_H
#include <cstdio>
#include <cstring>
#include <cassert>

#ifdef WIN32
#ifndef PUMP_CALLBACK
#define PUMP_CALLBACK __cdecl
#endif PUMP_CALLBACK
#else
#ifndef PUMP_CALLBACK
#define PUMP_CALLBACK
#endif // PUMP_CALLBACK
#endif // WIN32

/** Global user test prefix, user configured*/
#ifndef PUMP_CONF_TEST_PRE
#define TEST_UNIQUE_PRE_NAME YZ19900322_
#else
#define TEST_UNIQUE_PRE_NAME PUMP_CONF_TEST_PRE
#endif // PUMP_CONF_TEST_PRE

// pump_test version info
#ifndef PUMP_TEST_VERSION_MAJOR
#   define PUMP_TEST_VERSION_MAJOR 0
#endif // !PUMP_TEST_VERSION_MAJOR
#ifndef PUMP_TEST_VERSION_MINOR
#   define PUMP_TEST_VERSION_MINOR 0
#endif // !PUMP_TEST_VERSION_MINOR
#ifndef PUMP_TEST_VERSION_PATCH
#   define PUMP_TEST_VERSION_PATCH 9
#endif // !PUMP_TEST_VERSION_PATCH
#ifndef PUMP_TEST_VERSION_BUILD
#   define PUMP_TEST_VERSION_BUILD 0
#endif // !PUMP_TEST_VERSION_BUILD
#define __PUMP_TEST_VERSION_STR(str_) #str_
#define __PUMP_TEST_VERJOIN(major_,minor_,patch_,build_) __PUMP_TEST_VERSION_STR(major_.minor_.patch_.build_)
#define PUMP_TEST_VERSION __PUMP_TEST_VERJOIN(PUMP_TEST_VERSION_MAJOR,PUMP_TEST_VERSION_MINOR,PUMP_TEST_VERSION_PATCH,PUMP_TEST_VERSION_BUILD)

#define PTEST_Printf(_format, ...) CGlobalTestSceneMgr::GetGlobalTestMgr()->OutPut(_format, __VA_ARGS__)

#define PTEST_G_FN_DEF(indx_,...) INTER_PTEST_G_FN_DEF(indx_, __VA_ARGS__)
#define PTEST_G_FN_CALL(indx_,...) INTER_PTEST_G_FN_CALL(indx_, __VA_ARGS__)
#define PTEST_ASSERT(_exp,_format,...) INTER_PTEST_ASSERT(TEST_UNIQUE_PRE_NAME, _exp, _format,__VA_ARGS__)
#define PTEST_LOG(_key,_format,...) PTEST_Printf("  \""#_key"\": "#_format",\n", __VA_ARGS__)
#define PTEST_G_CASE_DEF(indx_) INTER_PTEST_G_FN_DEF(indx_,void)
#define PTEST_G_CASE_RUN_WITH_FN(indx_, _call, _msg) \
do \
{ \
    PTEST_Printf("{\n  \"test_case\": \""#indx_"\",\n"); \
    if (_call==-1) \
    { \
        PTEST_Printf("  \"result\": \"failed\",\n"); \
    }\
    else \
    { \
        PTEST_Printf("  \"result\": \"succed\",\n"); \
    } \
    PTEST_Printf("  \"msg\": \"%s\"\n}\n", _msg); \
} while(0);
#define TEST_G_CASE_RUN(indx_, _msg) \
do \
{ \
    PTEST_Printf("{\n  \"test_case\": \""#indx_"\",\n"); \
    if (INTER_MAKE_TEST_FN_NAME(indx_, TEST_UNIQUE_PRE_NAME)()==-1) \
    { \
        PTEST_Printf("  \"result\": \"failed\",\n"); \
    } \
    else \
    { \
        PTEST_Printf("  \"result\": \"succed\",\n"); \
    } \
    PTEST_Printf("  \"msg\": \"%s\"\n}\n", _msg); \
} while(0);
#define INTER_MAKE_STR(X) INTER_MAKE_STR_(X)
#define INTER_MAKE_STR_(X) #X
#define INTER_MAKE_TEST_FN_NAME(pre_, indx_) INTER_MAKE_TEST_FN_NAME_(pre_, indx_)
#define INTER_MAKE_TEST_FN_NAME_(pre_, indx_)  pre_##indx_
#define INTER_PTEST_G_FN_DEF(indx_,_args) INTER_PTEST_G_FN_DEF_(TEST_UNIQUE_PRE_NAME,indx_, _args)
#define INTER_PTEST_G_FN_DEF_(pre_, indx_, _args) int INTER_MAKE_TEST_FN_NAME_(pre_, indx_) (_args)
#define INTER_PTEST_G_FN_CALL(indx_, _args) INTER_PTEST_G_FN_CALL_(TEST_UNIQUE_PRE_NAME,indx_, _args)
#define INTER_PTEST_G_FN_CALL_(pre_, indx_, _args) INTER_MAKE_TEST_FN_NAME_(pre_, indx_)(_args)
#define INTER_PTEST_ASSERT(pre_, _exp, _format,...) INTER_PTEST_ASSERT_(pre_, _exp, _format,__VA_ARGS__)
#define INTER_PTEST_ASSERT_(pre_, _exp, _format,...) \
do \
{ \
    if(strstr(__FUNCTION__, #pre_)==NULL) \
    { \
        PTEST_Printf("PTEST_ASSERT() must be called in PTEST_G_FN_DEF(...) \n"#pre_); \
        assert(1==0); \
    } \
    if (!_exp) \
    { \
        PTEST_Printf("  \"assert\": {\n    \"pos\": \"%s:%u\",\n    \"msg\": \""#_format"\"\n  }\n", __FILE__,__LINE__,__VA_ARGS__); \
        return -1; \
    }\
} while(0);

#include <list>

namespace Pump
{
namespace Test
{

class CTestCaseBase;
class CTestSceneBase;
class CGlobalTestSceneMgr;

class CTestApplication
{
public:
    CTestApplication();
    ~CTestApplication();
};

class CGlobalTestSceneMgr
{
public:
    CGlobalTestSceneMgr();
    ~CGlobalTestSceneMgr();
    int Run();
    void RegisterTestScene(CTestSceneBase* pScene);
    void OutPut(const char* pFormat, ...);

    static CGlobalTestSceneMgr * GetGlobalTestMgr();

private:
    int (PUMP_CALLBACK *OutPutPUMP_CALLBACK)(const char* pFormat, va_list argv);
private:
    std::list<CTestSceneBase*> m_listTestScene;
    static CGlobalTestSceneMgr * s_pThis;
};

class CTestCaseBase
{
protected:
    explicit CTestCaseBase(const char* pTestCaseId);
    ~CTestCaseBase();
public:
    virtual int Init();
    virtual int Cleanup();
    int Run(CTestSceneBase * pScene);
    virtual int operator()(CTestSceneBase * pScene) = 0;
private:
    CTestCaseBase();
protected:
    const char* m_pTestCaseId;
    CTestSceneBase * m_pScene;
};

class CTestSceneBase
{
protected:
    explicit CTestSceneBase(const char* TestSceneId);
public:
    virtual int Init();
    virtual int Cleanup();
    int Run();
protected:
    void _RegisterTestCase(CTestCaseBase* pCase);
public:
    static CTestSceneBase * s_pThis;
private:
    CTestSceneBase();
protected:
    const char* m_pTestSceneId;
    std::list<CTestCaseBase*> m_listTestCase;
};

}
}

#define INTER_MAKE_TEST_CLASS_NAME(pre_,indx_) INTER_MAKE_TEST_C_NAME(INTER_MAKE_TEST_C_NAME(pre_,TEST_UNIQUE_PRE_NAME),indx_)
#define INTER_MAKE_TEST_C_NAME(pre_, indx_) INTER_MAKE_TEST_C_NAME_(pre_, indx_)
#define INTER_MAKE_TEST_C_NAME_(pre_, indx_) pre_##indx_
#define PTEST_SCENE_NAME(indx_) INTER_MAKE_TEST_CLASS_NAME(CTestScene_,indx_)
#define PTEST_C_SCENE_DEF(indx_, code_) \
class INTER_MAKE_TEST_CLASS_NAME(CTestScene_,indx_) \
    : public CTestSceneBase \
{ \
private: \
    INTER_MAKE_TEST_CLASS_NAME(CTestScene_,indx_)()  \
        : CTestSceneBase(INTER_MAKE_STR(INTER_MAKE_TEST_CLASS_NAME(CTestScene_,indx_))) {} \
    ~INTER_MAKE_TEST_CLASS_NAME(CTestScene_,indx_)() {} \
public: \
    static INTER_MAKE_TEST_CLASS_NAME(CTestScene_,indx_) * GetTestScene() \
    { \
        if (!s_pScene) \
        { \
            s_pScene = new INTER_MAKE_TEST_CLASS_NAME(CTestScene_,indx_)(); \
        } \
        return s_pScene; \
    } \
    static void RegisterTestCase(CTestCaseBase * pCase) \
    { \
        if (pCase) \
        { \
            GetTestScene()->_RegisterTestCase(pCase); \
        } \
    } \
code_ \
private: \
    static INTER_MAKE_TEST_CLASS_NAME(CTestScene_,indx_) * s_pScene; \
};\
INTER_MAKE_TEST_CLASS_NAME(CTestScene_,indx_) * INTER_MAKE_TEST_CLASS_NAME(CTestScene_,indx_)::s_pScene = NULL;
#define PTEST_CASE_NAME(indx_) INTER_MAKE_TEST_CLASS_NAME(CTestCase_,indx_)
#define PTEST_C_CASE_DEF(indx_,scene_, code_) \
class INTER_MAKE_TEST_CLASS_NAME(CTestCase_,indx_)  \
    : public CTestCaseBase \
{ \
public: \
    INTER_MAKE_TEST_CLASS_NAME(CTestCase_, indx_)() \
        : CTestCaseBase(INTER_MAKE_STR(INTER_MAKE_TEST_CLASS_NAME(CTestCase_,indx_))) \
    { \
        INTER_MAKE_TEST_CLASS_NAME(CTestScene_, scene_)::RegisterTestCase(this); \
    } \
    ~INTER_MAKE_TEST_CLASS_NAME(CTestCase_, indx_)() {} \
    virtual int operator()(CTestSceneBase * pScene); \
code_ \
private: \
    static  INTER_MAKE_TEST_CLASS_NAME(CTestCase_, indx_) * s_pCase; \
}; \
INTER_MAKE_TEST_CLASS_NAME(CTestCase_, indx_) * \
    INTER_MAKE_TEST_CLASS_NAME(CTestCase_, indx_)::s_pCase \
        = new INTER_MAKE_TEST_CLASS_NAME(CTestCase_, indx_)(); \
int INTER_MAKE_TEST_CLASS_NAME(CTestCase_, indx_)::operator()(CTestSceneBase * pScene)

#define PTEST_SCENE_PTR(scene_) INTER_MAKE_TEST_CLASS_NAME(CTestScene_,scene_)::GetTestScene()

#define PTEST_MAIN2(_argc, _argv, code_) \
    using namespace Pump::Test; \
    int main(_argc, _argv) \
    { \
        CTestApplication objTest; \
        code_ \
    }
#define PTEST_MAIN_BEGINE(_argc, _argv) \
using namespace Pump::Test; \
int main(_argc, _argv) \
{ \
    CTestApplication objTest; 
#define PTEST_MAIN_END }

#endif // PUMP_TEST_H
