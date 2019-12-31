#include "gtest/gtest.h"
#include <qsp/qsp.h>
#include <qsp/default/qsp_default.h>

bool QSPNoLastError()
{
    QSP_CHAR* loc;
    int code, actIndex, line;
    QSPGetLastErrorData(&code, &loc, &actIndex, &line);
    return code == 0;
}

int QSPGetLastErrorCode()
{
    QSP_CHAR* loc;
    int code, actIndex, line;
    QSPGetLastErrorData(&code, &loc, &actIndex, &line);
    return code;
}

//*/
//QSP_EXTERN void QSPEnableDebugMode(QSP_BOOL isDebug);
//QSP_EXTERN void QSPGetCurStateData(QSP_CHAR** loc, int* actIndex, int* line);
//QSP_EXTERN QSP_BOOL QSPGetExprValue(const QSP_CHAR* str, QSP_BOOL* isString,
//    int* numVal, QSP_CHAR* strVal,
//    int strValBufSize);
//QSP_EXTERN void QSPSetInputStrText(const QSP_CHAR* str);
//QSP_EXTERN QSP_BOOL QSPExecuteSelActionCode(QSP_BOOL isRefresh);
//QSP_EXTERN QSP_BOOL QSPSetSelActionIndex(int ind, QSP_BOOL isRefresh);


//QSP_EXTERN QSP_BOOL QSPSetSelObjectIndex(int ind, QSP_BOOL isRefresh);

//QSP_EXTERN void QSPShowWindow(int type, QSP_BOOL isShow);
//QSP_EXTERN QSP_BOOL QSPGetVarValuesCount(const QSP_CHAR* name, int* count);
//// name can be: "DISABLESCROLL" "NOSAVE" "USEHTML" "BACKIMAGE" "BCOLOR" "FCOLOR"
//// "LCOLOR" "FSIZE" "FNAME" "STYLESHEET"
//QSP_EXTERN QSP_BOOL QSPGetVarValues(const QSP_CHAR* name, int ind, int* numVal,
//    QSP_CHAR** strVal);
//QSP_EXTERN QSP_BOOL QSPGetVarNameByIndex(int ind, QSP_CHAR** name);
//// if link has EXEC: section
//QSP_EXTERN QSP_BOOL QSPExecString(const QSP_CHAR* str, QSP_BOOL isRefresh);
//// update by timer
//QSP_EXTERN QSP_BOOL QSPExecCounter(QSP_BOOL isRefresh);
//// after entering text
//QSP_EXTERN QSP_BOOL QSPExecUserInput(QSP_BOOL isRefresh);
////??? never used
//QSP_EXTERN QSP_BOOL
//QSPExecLocationCode(const QSP_CHAR* name, QSP_BOOL isRefresh);
//// for error message output
//QSP_EXTERN void QSPGetLastErrorData(int* errorNum, QSP_CHAR** errorLoc,
//    int* errorActIndex, int* errorLine);
//QSP_EXTERN const 
//QSP_EXTERN QSP_BOOL QSPLoadJsonData();
////?? move to const?
////QSP_EXTERN QSP_BOOL QSPLoadGameWorld(const QSP_CHAR *file, const QString *gameDirectory);
//QSP_EXTERN QSP_BOOL QSPLoadGameWorld(const QSP_CHAR* file);
//QSP_EXTERN QSP_BOOL QSPLoadGameWorldFromData(const void* data, int dataSize,
//    const QSP_CHAR* file);
//QSP_EXTERN QSP_BOOL QSPSaveGame(const QSP_CHAR* file, QSP_BOOL isRefresh);
//QSP_EXTERN QSP_BOOL QSPSaveGameAsData(void* buf, int bufSize, int* realSize,
//    QSP_BOOL isRefresh);
//QSP_EXTERN QSP_BOOL QSPOpenSavedGame(const QSP_CHAR* file, QSP_BOOL isRefresh, bool ignoreCRC);
//QSP_EXTERN QSP_BOOL QSPOpenSavedGameFromData(const void* data, int dataSize,
//    QSP_BOOL isRefresh);
//QSP_EXTERN QSP_BOOL QSPRestartGame(QSP_BOOL isRefresh);
//QSP_EXTERN void QSPSetCallBack(int type, QSP_CALLBACK func);

TEST(Game, initTest)
{
    QSPInit();
    ASSERT_TRUE(QSPNoLastError());

    ASSERT_STRNE(L"", QSPGetVersion());
    ASSERT_STRNE(L"", QSPGetCompiledDateTime());

    ASSERT_EQ(0, QSPGetFullRefreshCount());

    // Command & Control
    ASSERT_FALSE(QSPIsInCallBack());

    // File information
    ASSERT_EQ(nullptr, QSPGetQstFullPath());

    // Locations
    ASSERT_EQ(nullptr, QSPGetCurLoc());
    QSP_CHAR* loc;
    int actIndex, line;
    QSPGetCurStateData(&loc, &actIndex, &line);
    ASSERT_EQ(nullptr, loc);
    ASSERT_EQ(-1, actIndex);
    ASSERT_EQ(0, line);

    // Main Description
    ASSERT_EQ(nullptr, QSPGetMainDesc());
    ASSERT_FALSE(QSPIsMainDescChanged());

    // Vars Description
    ASSERT_EQ(nullptr, QSPGetVarsDesc());
    ASSERT_FALSE(QSPIsVarsDescChanged());

    // Variables
    ASSERT_LT(12799, QSPGetMaxVarsCount());
    QSP_CHAR* name;
    for (int i = 0; i < QSPGetMaxVarsCount(); ++i) {
        EXPECT_FALSE(QSPGetVarNameByIndex(i, &name));
    }

    // Actions
    ASSERT_EQ(0, QSPGetActionsCount());
    ASSERT_FALSE(QSPIsActionsChanged());
    ASSERT_EQ(-1, QSPGetSelActionIndex());
    ASSERT_TRUE(QSPExecuteSelActionCode(true));

    // Objects
    ASSERT_EQ(0, QSPGetObjectsCount());
    ASSERT_FALSE(QSPIsObjectsChanged());
    ASSERT_EQ(-1, QSPGetSelObjectIndex());


    QSPDeInit();
}

TEST(Game, cantStartWithoutLocation)
{
    QSPInit();
    ASSERT_TRUE(QSPNoLastError());

    auto result = QSPRestartGame(true);
    ASSERT_FALSE(result);
    
    auto errorCode = QSPGetLastErrorCode();
    ASSERT_EQ(QSP_ERR_GAMENOTLOADED, errorCode);
    ASSERT_STRNE(L"", QSPGetErrorDesc(errorCode));
}

