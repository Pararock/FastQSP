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
using ::testing::_;
using ::testing::AtLeast;
using ::testing::Exactly;
using ::testing::Mock;
using ::testing::NiceMock;

#include <string>
#include <functional>
#include <string.h>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

//std::wstring QSP_SIMPLE_GAME_PATH = L"/mnt/c/dev/Perso/fastqsp/FastQSPIva/vsout/qsp/test/Debug/assets/test.qsp";

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

namespace
{
    std::function<void(int, QSP_BOOL)> showWindowCallback;
    std::function<void(const QSP_CHAR*)> setInputStrTextCallback;
    std::function<void(const QSP_CHAR*)> closeFileCallback;
    std::function<void(const QSP_CHAR*)> showImageCallback;
    std::function<void(int)> setTimerCallback;
    std::function<void(QSP_BOOL)> refreshIntCallback;

    extern "C" void showWindowWrapper(int type, QSP_BOOL isShow)
    {
        showWindowCallback(type, isShow);
    }

    extern "C" void setInputStrTextWrapper(const QSP_CHAR * text)
    {
        setInputStrTextCallback(text);
    }

    extern "C" void closeFileWrapper(const QSP_CHAR * file)
    {
        closeFileCallback(file);
    }

    extern "C" void showImageWrapper(const QSP_CHAR * file)
    {
        showImageCallback(file);
    }

    extern "C" void setTimerWrapper(int timer)
    {
        setTimerCallback(timer);
    }

    extern "C" void refreshIntWrapper(QSP_BOOL isRedraw)
    {
        refreshIntCallback(isRedraw);
    }
}

extern "C" class Game
{
public:
    MOCK_METHOD(void, ShowWindow, (int, QSP_BOOL));
    MOCK_METHOD(void, SetInputStrText, (const QSP_CHAR*));
    MOCK_METHOD(void, CloseFile, (const QSP_CHAR*));
    MOCK_METHOD(void, ShowImage, (const QSP_CHAR*));
    MOCK_METHOD(void, SetTimer, (int));
    MOCK_METHOD(void, RefreshInt, (QSP_BOOL));


    void Register()
    {
        using namespace std::placeholders; // for `_1`
        showWindowCallback = std::bind(&Game::ShowWindow, this, _1, _2);
        setInputStrTextCallback = std::bind(&Game::SetInputStrText, this, _1);
        closeFileCallback = std::bind(&Game::CloseFile, this, _1);
        showImageCallback = std::bind(&Game::ShowImage, this, _1);
        setTimerCallback = std::bind(&Game::SetTimer, this, _1);
        refreshIntCallback = std::bind(&Game::RefreshInt, this, _1);

        QSPSetCallBack(QSP_CALL_SHOWWINDOW, (QSP_CALLBACK)&showWindowWrapper);
        QSPSetCallBack(QSP_CALL_SETINPUTSTRTEXT, (QSP_CALLBACK)&setInputStrTextWrapper);
        QSPSetCallBack(QSP_CALL_CLOSEFILE, (QSP_CALLBACK)&closeFileWrapper);
        QSPSetCallBack(QSP_CALL_SHOWIMAGE, (QSP_CALLBACK)&showImageWrapper);
        QSPSetCallBack(QSP_CALL_SETTIMER, (QSP_CALLBACK)&setTimerWrapper);
        QSPSetCallBack(QSP_CALL_REFRESHINT, (QSP_CALLBACK)&refreshIntWrapper);
    }

    void UnRegister()
    {
        showWindowCallback = nullptr;
        setInputStrTextCallback = nullptr;
        closeFileCallback = nullptr;
        showImageCallback = nullptr;
        setTimerCallback = nullptr;
        refreshIntCallback = nullptr;

        QSPSetCallBack(QSP_CALL_SHOWWINDOW, (QSP_CALLBACK)nullptr);
        QSPSetCallBack(QSP_CALL_SETINPUTSTRTEXT, (QSP_CALLBACK)nullptr);
        QSPSetCallBack(QSP_CALL_CLOSEFILE, (QSP_CALLBACK)nullptr);
        QSPSetCallBack(QSP_CALL_SHOWIMAGE, (QSP_CALLBACK)nullptr);
        QSPSetCallBack(QSP_CALL_SETTIMER, (QSP_CALLBACK)nullptr);
        QSPSetCallBack(QSP_CALL_REFRESHINT, (QSP_CALLBACK)nullptr);
    }
};

