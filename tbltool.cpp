#include "tbltool.h"
#include "ui_tbltool.h"

#include <QString>
#include <QFileDialog>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QStandardPaths>

#include "CreateOneFiles.h"

const QString strShowType("background-color: rgba(94,133,21,0);\
                          border: 1px groove gray;\
                          border-top-left-radius:6px;\
                          border-top-right-radius:6px;\
                          border-bottom-left-radius:6px;\
                          border-bottom-right-radius:6px;\
                          padding-left:0px;\
                          padding-right:0px;");

const QString strMenuShowType("background-color: rgba(0,200,200,200);\
                          border: 1px groove gray;\
                          border-top-left-radius:0px;\
                          border-top-right-radius:0px;\
                          border-bottom-left-radius:0px;\
                          border-bottom-right-radius:0px;\
                          padding-left:1px;\
                          padding-right:1px;");

TblTool::TblTool(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TblTool)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/icon/logo.png"));
    resize(580, 500);

    if (!Init())
    {
        return;
    }

    AddConnects();
    InitShow();
}

TblTool::~TblTool()
{
    if (nullptr != m_pButtonSelectFiles)
    {
        delete m_pButtonSelectFiles;
        m_pButtonSelectFiles = nullptr;
    }

    if (nullptr != m_pButtonSelectPath)
    {
        delete m_pButtonSelectPath;
        m_pButtonSelectPath = nullptr;
    }

    if (nullptr != m_pButtonCreate)
    {
        delete m_pButtonCreate;
        m_pButtonCreate = nullptr;
    }

    if (nullptr != m_pLineEditPath)
    {
        delete m_pLineEditPath;
        m_pLineEditPath = nullptr;
    }

    if (nullptr != m_pListWidgetFiles)
    {
        delete m_pListWidgetFiles;
        m_pListWidgetFiles = nullptr;
    }

    delete ui;
}

bool TblTool::Init()
{
    m_pButtonSelectFiles = new QPushButton(this);
    if (nullptr == m_pButtonSelectFiles)
    {
        return false;
    }

    m_pButtonSelectPath = new QPushButton(this);
    if (nullptr == m_pButtonSelectPath)
    {
        return false;
    }

    m_pButtonCreate = new QPushButton(this);
    if (nullptr == m_pButtonCreate)
    {
        return false;
    }
    m_pLineEditPath = new QLineEdit(this);
    if (nullptr == m_pLineEditPath)
    {
        return false;
    }

    m_pListWidgetFiles = new QListWidget(this);
    if (nullptr == m_pListWidgetFiles)
    {
        return false;
    }

    return true;
}

void TblTool::AddConnects()
{
    connect(m_pButtonSelectFiles, &QPushButton::clicked, this, &TblTool::OnClickSelectFiles);
    connect(m_pListWidgetFiles, &QListWidget::clicked, this, &TblTool::OnClickListWidgetItem);
    connect(m_pButtonCreate, &QPushButton::clicked, this, &TblTool::OnClickCreateAllFiles);
    connect(m_pButtonSelectPath, &QPushButton::clicked, this, &TblTool::OnClickSelectPath);
}

void TblTool::InitShow()
{
    m_pButtonSelectFiles->setText(QString::fromLocal8Bit("选择文件(*.txt)"));
    m_pButtonSelectFiles->setGeometry(rect().x() + 20, rect().y() + 20, 100, 40);
    m_pButtonSelectFiles->setStyleSheet(strShowType);

    m_pButtonSelectPath->setText(QString::fromLocal8Bit("文件存储路径"));
    m_pButtonSelectPath->setGeometry(rect().x() + 130, rect().y() + 20, 100, 40);
    m_pButtonSelectPath->setStyleSheet(strShowType);

    m_pLineEditPath->setPlaceholderText(QString::fromLocal8Bit("文件存储路径"));
    m_pLineEditPath->setGeometry(rect().x() + 240, rect().y() + 20, 200, 40);
    m_pLineEditPath->setStyleSheet(strShowType);

    m_pButtonCreate->setText(QString::fromLocal8Bit("创建文件"));
    m_pButtonCreate->setGeometry(rect().x() + 450, rect().y() + 20, 100, 40);
    m_pButtonCreate->setStyleSheet(strShowType);

    m_pListWidgetFiles->setGeometry(rect().x() + 20, rect().y() + 80, 540, 400);
    m_pListWidgetFiles->setStyleSheet(strShowType);
}

