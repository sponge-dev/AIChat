#include <QApplication>
#include "AIChatApp.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    AIChatApp window;
    window.show();
    return app.exec();
}
