#include "MyWidget.h"
#include <QApplication>
#include "TabDlg.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TabDlg dlg;
    dlg.show();

    return a.exec();
}
