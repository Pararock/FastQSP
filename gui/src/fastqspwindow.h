#ifndef QSPVIEW_H
#define QSPVIEW_H

#include <qsp/default/qsp_default.h>
#include <qsp/jack.h>
#include "qsp_htmlbuilder.h"

#include <qglobal.h>
#include <QApplication>
#include <QMainWindow>
#include <QResizeEvent>
#include <QMenuBar>
#include <QMenu>
#include <QFileDialog>
#include <QTextEdit>
#include <QWebEngineView>
#include <QString>
#include <QRegExp>
#include <QDebug>
#include <QGridLayout>
#include <QBoxLayout>
#include <QFontDatabase>
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QShortcut>
#include <QDesktopServices>
#include <QTime>
#include <QSettings>
#include <QTimer>
#include <QGraphicsTextItem>


#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QMap>
#include "audiostream.h"

#include <src\url_schemes.h>

class FastQSPWindow : public QMainWindow {
  Q_OBJECT
public:
  explicit FastQSPWindow(QWidget *parent = 0);
  ~FastQSPWindow();
  void openFile(const QString &filename);
  void playAudio(QString filename, int vol, QString flags);
  void stopAudio(QString filename);
  void refreshView();
  int getTimeFromStart();

signals:

public slots:
  void saveGame(const QString &filename);
  void saveGameDialog();
  void openFileDialog();

protected:
  void timerEvent(QTimerEvent *event);
  void resizeEvent(QResizeEvent *event);
  bool eventFilter(QObject *, QEvent *e);
  void closeEvent(QCloseEvent *event);

private slots:
  void about();
  void showHtml();
  void autosave();
  void loadGameDialog();
  void reloadQSP();
  void loadGame(const QString &filename);
  void quicksave();
  void quickload();
  void startQuickloading();
  void restartGame();
  void linkClicked(const QUrl &url);
  void toggleFullscreen();
  void toggleUpdate();
  void replayVideo(qint64 pos);
  void saveIgnoreCRCState();
  void saveAutosaveWhenExiting();
  void saveMutedState(); //Reminder: One more option to be saved: Do proper settings handling!
  void hideSaveStatus();
  void nextScreen();
  void prevScreen();
  void gotoMainScreen();

private:
//  Jack *qspJack;
  Jack& qspJack = Jack::getInstance();
  QMenu *gameMenu;
  QAction *autosaveAction;
  QAction *ignoreCRCAction;
  QAction *muteAction;
  QWebEngineView *webView;
  QGraphicsView *graphicsView;
  QGraphicsScene *scene;
  QMediaPlayer *videoPlayer ;
  QGraphicsVideoItem *videoItem;
  RequestHandlers requestHandlers;
  QString gameDirectory;
  uint gameWidth;
  uint gameHeight;
  qreal aspectRatio;
  qreal scaleFactor;
  bool gameIsOpen;
  QStringList scanHTMLForImages(QString html);
  bool choseRandomImageFromArray(QStringList urlmatches);
  void maybePlayVideo(QString html, QStringList urlmatches);
  QSettings settings;
  QGraphicsTextItem *savestatus = new QGraphicsTextItem;
  bool replaceHTML = false;
  QString qspFilePath = "";
  QString origImage, newImage; //newImage contains a randomly chosen image,
                               //from a given JSON array. Is set from choseRandomImageFromArray.

  QMediaPlayer *player;
  QMap<QString, AudioStream*> audio;
  QTime timer;
  QDir saveDir;
  void loadFonts();
  void loadPage();
  void displayLastError();
};

#endif // QSPVIEW_H
