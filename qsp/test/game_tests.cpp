#include <gtest/gtest.h>
#include <gmock/gmock.h>
//include "gtest/gtest.h"
//#include "gmock/gmock.h"
#include <qsp/qsp.h>
#include <qsp/default/qsp_default.h>

//#ifdef __GNUC__
//#include <unistd.h>
//#elif _MSC_VER
//#include <windows.h>
//#endif
using ::testing::AtLeast;
using ::testing::_;
using ::testing::Mock;
using ::testing::Exactly;

#include <string>
#include <string.h>

using namespace std;

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

    EXPECT_STRNE(L"", QSPGetVersion());
    EXPECT_STRNE(L"", QSPGetCompiledDateTime());

    EXPECT_EQ(0, QSPGetFullRefreshCount());

    // Command & Control
    EXPECT_FALSE(QSPIsInCallBack());

    // File information
    EXPECT_EQ(nullptr, QSPGetQstFullPath());

    // Locations
    EXPECT_EQ(nullptr, QSPGetCurLoc());
    QSP_CHAR* loc;
    int actIndex, line;
    QSPGetCurStateData(&loc, &actIndex, &line);
    EXPECT_EQ(nullptr, loc);
    EXPECT_EQ(-1, actIndex);
    EXPECT_EQ(0, line);

    // Main Description
    EXPECT_EQ(nullptr, QSPGetMainDesc());
    EXPECT_FALSE(QSPIsMainDescChanged());

    // Vars Description
    EXPECT_EQ(nullptr, QSPGetVarsDesc());
    EXPECT_FALSE(QSPIsVarsDescChanged());

    // Variables
    EXPECT_LT(12799, QSPGetMaxVarsCount());
    QSP_CHAR* name;
    for (int i = 0; i < QSPGetMaxVarsCount(); ++i) {
        EXPECT_FALSE(QSPGetVarNameByIndex(i, &name));
    }

    // Actions
    EXPECT_EQ(0, QSPGetActionsCount());
    EXPECT_FALSE(QSPIsActionsChanged());
    EXPECT_EQ(-1, QSPGetSelActionIndex());
    EXPECT_TRUE(QSPExecuteSelActionCode(true));

    // Objects
    EXPECT_EQ(0, QSPGetObjectsCount());
    EXPECT_FALSE(QSPIsObjectsChanged());
    EXPECT_EQ(-1, QSPGetSelObjectIndex());

    QSPDeInit();
}

TEST(Game, loadBasicQSP)
{
    QSPInit();

    //auto result = QSPLoadGameWorld(L"assets\\test.qsp");
    auto result = QSPLoadGameWorld(L"assets/test.qsp");
    ASSERT_TRUE(result);

    EXPECT_EQ(0, QSPGetFullRefreshCount());

    // Command & Control
    EXPECT_FALSE(QSPIsInCallBack());

    // File information
    EXPECT_STRNE(L"", QSPGetQstFullPath());

    // Locations
    EXPECT_EQ(nullptr, QSPGetCurLoc());
    QSP_CHAR* loc;
    int actIndex, line;
    QSPGetCurStateData(&loc, &actIndex, &line);
    EXPECT_EQ(nullptr, loc);
    EXPECT_EQ(-1, actIndex);
    EXPECT_EQ(0, line);

    // Main Description
    EXPECT_EQ(nullptr, QSPGetMainDesc());
    EXPECT_FALSE(QSPIsMainDescChanged());

    // Vars Description
    EXPECT_EQ(nullptr, QSPGetVarsDesc());
    EXPECT_FALSE(QSPIsVarsDescChanged());

    // Variables
    EXPECT_LT(12799, QSPGetMaxVarsCount());
    QSP_CHAR* name;
    for (int i = 0; i < QSPGetMaxVarsCount(); ++i) {
        EXPECT_FALSE(QSPGetVarNameByIndex(i, &name));
    }

    // Actions
    EXPECT_EQ(0, QSPGetActionsCount());
    EXPECT_FALSE(QSPIsActionsChanged());
    EXPECT_EQ(-1, QSPGetSelActionIndex());
    EXPECT_TRUE(QSPExecuteSelActionCode(true));

    // Objects
    EXPECT_EQ(0, QSPGetObjectsCount());
    EXPECT_FALSE(QSPIsObjectsChanged());
    EXPECT_EQ(-1, QSPGetSelObjectIndex());


    QSPDeInit();
}

namespace {
    std::function<void(int, QSP_BOOL)> showWindowCallback;
    std::function<void(const QSP_CHAR*)> setInputStrTextCallback;
    std::function<void(const QSP_CHAR*)> closeFileCallback;
    std::function<void(const QSP_CHAR*)> showImageCallback;
    std::function<void(int)> setTimerCallback;

    extern "C" void showWindowWrapper(int type, QSP_BOOL isShow) {
        showWindowCallback(type, isShow);
    }

    extern "C" void setInputStrTextWrapper(const QSP_CHAR * text) {
        setInputStrTextCallback(text);
    }

    extern "C" void closeFileWrapper(const QSP_CHAR * file) {
        closeFileCallback(file);
    }

    extern "C" void showImageWrapper(const QSP_CHAR * file) {
        showImageCallback(file);
    }

    extern "C" void setTimerWrapper(int timer) {
        setTimerCallback(timer);
    }
}

