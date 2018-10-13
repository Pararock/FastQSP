#ifndef JACK_H
#define JACK_H

#include <QtCore/QtGlobal>
#define JACKQSP_EXPORT Q_DECL_EXPORT
#define JACKQSP_EXPORT2

#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <QList>
#include "common.h"
#include <QDebug>

class JACKQSP_EXPORT Jack
{
public:
  JACKQSP_EXPORT2 static Jack& getInstance();

  void executeJSON();
  QList<QString> getImageArrays(QString key);
  JACKQSP_EXPORT2 int qspCurrentObjectsCount();
  JACKQSP_EXPORT2 bool isGotoMainScreenAcceptable();

public slots:
  JACKQSP_EXPORT2 void saveGameStatus(QString filename);
  JACKQSP_EXPORT2 void loadGameStatus(QString filename);

private:
  Jack() { qDebug() << "success";}
  Jack(Jack const&) = delete;
  void operator=(Jack const&) = delete;

  void iterateKeys(const QJsonDocument& doc, int array_indice);
  void setStringVariable(QString key, int array_indice, QString value);
  void setStringVariable(QString key, QString value);
  void setNumericVariable(QString key, int array_indice, int value);
  void setNumericVariable(QString key, int value);
  void executeLocation(QString location);
  void prepareLocations(QString current_location, int interaction_city);
  void setStylesheet();
  QJsonObject loadJSONObjFromFile(QString filename);
  QMap<QString, QList<QString>> image_arrays;

};

#endif // JACK_H
