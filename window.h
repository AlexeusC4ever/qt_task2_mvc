#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QSize>
#include <QStringList>
#include "treeitem.h"
#include "treemodel.h"

class QLabel;
//class QScrollArea;
class QAction;

QT_BEGIN_NAMESPACE
namespace Ui { class Window; }
QT_END_NAMESPACE

class Window : public QMainWindow
{
    Q_OBJECT

public:
    Window(const QString &fileName, QWidget *parent);
    ~Window();

protected:
//    virtual void closeEvent(QCloseEvent *pEvent);

private slots:
    void on_buttonStart1Doc_clicked();

    void on_buttonStartEl_clicked();

private:
    Ui::Window *ui;
    QLabel *m_pLabelFileName;

    enum{ enMaxRecentFiles = 5 };

//    QAction *m_pActionViewFile;
    QString m_fileName;
    TreeModel* m_TreeModel;
    void updateStatusBar();
    void loadFile(const QString &rcFileName);
    void readXmlFile(const QString &rcFileName);
    void writeXmlFile(const QString &rcFileName);
};

#endif // WINDOW_H
