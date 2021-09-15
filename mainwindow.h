#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSize>
#include <QStringList>
#include "treeitem.h"
#include "treemodel.h"
#include "window.h"

class QLabel;
//class QScrollArea;
class QAction;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
//    virtual void closeEvent(QCloseEvent *pEvent);

private slots:
    void open();
    void openRecentFile();

    void on_buttonStart1Doc_clicked();

    void on_buttonStartEl_clicked();

private:
    Ui::MainWindow *ui;
    QLabel *m_pLabelFileName;

    enum{ enMaxRecentFiles = 5 };

    QVector<Window*> m_pWindows;
    QAction *m_apActionsRecent[enMaxRecentFiles];
    QAction *m_pActionSeparator;
    QAction *m_pActionViewFile;
    QString m_fileName;
    QStringList m_listRecentFiles;
    TreeModel* m_TreeModel;
    void loadFile(const QString &rcFileName);
    void updateRecentFileActions();
    void updateStatusBar();
    void readXmlFile(const QString &rcFileName);
    void writeXmlFile(const QString &rcFileName);
};

#endif // MAINWINDOW_H
