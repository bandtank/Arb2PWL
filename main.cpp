#include <QApplication>
#include "Arb2PWLWidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Arb2PWLWidget arb2PWLWidget;

    arb2PWLWidget.show();
    return app.exec();
}
