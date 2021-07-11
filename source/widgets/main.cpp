#include "mainwindow.h"
#include <QApplication>
#include <QPixmap>
#include <Windows.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (argc > 1)
    {
        if (!strcmp("b", argv[1]))
        {
            g_DBG_Play_As_White = false;
        }
    }

    if (argc > 2)
    {
        if (!strcmp("automatch", argv[2]))
        {
            g_DBG_Automatch = true;
        }
    }

    MainWindow w;
	w.show();

    return a.exec();
}
