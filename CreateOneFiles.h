#pragma once
#include <QString>
#include <QStringList>

#include "DBC.h"

class CreateOneFiles
{
public:
    CreateOneFiles(const QString& filePath, const QString& fileSavePath);
    ~CreateOneFiles();

    bool CreateSource();
    bool CreateHead();

private:
    QString GetMemberName(DBCFile* pDBCFile, int nField);
    QString GetFieldType(DBCFile* pDBCFile, int nField);
    QString GetFieldName(DBCFile* pDBCFile, int nField);

    void    CreateEnumList();

private:
    QString m_strFilePath;
    QString m_strBaseClassName;
    QString m_strTblClassName;
    QString m_strFileSavePath;
    QString m_strBaseName;
    QString m_strFileName;
    bool    bCanCre;

    QStringList m_strEnumList;

    std::string m_strTblFilePath;
};
