#include "./build/ui_window.h"
#include "window.h"
#include <QtWidgets>

namespace
{
    QString strippedName(const QString &rcFullFileName)
    {
        return QFileInfo(rcFullFileName).fileName();
    }
}



Window::Window(const QString &fileName, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Window)
{
    ui->setupUi(this);

    m_pLabelFileName = new QLabel;
    statusBar()->addWidget(m_pLabelFileName, 1);

    loadFile(fileName);
}

void Window::readXmlFile(const QString &rcFileName)
{

    QFile file(rcFileName);
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
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
//    m_TreeModel->insertColumns(1, 1);
    m_TreeModel->insertColumn(1);
    while(!reader.atEnd())
    {
        QVector<QVariant> sl;

//        if(reader.tokenType() == QXmlStreamReader::Characters)
//        {
//            sl << reader.readElementText();
//            qDebug() << sl;
//            stackEls.top()->setData(1, sl[0]);
//            qDebug() <<  stackEls.top()->data(1);
//        }

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
//    QModelIndex index = m_TreeModel->index(0, 0, QModelIndex());
    ui->treeView2->setModel(m_TreeModel);
}



void Window::writeXmlFile(const QString &FileName)
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

void Window::loadFile(const QString &rcFileName)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    readXmlFile(rcFileName);

    m_fileName = rcFileName;
    updateStatusBar();
    QApplication::restoreOverrideCursor();
    statusBar()->showMessage(tr("File loaded"), 2000);
}

void Window::updateStatusBar()
{
    QString strLabel;

    m_pLabelFileName->setText(m_fileName);

}

Window::~Window()
{
    delete ui;
}


void Window::on_buttonStart1Doc_clicked()
{

}


void Window::on_buttonStartEl_clicked()
{
    if(m_fileName.isEmpty()) return;
    QModelIndex index = ui->treeView2->currentIndex();
    QVector<QVariant> name;
    name.emplace_back("column1");

    TreeItem* item = m_TreeModel->getItem(index);
    TreeItem* childItem = new TreeItem(name, item);
    item->appendChild(childItem);

//    m_TreeModel->insertRow(item->childCount(), index);
    ui->treeView2->expand(index);
}

