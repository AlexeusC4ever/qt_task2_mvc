#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSize>
#include <QStringList>
#include "treeitem.h"
#include "treemodel.h"

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
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void closeEvent(QCloseEvent *event);

private slots:
    void open();
    void openRecentFile();
    void on_buttonStart1Doc_clicked();

    void on_buttonStartEl_clicked();

    void on_actionNew_triggered();

    void on_actionCloseWindows_triggered();

private:
    Ui::MainWindow *ui;
    QLabel *m_pLabelNumWindows;

    enum{ enMaxRecentFiles = 5 };
    int m_nWindows;
    int m_OwnWindowsCounter;
    QAction *m_apActionsRecent[enMaxRecentFiles];
    QAction *m_pActionSeparator;
    QAction *m_pActionViewFile;
    QString m_fileName;
    QStringList m_listRecentFiles;
    TreeModel* m_TreeModel;
    void loadFile(const QString &rcFileName);
    void updateRecentFileActions();
    void updateStatusBar();
    void setNumWindows(int nWindows);
    void updateAllNumWindows();
    void readXmlFile(const QString &rcFileName);
    void writeXmlFile(const QString &rcFileName);
};

#endif // MAINWINDOW_H
