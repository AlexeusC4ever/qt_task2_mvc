#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>
#include <QMessageBox>

#include <QtWidgets>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator translator;
    bool bTranslationLoaded = false;
    const char *aszTranslationDirs[] = {
        ".",
        "./qt_task2_mvc",
        "./../translations",
        ":/translations",
        0
    };

    for(
        const char **ppszDir = aszTranslationDirs;
        *ppszDir != 0; ++ppszDir
    )
    {
        bTranslationLoaded = translator.load(
        QLocale::system(), "qt_mvc", "_", *ppszDir);
        if(bTranslationLoaded) break;
    }

    if(bTranslationLoaded)
        app.installTranslator(&translator);
    else
        QMessageBox::warning(0, "Translation", "Not loaded");

    MainWindow w;
    w.show();



    return app.exec();
}
