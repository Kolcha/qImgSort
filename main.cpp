#include "main_window.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  a.setApplicationName("ImgSort");
  a.setApplicationVersion("2.0.3");
  a.setOrganizationName("Nick Korotysh");
  a.setOrganizationDomain("qimgsort.sourceforge.net");

  MainWindow w;
  w.setWindowTitle(QString("%1 %2").arg(a.applicationName(), a.applicationVersion()));
  w.show();

  return a.exec();
}
