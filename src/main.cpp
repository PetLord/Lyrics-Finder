#include "LyricsFinderUI.h"
#include <QtWidgets/QApplication>
#include <fstream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LyricsFinderUI w;
    w.show();
    return a.exec();
}
