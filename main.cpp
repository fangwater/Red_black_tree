#include "red_black_tree.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    red_black_tree w;
    w.show();

    return a.exec();
}
