#include <fstream>

#include "locations.h"
#include "common.h"
#include "variables.h"
#include "qsp/jack.h"
#include "game.h"
#include "callbacks.h"
#include "objects.h"

//void Jack::executeJSON()
//{
//      //  loadGameStatus();
//}

//QList<QString> Jack::getImageArrays(QString key)
//{
//    if (image_arrays.contains(key))
//        return image_arrays[key];
//    else
//        return QList<QString>();
//}

//void Jack::iterateKeys(const std::ifstream& stream, int array_indice)
//{
    //QJsonObject obj = doc.object();

    //for (QString key : obj.keys())
    //{
    //    if (obj.value(key).isString())
    //    {
    //        QString value = obj.value(key).toString();
    //        if (array_indice == 0)
    //        {
    //            if (key.at(0) != '$')
    //                key = "$" + key;
    //            setStringVariable(key, value);
    //        }
    //        else
    //            setStringVariable(key, array_indice, value);
    //    }
    //    else if (obj.value(key).isDouble() | obj.value(key).isBool())
    //    {
    //        int value = obj.value(key).toInt();
    //        if (array_indice == 0)
    //            setNumericVariable(key, value);
    //        else
    //            setNumericVariable(key, array_indice, value);
    //    }
    //    else if (obj.value(key).isArray())
    //    {
    //        QJsonArray img_array = obj[key].toArray();
    //        QVariantList list = img_array.toVariantList();

    //        //      if(image_arrays[list[0].toString()] == NULL)
    //        //        image_arrays[list[0].toString()] = new QList<QString>;
    //        //      else
    //        image_arrays[list[0].toString()].clear();

    //        for (int i = 0; i < list.size(); ++i)
    //        {
    //            if (i == 0)
    //                setStringVariable(key, array_indice, list[0].toString());

    //            image_arrays[list[0].toString()].append(list[i].toString());
    //        }
    //    }
    //    else if (obj.value(key).isObject())
    //    {
    //        QJsonObject menu_item = obj[key].toObject();

    //        for (QString key : menu_item.keys())
    //            setStringVariable(key, 0, menu_item.value(key).toString());
    //    }
    //}
//}

//void Jack::setStringVariable(QString key, int array_indice, QString value)
//{
//    int name_length = key.length();
//    int val_length = value.length();
//
//    QSP_CHAR* var_name = new QSP_CHAR[name_length + 1];
//    QSP_CHAR* qsp_value = new QSP_CHAR[val_length + 1];
//
//    key.toWCharArray(var_name);
//    value.toWCharArray(qsp_value);
//
//    var_name[name_length] = L'\0';
//    qsp_value[val_length] = L'\0';
//
//    QSPVar* var = qspVarReference(var_name, QSP_TRUE); //True = create if not found (I think)
//
//    QSPVariant val = qspGetEmptyVariant(QSP_TRUE); //True = string type
//    QSP_STR(val) = qsp_value;
//
//    //  qDebug() << "Key:" << key << "value:" << value << "indice:" << array_indice;
//
//    qspSetVarValueByReference(var, array_indice, &val);
//
//    delete[] var_name;
//    delete[] qsp_value;
//}

//void Jack::setStringVariable(QString key, QString value)
//{
//    QSP_CHAR* var_name = new QSP_CHAR[key.length() + 1];
//
//    key.toWCharArray(var_name);
//
//    var_name[key.length()] = L'\0';
//
//    QSP_CHAR* var_val = new QSP_CHAR[value.length() + 1];
//
//    value.toWCharArray(var_val);
//
//    var_val[value.length()] = L'\0';
//
//
//    QSPVariant variant = qspGetEmptyVariant(QSP_TRUE); // True = string type
//    QSP_STR(variant) = var_val;
//
//    qspSetVar(var_name, &variant, '=');
//
//    delete[] var_name;
//    delete[] var_val;
//}

//void Jack::setNumericVariable(QString key, int array_indice, int value)
//{
//    int name_length = key.length();
//
//    QSP_CHAR* var_name = new QSP_CHAR[name_length + 1];
//
//    key.toWCharArray(var_name);
//
//    var_name[name_length] = L'\0';
//
//    QSPVar* var = qspVarReference(var_name, QSP_TRUE); //True = create if not found (I think)
//
//    QSPVariant val = qspGetEmptyVariant(QSP_FALSE); //False = not string type
//    QSP_NUM(val) = value;
//
//    //  qDebug() << "Key:" << key << "value:" << value << "indice:" << array_indice;
//
//    qspSetVarValueByReference(var, array_indice, &val);
//
//
//    delete[] var_name;
//}

//void Jack::setNumericVariable(QString key, int value)
//{
//    QSP_CHAR* var_name = new QSP_CHAR[key.length() + 1];
//
//    key.toWCharArray(var_name);
//    var_name[key.length()] = L'\0';
//
//
//    QSPVariant variant = qspGetEmptyVariant(QSP_FALSE); //False = not string type
//    QSP_NUM(variant) = value;
//
//    qspSetVar(var_name, &variant, '=');
//
//    delete[] var_name;
//}
//
//void Jack::setStylesheet()
//{
//    QFileInfo file_info(*qspGameDirectory + "content/stylesheet.css");
//
//    if (file_info.isFile())
//    {
//        QFile file(file_info.filePath());
//        file.open(QIODevice::ReadOnly | QIODevice::Text);
//        setStringVariable("STYLESHEET", 0, QString(file.readAll()));
//        file.close();
//    }
//}

//QJsonObject Jack::loadJSONObjFromFile(QString filename)
//{
//    QFileInfo file_info(filename);
//
//    QFile file(file_info.absoluteFilePath());
//    file.open(QIODevice::ReadOnly | QIODevice::Text);
//    QByteArray json_data = file.readAll();
//    file.close();
//
//    QJsonDocument doc = QJsonDocument::fromJson(json_data);
//
//    return doc.object();
//}
//
//int Jack::qspCurrentObjectsCount()
//{
//    return qspCurObjectsCount;
//}
//
//Jack& Jack::getInstance()
//{
//    static Jack instance;
//
//    return instance;
//}
