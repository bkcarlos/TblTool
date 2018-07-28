#include "CreateOneFiles.h"
#include <QFileInfo>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

CreateOneFiles::CreateOneFiles(const QString& filePath, const QString& fileSavePath)
{
    if(filePath.isEmpty() || fileSavePath.isEmpty())
    {
        QMessageBox::warning(nullptr, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("文件名为空"));
        bCanCre = false;
        return;
    }

    QFileInfo fileInfo(filePath);
    m_strFileSavePath = fileSavePath;

    m_strBaseName = fileInfo.baseName();
    m_strFileName = fileInfo.fileName();
    m_strBaseName = m_strBaseName.at(0).toUpper() + QString(m_strBaseName.data() + 1);

    m_strTblClassName = m_strBaseName + "Tbl";
    m_strBaseClassName = QString("CFG_") + m_strBaseName;

    QByteArray baTemp = filePath.toLatin1();
    m_strTblFilePath = baTemp.data();

    CreateEnumList();

    bCanCre = true;
}

CreateOneFiles::~CreateOneFiles()
{
}

bool CreateOneFiles::CreateSource()
{
    if(!bCanCre)
    {
        QMessageBox::warning(nullptr, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("文件名为空"));
        return false;
    }

    DBCFile fileDBC(0);
    if(fileDBC.OpenFromTXT(m_strTblFilePath.c_str()) == false)
    {
        return false;
    }
    int nCol = fileDBC.GetFieldsNum();

    QString saveFileSourceName = m_strFileSavePath + "//" + m_strTblClassName + ".cpp";
    QFile fileSource(saveFileSourceName);
    if(!fileSource.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream outFileSource(&fileSource);

    outFileSource << "#include \"" << m_strTblClassName << ".h\"" << "\n";
    outFileSource << "#include \"../common/DBC.h\"" << "\n";
    outFileSource << "const char* " << m_strTblClassName << "::szConfigFile = \"Config/" << m_strFileName << "\";" << "\n\n";

    outFileSource << m_strTblClassName << "::" << m_strTblClassName << "(void)" << "\n";
    outFileSource << "{" << "\n";
    outFileSource << "}" << "\n\n";

    outFileSource << m_strTblClassName << "::" << "~" << m_strTblClassName << "(void)" << "\n";
    outFileSource << "{" << "\n";
    outFileSource << "\tClear();" << "\n";
    outFileSource << "}" << "\n\n";

    //loadinfo function
    outFileSource << "void " << m_strTblClassName << "::LoadInfo()" << "\n";
    outFileSource << "{" << "\n";
    outFileSource << "\t" << "Clear();" << "\n\n";
    outFileSource << "\t" << "DBCFile fileDBC(0);" << "\n";
    outFileSource << "\t" << "if( fileDBC.OpenFromTXT(szConfigFile) == false)" << "\n";
    outFileSource << "\t" << "{\n";
    outFileSource << "\t\t" << "return;\n";
    outFileSource << "\t" << "}\n\n";

    outFileSource << "\t" << "int nRow = fileDBC.GetRecordsNum();" << "\n";
    outFileSource << "\t" << "if( nRow <= 0 )\n";
    outFileSource << "\t" << "{\n";
    outFileSource << "\t\t" << "return;\n";
    outFileSource << "\t" << "}\n\n";

    outFileSource << "\t" << "for (int i = 0; i < nRow; i++)" << "\n";
    outFileSource << "\t" << "{" << "\n";
    outFileSource << "\t\t" << m_strBaseClassName << " *pTblBase" << "= new " << m_strBaseClassName << ";" << "\n";
    outFileSource << "\t\t" << "if ( NULL == pTblBase)" << "\n";
    outFileSource << "\t\t" << "{" << "\n";
    outFileSource << "\t\t\t" << "continue;" << "\n";
    outFileSource << "\t\t" << "}" << "\n\n";

    bool bFirstString = true;
    for(int i = 0; i < nCol; i++)
    {
        int nFieldType = fileDBC.GetFieldType(i);
        QString strValueName;
        if(nFieldType == 0)
        {
            strValueName = "iValue";
            outFileSource << "\t\t" << "pTblBase" << "->set_" << GetFieldName(&fileDBC, i) << "(fileDBC.Search_Posistion(i, " << m_strEnumList[i] << ")->" << strValueName << ");" << "\n";
        }
        else if(nFieldType == 1)
        {
            strValueName = "fValue";
            outFileSource << "\t\t" << "pTblBase" << "->set_" << GetFieldName(&fileDBC, i) << "(fileDBC.Search_Posistion(i, " << m_strEnumList[i] << ")->" << strValueName << ");" << "\n";
        }
        else if(nFieldType == 2)
        {
            strValueName = "pString";
            if(bFirstString)
            {
                outFileSource << "\t\tconst char* ";
                bFirstString = false;
            }
            else
            {
                outFileSource << "\t\t";
            }
            outFileSource << "pString = fileDBC.Search_Posistion(i, " << m_strEnumList[i] << ")->pString;" << "\n";
            outFileSource << "\t\tif(NULL != pString && strcmp(pString, \"\") != 0)" << "\n";
            outFileSource << "\t\t{" << "\n";
            outFileSource << "\t\t\t// !!!!todo add deal with pString !!!!!" << "\n";
            outFileSource << "\t\t\t" << "pTblBase" << "->set_" << GetFieldName(&fileDBC, i) << "(pString);" << "\n\n";
            outFileSource << "\t\t}" << "\n";
        }
    }
    outFileSource << "\n\t\t" << "m_mapTblBase" << ".insert(make_pair(pTblBase->get_" << GetFieldName(&fileDBC, 0) << "(), pTblBase" << "));" << "\n\n";
    outFileSource << "\t" << "}" << "\n";
    outFileSource << "\t" << "printf(\"Load %s cnt:%d\\n\", " << m_strTblClassName << "::szConfigFile" << ", nRow);" << "\n";
    outFileSource << "}" << "\n\n";

    //clear function
    outFileSource << "void " << m_strTblClassName << "::Clear()" << "\n";
    outFileSource << "{" << "\n";
    outFileSource << "\tstd::map<int, " << m_strBaseClassName << "*>::iterator iter = m_mapTblBase.begin();" << "\n";
    outFileSource << "\tfor (; iter != m_mapTblBase.end(); ++iter)" << "\n";
    outFileSource << "\t" << "{" << "\n";
    outFileSource << "\t\t" << m_strBaseClassName << " *pTblBase" << " = static_cast<" << m_strBaseClassName << "*>(iter->second);" << "\n";
    outFileSource << "\t\t" << "if (NULL != pTblBase)\n";
    outFileSource << "\t\t" << "{" << "\n";
    outFileSource << "\t\t\t" << "delete pTblBase;" << "\n";
    outFileSource << "\t\t\t" << "pTblBase = NULL;" << "\n";
    outFileSource << "\t\t" << "}" << "\n";
    outFileSource << "\t" << "}" << "\n";
    outFileSource << "\t" << "m_mapTblBase.clear();" << "\n";
    outFileSource << "}" << "\n\n";

    //get function
    outFileSource << m_strBaseClassName << "* " << m_strTblClassName << "::GetInfo" << "(int nIndex)" << "\n";
    outFileSource << "{" << "\n";
    outFileSource << "\t" << "map<int," << m_strBaseClassName << "*>::iterator iterFind = m_mapTblBase.find(nIndex);" << "\n";
    outFileSource << "\t" << "if (iterFind != m_mapTblBase.end())" << "\n";
    outFileSource << "\t" << "{" << "\n";
    outFileSource << "\t\t" << "return static_cast<" << m_strBaseClassName << "*>(iterFind->second);" << "\n";
    outFileSource << "\t" << "}" << "\n";
    outFileSource << "\t" << "return NULL;" << "\n";
    outFileSource << "}" << "\n\n";

    outFileSource.flush();
    fileSource.close();

    return true;
}

bool CreateOneFiles::CreateHead()
{
    if(!bCanCre)
    {
        QMessageBox::warning(nullptr, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("文件名为空"));
        return false;
    }

    DBCFile fileDBC(0);
    if(fileDBC.OpenFromTXT(m_strTblFilePath.c_str()) == false)
    {
        return false;
    }

    int nCol = fileDBC.GetFieldsNum();

    QString saveFileHName = m_strFileSavePath + "//" + m_strTblClassName + ".h";
    QFile fileHead(saveFileHName);
    if(!fileHead.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream outFileHead(&fileHead);

    outFileHead << "#ifndef _" << m_strTblClassName.toUpper() << "_H_" << "\n";
    outFileHead << "#define _" << m_strTblClassName.toUpper() << "_H_" << "\n";
    outFileHead << "\n";

    outFileHead << "#include <map>" << "\n";
    outFileHead << "#include <string>" << "\n";
    outFileHead << "#include \"../common/Singleton.h\"" << "\n\n";

    // enum  value
    outFileHead << "enum " << m_strBaseName << "_type" << "\n";
    outFileHead << "{" << "\n";

    for(int i = 0; i < m_strEnumList.size(); ++i)
    {
        outFileHead << "\t" << m_strEnumList[i] << ",\n";
    }

    outFileHead << "};" << "\n";
    outFileHead << "\n";

    // CFG_class
    outFileHead << "class " << m_strBaseClassName << "\n";
    outFileHead << "{" << "\n";
    outFileHead << "public:" << "\n";

    // init
    outFileHead << "\t" << m_strBaseClassName << "()";
    for(int i = 0; i < nCol; i++)
    {
        if(GetFieldType(&fileDBC, i) == "int")
        {
            if(i == 0)
            {
                outFileHead << "\n\t\t: ";
            }
            else
            {
                outFileHead << "\n\t\t, ";
            }
            outFileHead << GetMemberName(&fileDBC, i) << "(0)";
        }
        else if(GetFieldType(&fileDBC, i) == "float")
        {
            if(i == 0)
            {
                outFileHead << "\n\t\t: ";
            }
            else
            {
                outFileHead << "\n\t\t, ";
            }

            outFileHead << GetMemberName(&fileDBC, i) << "(0.0)";
        }
        else if(GetFieldType(&fileDBC, i) == "std::string")
        {
            if(i == 0)
            {
                outFileHead << "\n\t\t: ";
            }
            else
            {
                outFileHead << "\n\t\t, ";
            }
            outFileHead << GetMemberName(&fileDBC, i) << "(\"\")";
        }
        else
        {
            continue;
        }
    }
    outFileHead << "\n\t{};\n";

    // ~CFG_class()
    outFileHead << "\t" << "~" << m_strBaseClassName << "(){};" << "\n\n";

    for(int i = 0; i < nCol; ++i)
    {
        if(GetFieldType(&fileDBC, i) == "int")
        {
            outFileHead << "\t" << GetFieldType(&fileDBC, i) << "\t\t" << "get_" << GetFieldName(&fileDBC, i) << "() const { return " << GetMemberName(&fileDBC, i) << "; }" << "\n";
        }
        else
        {
            outFileHead << "\t" << GetFieldType(&fileDBC, i) << "\t" << "get_" << GetFieldName(&fileDBC, i) << "() const { return " << GetMemberName(&fileDBC, i) << "; }" << "\n";
        }

        outFileHead << "\t" << "void" << "\t" << "set_" << GetFieldName(&fileDBC, i) << "(" << GetFieldType(&fileDBC, i) << " val) { " << GetMemberName(&fileDBC, i) << " = val; }" << "\n\n";
    }

    outFileHead << "private:" << "\n";

    for(int i = 0; i < nCol; i++)
    {
        if(GetFieldType(&fileDBC, i) == "int")
        {
            outFileHead << "\t" << GetFieldType(&fileDBC, i) << "\t\t" << GetMemberName(&fileDBC, i) << ";\n";
        }
        else
        {
            outFileHead << "\t" << GetFieldType(&fileDBC, i) << "\t" << GetMemberName(&fileDBC, i) << ";\n";
        }
    }

    outFileHead << "};";
    outFileHead << "\n\n";

    outFileHead << "class " << m_strTblClassName << "\n";
    outFileHead << "{" << "\n";
    outFileHead << "public:" << "\n";
    outFileHead << "\t" << m_strTblClassName << "();" << "\n";
    outFileHead << "\t~" << m_strTblClassName << "();" << "\n\n";

    outFileHead << "\t" << "void\t\tLoadInfo();" << "\n";
    outFileHead << "\t" << "void\t\tClear();" << "\n\n";

    outFileHead << "\t" << m_strBaseClassName << "*\t GetInfo(int nIndex);" << "\n\n";
    outFileHead << "private:" << "\n";

    outFileHead << "\t" << "static const char* szConfigFile;" << "\n";
    outFileHead << "\t" << "std::map<int, " << m_strBaseClassName << "*>\tm_mapTblBase;" << "\n";

    outFileHead << "};" << "\n\n";

    outFileHead << "typedef Singleton<" << m_strTblClassName << "> " << m_strTblClassName << "Inst;\n\n";
    outFileHead << "#endif" << "\n\n";

    outFileHead.flush();
    fileHead.close();

    return true;
}

QString CreateOneFiles::GetMemberName(DBCFile* pDBCFile, int nField)
{
    if(nullptr == pDBCFile)
    {
        return QString();
    }

    int nFieldType = pDBCFile->GetFieldType(nField);
    std::string strFieldName = pDBCFile->GetFieldName(nField);
    if(strFieldName.empty())
    {
        QMessageBox::information(nullptr, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("取名不对"));
        return QString();
    }

    QString fieldName("m_");

    if(nFieldType == 0)
    {
        fieldName += "n";
    }
    else if(nFieldType == 1)
    {
        fieldName = "f";
    }
    else if(nFieldType == 2)
    {
        fieldName = "str";
    }

    QString strTemp = QString(strFieldName.c_str());
    fieldName += strTemp.at(0).toUpper() + QString(strTemp.data() + 1);
    return fieldName;
}

QString CreateOneFiles::GetFieldType(DBCFile* pDBCFile, int nField)
{
    if(nullptr == pDBCFile)
    {
        return QString();
    }

    int nFieldType = pDBCFile->GetFieldType(nField);

    if(nFieldType == 0)
    {
        return "int";
    }
    else if(nFieldType == 1)
    {
        return "float";
    }
    else if(nFieldType == 2)
    {
        return "std::string";
    }

    return QString();
}

QString CreateOneFiles::GetFieldName(DBCFile* pDBCFile, int nField)
{
    std::string strFieldName = pDBCFile->GetFieldName(nField);
    if(strFieldName.empty())
    {
        QMessageBox::information(nullptr, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("取名不对"));
        return QString();
    }

    return QString(strFieldName.c_str());
}

void CreateOneFiles::CreateEnumList()
{
    DBCFile fileDBC(0);
    if(fileDBC.OpenFromTXT(m_strTblFilePath.c_str()) == false)
    {
        return;
    }

    int nCol = fileDBC.GetFieldsNum();
    for(int i = 0; i < nCol; i++)
    {
        QString enumStr(m_strBaseName);
        enumStr += "_" + QString(fileDBC.GetFieldName(i).c_str());
        m_strEnumList.push_back(enumStr.toUpper());
    }
}