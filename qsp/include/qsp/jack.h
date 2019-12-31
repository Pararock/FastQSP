//#ifndef JACK_H
//#define JACK_H
//
//#include <string>
//#define JACKQSP_EXPORT Q_DECL_EXPORT
//#define JACKQSP_EXPORT2
//
//class JACKQSP_EXPORT Jack
//{
//public:
//  JACKQSP_EXPORT2 static Jack& getInstance();
//
//  void executeJSON();// QSP_EXTERN void QSPGetActionData(int ind, QSP_CHAR** imgPath, QSP_CHAR** desc);
//  //std::array<std::string>> getImageArrays(std::string_view);
//  JACKQSP_EXPORT2 int qspCurrentObjectsCount();
//  JACKQSP_EXPORT2 bool isGotoMainScreenAcceptable();
//
//public slots:
//
//private:
//  Jack() { qDebug() << "success";}
//  Jack(Jack const&) = delete;
//  void operator=(Jack const&) = delete;
//
//  void iterateKeys(const std::ifstream& jsonContent, int array_indice);
//  void setStringVariable(QString key, int array_indice, QString value);
//  void setStringVariable(QString key, QString value);
//  void setNumericVariable(QString key, int array_indice, int value);
//  void setNumericVariable(QString key, int value);
//  void setStylesheet();
//  QJsonObject loadJSONObjFromFile(QString filename);
//  QMap<QString, QList<QString>> image_arrays;
//
//};
//
//#endif // JACK_H
