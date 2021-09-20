#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>

namespace
{
    QString strippedName(const QString &rcFullFileName)
    {
        return QFileInfo(rcFullFileName).fileName();
    }
}

MainWindow::MainWindow(QWidget *parent):
        QMainWindow(parent),
        ui(new Ui::MainWindow),
        m_nWindows(1), m_OwnWindowsCounter(1)
{
    ui->setupUi(this);

    m_pLabelNumWindows = new QLabel;
    statusBar()->addWidget(m_pLabelNumWindows, 1);

    updateStatusBar();


    for(int i = 0; i < enMaxRecentFiles; ++ i)
    {
        m_apActionsRecent[i] = new QAction(this);
        m_apActionsRecent[i]->setVisible(false);
        connect(m_apActionsRecent[i], SIGNAL(triggered()),
            this, SLOT(openRecentFile()));

        ui->menu1->addAction(m_apActionsRecent[i]);
    }

    m_pActionSeparator = ui->menu1->addSeparator();
    m_pActionSeparator->setVisible(false);

    connect(ui->actionOpenFile, SIGNAL(triggered()), this, SLOT(open()));

    QAction *actionExit = new QAction(this);
    actionExit->setText(tr("Выход"));
    ui->menu1->addAction(actionExit);
    connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));

    setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->accept();

    --m_nWindows;
    updateAllNumWindows();
}

void MainWindow::readXmlFile(const QString &rcFileName)
{

    QFile file(rcFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this,
                             tr("Ошибка"),
                             tr("Ошибка чтения файла"));
        return;
    }

    QXmlStreamReader reader(file.readAll());
    QXmlStreamAttributes attributes;

    file.close();

    QStack<TreeItem*> stackEls;
    QVector<QVariant> header;
    header.emplaceBack(rcFileName);
    TreeItem *rootItem = new TreeItem(header);

    m_TreeModel = new TreeModel(ui->treeView2);
    m_TreeModel->setRoot(rootItem);
//    m_TreeModel->insertColumns(1, 1, QModelIndex());
    while(!reader.atEnd())
    {
        QVector<QVariant> sl;

        if(reader.isStartElement())
        {
            attributes = reader.attributes();

            sl << reader.name().toString();

            TreeItem *item = new TreeItem(sl, rootItem);

            for(int i = 0; i < attributes.size(); ++i)
            {
                QVector<QVariant> strAttrs;
                QXmlStreamAttribute attr = attributes[i];
                strAttrs << attr.name().toString();
                QString strAttr = "attribute(" + strAttrs[0].toString() + ")";
                strAttrs[0] = strAttr;
                strAttrs << attr.value().toString();
                TreeItem *attrItem = new TreeItem(strAttrs, item);
                item->appendChild(attrItem);
            }

            if(!stackEls.count())
            {
                m_TreeModel->addTopLevelItem(item);

            }
            else
            {
                stackEls.top()->appendChild(item);
            }
            stackEls.push(item);

            rootItem = item;
        }

        if(reader.isEndElement())
        {
            rootItem = rootItem->parent();
            stackEls.pop();
        }

        reader.readNext();
    }
    ui->treeView2->setModel(m_TreeModel);
}



void MainWindow::writeXmlFile(const QString &FileName)
{
    QFile file(FileName);

    if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QMessageBox::warning(this,
                             tr("Ошибка"),
                             tr("Ошибка записи в файла"));
        return;
    }

    QXmlStreamWriter writer(&file);

    writer.setAutoFormatting(true);
    writer.writeStartDocument();

    writer.writeStartElement("bookmark");
    writer.writeAttribute("href", "http://qt-project.org/");
    writer.writeTextElement("title", "Qt Project");
    writer.writeEndElement(); // bookmark

    writer.writeEndDocument();
}

void MainWindow::open()
{
    QString fileName =
            QFileDialog::getOpenFileName(this,
                tr("Open File"),
                "/home",
                tr("XML files (*.xml)"));

    QString extension = fileName.mid(fileName.length() - 4, 4);

    if(extension != ".xml")
    {
        QMessageBox::warning(this, tr("Ошибка"), tr("Расширение файла должно быть .xml"));
        return;
    }

    if(!fileName.isEmpty())
        loadFile(fileName);

}

void MainWindow::loadFile(const QString &rcFileName)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    readXmlFile(rcFileName);

    m_fileName = rcFileName;
    m_listRecentFiles.removeAll(m_fileName);
    m_listRecentFiles.prepend(m_fileName);
    updateRecentFileActions();
    updateStatusBar();
    QApplication::restoreOverrideCursor();
    statusBar()->showMessage(tr("File loaded"), 2000);
}

void MainWindow::openRecentFile()
{
    QAction *pAction = qobject_cast<QAction *>(sender());
    if(pAction)
        loadFile(pAction->data().toString());
}



void MainWindow::updateRecentFileActions()
{
    QMutableStringListIterator i(m_listRecentFiles);
    while(i.hasNext())
        if(!QFile::exists(i.next()))
            i.remove();

    for(int j = 0; j < enMaxRecentFiles; ++j)
    {
        if(j < m_listRecentFiles.count())
        {
            QString text = QString("&%1. %2")
                    .arg(j + 1)
                    .arg(strippedName(m_listRecentFiles[j]));
            m_apActionsRecent[j]->setText(text);
            m_apActionsRecent[j]->setData(m_listRecentFiles[j]);
            m_apActionsRecent[j]->setVisible(true);
        }
        else
        {
            m_apActionsRecent[j]->setVisible(false);
            m_pActionSeparator->setVisible(!m_listRecentFiles.empty());
        }
    }
}

void MainWindow::updateStatusBar()
{
    QString strLabel;
    strLabel = QString(tr("%1 windows")).arg(m_nWindows);
    m_pLabelNumWindows->setText(strLabel);
}

void MainWindow::setNumWindows(int nWindows)
{
    m_nWindows = nWindows;
    updateStatusBar();
}

void MainWindow::updateAllNumWindows()
{
    foreach(QWidget *pWidget, QApplication::topLevelWidgets())
    {
        if(MainWindow *pMainWindow =
                qobject_cast<MainWindow *>(pWidget))
            pMainWindow->setNumWindows(m_nWindows);
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_buttonStart1Doc_clicked()
{
    if(m_fileName.isEmpty())
    {

    }
    else
        return;
}


void MainWindow::on_buttonStartEl_clicked()
{
    if(m_fileName.isEmpty()) return;
    QModelIndex index = ui->treeView2->currentIndex();
    QVector<QVariant> name;
    name.emplace_back("column1");

    TreeItem* item = m_TreeModel->getItem(index);
    TreeItem* childItem = new TreeItem(name, item);
    item->appendChild(childItem);

//    ui->treeView2->expand(index);
}


void MainWindow::on_actionNew_triggered()
{
    MainWindow *pMainWindow = new MainWindow;
    QString strWindow = QString(tr("Window %1").arg(m_OwnWindowsCounter));
    ++m_OwnWindowsCounter;
    QAction *newWindow = new QAction(this);
    newWindow->setText(strWindow);
    ui->menu_2->addAction(newWindow);

    connect(newWindow, SIGNAL(triggered()), pMainWindow, SLOT(raise()));

    ++m_nWindows;
    updateAllNumWindows();
    pMainWindow->show();
}


void MainWindow::on_actionCloseWindows_triggered()
{
    qApp->closeAllWindows();
}