extern "C" class Game {
public:
    MOCK_METHOD(void, ShowWindow, (int, QSP_BOOL));
    MOCK_METHOD(void, SetInputStrText, (const QSP_CHAR*));
    MOCK_METHOD(void, CloseFile, (const QSP_CHAR*));
    MOCK_METHOD(void, ShowImage, (const QSP_CHAR*));
    MOCK_METHOD(void, SetTimer, (int));

    void Register()
    {
        using namespace std::placeholders; // for `_1`

        showWindowCallback = std::bind(&Game::ShowWindow, this, _1, _2);
        setInputStrTextCallback = std::bind(&Game::SetInputStrText, this, _1);
        closeFileCallback = std::bind(&Game::CloseFile, this, _1);
        showImageCallback = std::bind(&Game::ShowImage, this, _1);
        setTimerCallback = std::bind(&Game::SetTimer, this, _1);

        QSPSetCallBack(QSP_CALL_SHOWWINDOW, (QSP_CALLBACK)&showWindowWrapper);
        QSPSetCallBack(QSP_CALL_SETINPUTSTRTEXT, (QSP_CALLBACK)&setInputStrTextWrapper);
        QSPSetCallBack(QSP_CALL_CLOSEFILE, (QSP_CALLBACK)&closeFileWrapper);
        QSPSetCallBack(QSP_CALL_SHOWIMAGE, (QSP_CALLBACK)&showImageWrapper);
        QSPSetCallBack(QSP_CALL_SETTIMER, (QSP_CALLBACK)&setTimerWrapper);
    }
    //MOCK_METHOD(void, ShowWindow2, (int, QSP_BOOL), (Calltype(__stdcall)));
    /*void showWindowInternal(int type, QSP_BOOL isShow)
    {
        ShowWindow(type, isShow);
    }*/
    //void showWindowNOP(int type, QSP_BOOL isShow)
    //{
    //    ASSERT_FALSE(true);
    //}
};

TEST(Game, startBasicGame)
{
    
    QSPInit();

    Game mockGameInterface;
    mockGameInterface.Register();

    //auto result = QSPLoadGameWorld(L"assets\\test.qsp");
    auto result = QSPLoadGameWorld(L"assets/test.qsp");
    ASSERT_TRUE(result);
    EXPECT_CALL(mockGameInterface, ShowWindow(_, true)).Times(Exactly(4));// << L"Expect ShowWindow to be call once for each window type";
    EXPECT_CALL(mockGameInterface, SetInputStrText(nullptr)).Times(Exactly(1));// << L"Expect SetInputStringText to be cleaned on startup";
    EXPECT_CALL(mockGameInterface, CloseFile(nullptr)).Times(Exactly(1));// << L"Expect file to be closed on startup (TODO: might have to deal with the new play command)";
    EXPECT_CALL(mockGameInterface, ShowImage(nullptr)).Times(Exactly(1));// << L"Expect image shown at start";
    EXPECT_CALL(mockGameInterface, SetTimer(500)).Times(Exactly(1));// << L"Expect timer tick to be set";

    ASSERT_TRUE(QSPRestartGame(true)) << L"Restart of a good qsp file should be sucessful";

    EXPECT_EQ(1, QSPGetFullRefreshCount()) << L"Refresh count should be at 1 after a game start";

    // Locations
    EXPECT_STREQ(L"start", QSPGetCurLoc()) << L"Current Location shuld be location at index 0";
    QSP_CHAR* loc;
    int actIndex, line;
    // Not sure about this one
    QSPGetCurStateData(&loc, &actIndex, &line);
    EXPECT_EQ(nullptr, loc);
    EXPECT_EQ(-1, actIndex);
    EXPECT_EQ(0, line);

    // Main Description
    EXPECT_STREQ(L"Hello From the Description", QSPGetMainDesc()) << L"Main Description should be accurate";
    EXPECT_TRUE(QSPIsMainDescChanged()) << L"Main Description should have changed";

    // Vars Description
    EXPECT_STREQ(L"Hello from the Execute on visit\r\n", QSPGetVarsDesc()) << L"Vars Description should be accurate";
    EXPECT_TRUE(QSPIsVarsDescChanged()) << L"Vars should have changed";

    // Variables
    QSP_CHAR* varString;
    int varIntValue;

    QSP_CHAR* name;
    int variableCount = 0;
    // Now we test that only the 2 variables we expect are set
    for (int i = 0; i < QSPGetMaxVarsCount(); ++i) {
        result = QSPGetVarNameByIndex(i, &name);
        if (result) {
            if (wcscmp(L"USEHTML", name) == 0) {
                EXPECT_TRUE(QSPGetVarValuesCount(L"USEHTML", &variableCount)) << L"USEHTML have a value set";
                EXPECT_EQ(1, variableCount) << L"USEHTML should not be empty";

                EXPECT_TRUE(QSPGetVarValues(L"USEHTML", 0, &varIntValue, &varString)) << L"USEHTML variable should be redeable";
                EXPECT_EQ(1, varIntValue) << L"Variable was set to 1 in code";
                EXPECT_EQ(nullptr, varString) << L"No string value set";
            }
            else if (wcscmp(L"ARGS", name) == 0) {
                EXPECT_TRUE(QSPGetVarValuesCount(L"ARGS", &variableCount)) << L"We expected ARGS to exist";
                EXPECT_EQ(0, variableCount) << L"ARGS should empty";

                EXPECT_FALSE(QSPGetVarValues(L"ARGS", 0, &varIntValue, &varString)) << L"Should not be able to read a non existing value";
            }
            else {
                EXPECT_FALSE(result) << L"No other variable should be set";
            }
        }
    }

    // Actions
    EXPECT_EQ(2, QSPGetActionsCount());
    EXPECT_TRUE(QSPIsActionsChanged());
    EXPECT_EQ(-1, QSPGetSelActionIndex());
    EXPECT_TRUE(QSPExecuteSelActionCode(true));

    // Objects
    EXPECT_EQ(2, QSPGetObjectsCount());
    EXPECT_TRUE(QSPIsObjectsChanged());
    EXPECT_EQ(-1, QSPGetSelObjectIndex());

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