void TblTool::OnClickSelectFiles()
{
    QFileDialog* pFileDiglogSelectFiles = new QFileDialog(this);
    pFileDiglogSelectFiles->setWindowTitle(QString::fromLocal8Bit("请选择需要生成的文件"));
    pFileDiglogSelectFiles->setDirectory(".");
    pFileDiglogSelectFiles->setViewMode(QFileDialog::List);
    pFileDiglogSelectFiles->setNameFilter("txt files(*.txt)");
    pFileDiglogSelectFiles->setFileMode(QFileDialog::ExistingFiles);

    QStringList fileNameList;
    if (pFileDiglogSelectFiles->exec() == QFileDialog::Accepted)
    {
        fileNameList = pFileDiglogSelectFiles->selectedFiles();
    }

    m_pListWidgetFiles->clear();
    m_pListWidgetFiles->addItems(fileNameList);
}

void TblTool::OnClickSelectPath()
{
    QString strFilePath = QFileDialog::getExistingDirectory(this, QString::fromLocal8Bit("请选择生成文件保存路径..."), QString::fromLocal8Bit("./"));
    if (!strFilePath.isEmpty())
    {
        m_strSelectPath = strFilePath;
        m_pLineEditPath->setText(strFilePath);
    }
}

void TblTool::OnClickCreateAllFiles()
{
    if (m_pListWidgetFiles->count() == 0)
    {
        if (QMessageBox::Ok == QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请选择需要生成的文件..."), QMessageBox::Ok | QMessageBox::No, QMessageBox::Ok))
        {
            OnClickSelectFiles();
        }
    }

    for (int i = 0; i < m_pListWidgetFiles->count(); ++i)
    {
        QListWidgetItem* pListWidgetItem = m_pListWidgetFiles->item(i);
        QString filePath = pListWidgetItem->text();
        CreateOneFile(filePath);
    }

    QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("文件已经全部生成..."), QMessageBox::Ok, QMessageBox::Ok);
}

void TblTool::OnClickListWidgetItem()
{
    QMenu* pItemMenu = new QMenu(m_pListWidgetFiles);
    pItemMenu->setStyleSheet(strMenuShowType);

    QAction* pDelAction = new QAction(QString::fromLocal8Bit("删除"), m_pListWidgetFiles);
    connect(pDelAction, &QAction::triggered, this, &TblTool::OnClickListWidgetItemDel);

    QAction* pCreAction = new QAction(QString::fromLocal8Bit("生成"), m_pListWidgetFiles);
    connect(pCreAction, &QAction::triggered, this, &TblTool::OnClickListWidgetItemCre);

    pItemMenu->addAction(pDelAction);
    pItemMenu->addAction(pDelAction);
    pItemMenu->addAction(pCreAction);

    pItemMenu->exec(QCursor::pos());

    delete pItemMenu;
    pItemMenu = nullptr;
    delete pDelAction;
    pDelAction = nullptr;
    delete pCreAction;
    pCreAction = nullptr;
}

void TblTool::OnClickListWidgetItemDel()
{
    int nCurRow = m_pListWidgetFiles->currentRow();
    m_pListWidgetFiles->takeItem(nCurRow);
}

void TblTool::OnClickListWidgetItemCre()
{
    int nCurRow = m_pListWidgetFiles->currentRow();
    QListWidgetItem* pListWidgetItem = m_pListWidgetFiles->item(nCurRow);
    QString filePath = pListWidgetItem->text();
    CreateOneFile(filePath);
}

void TblTool::CreateOneFile(QString& filePath)
{
    QString fileSavePath = m_pLineEditPath->text();
    if (fileSavePath.isEmpty())
    {
        fileSavePath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    }

    CreateOneFiles* pOneFile = new CreateOneFiles(filePath, fileSavePath);
    pOneFile->CreateHead();
    pOneFile->CreateSource();
    delete pOneFile;
}