class GameTest : public ::testing::Test
{
protected:
    NiceMock<Game> mockGameInterface;

    GameTest()
    {
        QSPInit();
    }

    ~GameTest() override
    {
        QSPDeInit();
        //mockGameInterface.UnRegister();
    }

    void SetUp()
    {

    }

    void TearDown() override
    {
    }

    void LoadGame(const QSP_CHAR* qspGame) {
        ASSERT_TRUE(QSPLoadGameWorld(qspGame));
    }
private:
};

class BasicGameTest : public GameTest
{
protected:
    NiceMock<Game> mockGameInterface;
    fs::path baseFolder;
    fs::path game;

    BasicGameTest() {
        baseFolder = std::filesystem::current_path() / L"assets";
        game = baseFolder / L"test.qsp";
    }

    void SetUp() override
    {
        mockGameInterface.Register();
        LoadGame(game.wstring().c_str());
    }

    void TearDown() override
    {
        mockGameInterface.UnRegister();
    }
private:
};

TEST_F(GameTest, loadBasicQSP)
{
    // We don't call LoadGame() here to test empty default
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
    for (int i = 0; i < QSPGetMaxVarsCount(); ++i)
    {
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
}

TEST_F(BasicGameTest, mainDescription)
{
    ASSERT_TRUE(QSPRestartGame(true)) << L"Restart of a good qsp file should be sucessful";

    EXPECT_EQ(2, QSPGetFullRefreshCount()) << L"Refresh count should be at 2 after a redirect game start";

    // Main Description
    EXPECT_STREQ(L"\r\n<h1>QSP Demo File</h1>\r\n\r\n<p>QSP is a open source multi-platform game engine for running text based games. It can be rendered as HTML like this page or <a href=\"EXEC:GT 'TEXT'\">text only</a>.</p>\r\n\r\n<p></p>\r\n\r\n<p>You are currently in Welcome running 5.7.0</p>\r\n\r\n\r\n<!-- $newLocationExecutedEvent  -->", QSPGetMainDesc()) << L"Main Description should be accurate";
    EXPECT_TRUE(QSPIsMainDescChanged()) << L"Main Description should have changed";

    //QSPDeInit();
}

TEST_F(BasicGameTest, callback)
{
    for (int i = 0; i < 4; i++)
    {
        EXPECT_CALL(mockGameInterface, ShowWindow(i, true)).Times(Exactly(1));      // << L"Expect ShowWindow to be call once for each window type";
    }

    EXPECT_CALL(mockGameInterface, SetInputStrText(nullptr)).Times(Exactly(1)); // << L"Expect SetInputStringText to be cleaned on startup";
    EXPECT_CALL(mockGameInterface, CloseFile(nullptr)).Times(Exactly(1));       // << L"Expect file to be closed on startup (TODO: might have to deal with the new play command)";
    EXPECT_CALL(mockGameInterface, ShowImage(nullptr)).Times(Exactly(1));       // << L"Expect image shown at start";
    EXPECT_CALL(mockGameInterface, SetTimer(500)).Times(Exactly(1));            // << L"Expect timer tick to be set";
    EXPECT_CALL(mockGameInterface, RefreshInt(false)).Times(Exactly(1));        // << L"Expect Refresh Int to be called for a redraw";

    ASSERT_TRUE(QSPRestartGame(true)) << L"Restart of a good qsp file should be sucessful";
}

TEST_F(BasicGameTest, varsDescription)
{
    ASSERT_TRUE(QSPRestartGame(true)) << L"Restart of a good qsp file should be sucessful";

    // Vars Description
    EXPECT_STREQ(L"You are in location: Welcome\r\n", QSPGetVarsDesc()) << L"Vars Description should be accurate";
    EXPECT_TRUE(QSPIsVarsDescChanged()) << L"Vars should have changed";
}

TEST_F(BasicGameTest, actions)
{
    ASSERT_TRUE(QSPRestartGame(true)) << L"Restart of a good qsp file should be sucessful";

    EXPECT_EQ(2, QSPGetActionsCount());
    QSP_CHAR* imgPath, * desc;
    QSPGetActionData(0, &imgPath, &desc);
    EXPECT_EQ(nullptr, imgPath);
    EXPECT_STREQ(L"SecondPage", desc);

    QSPGetActionData(1, &imgPath, &desc);
    auto action0ImgExpecteDPath = baseFolder / L"pics/action0.bmp";
    auto action0ReceivedPath = fs::path(imgPath);
    EXPECT_TRUE(fs::equivalent(action0ImgExpecteDPath, action0ReceivedPath));
    EXPECT_STREQ(L"Action onnewloc 0", desc);

    EXPECT_TRUE(QSPIsActionsChanged());
    EXPECT_EQ(-1, QSPGetSelActionIndex());
    EXPECT_TRUE(QSPExecuteSelActionCode(true));
}

TEST_F(BasicGameTest, objects)
{
    ASSERT_TRUE(QSPRestartGame(true)) << L"Restart of a good qsp file should be sucessful";

    EXPECT_EQ(0, QSPGetObjectsCount());
    EXPECT_FALSE(QSPIsObjectsChanged());
    EXPECT_EQ(-1, QSPGetSelObjectIndex());
}

TEST_F(BasicGameTest, locations)
{
    ASSERT_TRUE(QSPRestartGame(true)) << L"Restart of a good qsp file should be sucessful";

    EXPECT_STREQ(L"Welcome", QSPGetCurLoc()) << L"Current Location should be lin welcome since location 0 redirect to welcome";
    QSP_CHAR* loc;
    int actIndex, line;
    // Not sure about this one seems only use internal, might be expose in debug mode
    QSPGetCurStateData(&loc, &actIndex, &line);
    EXPECT_EQ(nullptr, loc);
    EXPECT_EQ(-1, actIndex);
    EXPECT_EQ(0, line);
}

TEST_F(BasicGameTest, loadSave)
{
    ASSERT_TRUE(QSPRestartGame(true)) << L"Restart of a good qsp file should be sucessful";
}

TEST_F(BasicGameTest, variables)
{
    ASSERT_TRUE(QSPRestartGame(true)) << L"Restart of a good qsp file should be sucessful";

    QSP_CHAR* varString;
    QSP_BOOL result;
    int varIntValue;

    QSP_CHAR* name;
    int variableCount = 0;

    for (int i = 0; i < QSPGetMaxVarsCount(); ++i)
    {
        result = QSPGetVarNameByIndex(i, &name);
        if (result)
        {
            if (wcscmp(L"USEHTML", name) == 0)
            {
                EXPECT_TRUE(QSPGetVarValuesCount(L"USEHTML", &variableCount)) << L"USEHTML have a value set";
                EXPECT_EQ(1, variableCount) << L"USEHTML should not be empty";

                EXPECT_TRUE(QSPGetVarValues(L"USEHTML", 0, &varIntValue, &varString)) << L"USEHTML variable should be redeable";
                EXPECT_EQ(1, varIntValue) << L"Variable was set to 1 in code";
                EXPECT_EQ(nullptr, varString) << L"No string value set";
            }
            else if (wcscmp(L"ONNEWLOC", name) == 0)
            {
                EXPECT_TRUE(QSPGetVarValuesCount(L"ONNEWLOC", &variableCount)) << L"We expected ONNEWLOC to exist";
                EXPECT_EQ(1, variableCount) << L"ONNEWLOC should have 1 value";

                EXPECT_TRUE(QSPGetVarValues(L"ONNEWLOC", 0, &varIntValue, &varString)) << L"ONNEWLOC variable should be redeable";
                EXPECT_EQ(0, varIntValue) << L"int value irrelevant";
                EXPECT_STREQ(L"$onNewLocationEvent", varString) << L"expected ONNEWLOC variable to hold the right location name";
            }
            else if (wcscmp(L"ONOBJSEL", name) == 0)
            {
                EXPECT_TRUE(QSPGetVarValuesCount(L"ONOBJSEL", &variableCount)) << L"We expected ONOBJSEL to exist";
                EXPECT_EQ(1, variableCount) << L"ONOBJSEL should empty";

                EXPECT_TRUE(QSPGetVarValues(L"ONOBJSEL", 0, &varIntValue, &varString)) << L"ONOBJSEL variable should be redeable";
                EXPECT_STREQ(L"$onObjectSelectedEvent", varString) << L"expected ONOBJSEL variable to hold the right location name";
            }
            else if (wcscmp(L"ONOBJADD", name) == 0)
            {
                EXPECT_TRUE(QSPGetVarValuesCount(L"ONOBJADD", &variableCount)) << L"We expected ONOBJADD to exist";
                EXPECT_EQ(1, variableCount) << L"ONOBJADD should empty";

                EXPECT_TRUE(QSPGetVarValues(L"ONOBJADD", 0, &varIntValue, &varString)) << L"ONOBJADD variable should be redeable";
                EXPECT_STREQ(L"$onObjectAddedEvent", varString) << L"expected ONOBJADD variable to hold the right location name";
            }
            else if (wcscmp(L"ONGSAVE", name) == 0)
            {
                EXPECT_TRUE(QSPGetVarValuesCount(L"ONGSAVE", &variableCount)) << L"We expected ONGSAVE to exist";
                EXPECT_EQ(1, variableCount) << L"ONGSAVE should empty";

                EXPECT_TRUE(QSPGetVarValues(L"ONGSAVE", 0, &varIntValue, &varString)) << L"ONGSAVE variable should be redeable";
                EXPECT_STREQ(L"$onGameSaveEvent", varString) << L"expected ONNEWLOC variable to hold the right location name";
            }
            else if (wcscmp(L"ONOBJDEL", name) == 0)
            {
                EXPECT_TRUE(QSPGetVarValuesCount(L"ONOBJDEL", &variableCount)) << L"We expected ONOBJDEL to exist";
                EXPECT_EQ(1, variableCount) << L"ONOBJDEL should empty";

                EXPECT_TRUE(QSPGetVarValues(L"ONOBJDEL", 0, &varIntValue, &varString)) << L"ONOBJDEL variable should be redeable";
                EXPECT_STREQ(L"$onObjectDeletedEvent", varString) << L"expected ONNEWLOC variable to hold the right location name";
            }
            else if (wcscmp(L"ONACTSEL", name) == 0)
            {
                EXPECT_TRUE(QSPGetVarValuesCount(L"ONACTSEL", &variableCount)) << L"We expected ONACTSEL to exist";
                EXPECT_EQ(1, variableCount) << L"ONACTSEL should empty";

                EXPECT_TRUE(QSPGetVarValues(L"ONACTSEL", 0, &varIntValue, &varString)) << L"ONACTSEL variable should be redeable";
                EXPECT_STREQ(L"$onActionSelectedEvent", varString) << L"expected ONNEWLOC variable to hold the right location name";
            }
            else if (wcscmp(L"ONGLOAD", name) == 0)
            {
                EXPECT_TRUE(QSPGetVarValuesCount(L"ONGLOAD", &variableCount)) << L"We expected ONGLOAD to exist";
                EXPECT_EQ(1, variableCount) << L"ONGLOAD should empty";

                EXPECT_TRUE(QSPGetVarValues(L"ONGLOAD", 0, &varIntValue, &varString)) << L"ONGLOAD variable should be redeable";
                EXPECT_STREQ(L"$onGameLoadEvent", varString) << L"expected ONGLOAD variable to hold the right location name";
            }
            else if (wcscmp(L"ARGS", name) == 0)
            {
                EXPECT_TRUE(QSPGetVarValuesCount(L"ARGS", &variableCount)) << L"We expected ARGS to exist";
                EXPECT_EQ(0, variableCount) << L"ARGS should empty";

                EXPECT_FALSE(QSPGetVarValues(L"ARGS", 0, &varIntValue, &varString)) << L"Should not be able to read a non existing value";
            }
            else if (wcscmp(L"RESULT", name) == 0)
            {
                EXPECT_TRUE(QSPGetVarValuesCount(L"RESULT", &variableCount)) << L"We expected RESULT to exist";
                EXPECT_EQ(0, variableCount) << L"RESULT should empty";
            }
            else
            {
                EXPECT_FALSE(result) << name << L" is set, but no other variable was expected";
            }
        }
    }
}


TEST(Game, Default)
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
    for (int i = 0; i < QSPGetMaxVarsCount(); ++i)
    {
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