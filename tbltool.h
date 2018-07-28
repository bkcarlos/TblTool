#ifndef TBLTOOL_H
#define TBLTOOL_H

#include <QMainWindow>

#include <QPushButton>
#include <QLineEdit>
#include <QListWidget>

namespace Ui {
    class TblTool;
}

class TblTool : public QMainWindow
{
    Q_OBJECT

public:
    explicit TblTool(QWidget *parent = 0);
    ~TblTool();

private:
    bool Init();
    void AddConnects();
    void InitShow();

    void OnClickSelectFiles();
    void OnClickSelectPath();
    void OnClickCreateAllFiles();

    void OnClickListWidgetItem();

    void OnClickListWidgetItemDel();
    void OnClickListWidgetItemCre();

    void CreateOneFile(QString& filePath);

private:
    Ui::TblTool *ui;

    QPushButton* m_pButtonSelectFiles;
    QPushButton* m_pButtonSelectPath;
    QPushButton* m_pButtonCreate;
    QLineEdit*   m_pLineEditPath;
    QListWidget* m_pListWidgetFiles;
    QString      m_strSelectPath;
};

#endif // TBLTOOL_H
