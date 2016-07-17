#include "main_window.h"

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  a.setApplicationName("qImgSort");
  a.setApplicationVersion("2.1.1");
  a.setOrganizationName("Nick Korotysh");
  a.setOrganizationDomain("qimgsort.sourceforge.net");

  // install app translators
  QTranslator app_translator;
  QTranslator qt_translator;
  QString langs_path = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
  QStringList ui_languages = QLocale::system().uiLanguages();
  foreach (QString locale, ui_languages) {
    locale = QLocale(locale).name();
    if (app_translator.load(QLatin1String("qimgsort_") + locale, langs_path)) {
      a.installTranslator(&app_translator);
      if (qt_translator.load(QLatin1String("qt_") + locale, langs_path)) {
        a.installTranslator(&qt_translator);
      }
      break;
    } else if (locale == QLatin1String("C") /* overrideLanguage == "English" */) {
      // use built-in
      break;
    } else if (locale.startsWith(QLatin1String("en")) /* "English" is built-in */) {
      // use built-in
      break;
    } else if (locale.contains("ua", Qt::CaseInsensitive)) /* Ukrainian, use russian */ {
      if (app_translator.load(QLatin1String("qimgsort_ru"), langs_path)) {
        a.installTranslator(&app_translator);
      }
      if (qt_translator.load(QLatin1String("qt_ru"), langs_path)) {
        a.installTranslator(&qt_translator);
      }
      break;
    }
  }

  MainWindow w;
  w.setWindowIcon(QIcon(":/icons/app_icon.png"));
  w.setWindowTitle(QString("%1 %2").arg(a.applicationName(), a.applicationVersion()));
  w.show();

  return a.exec();
}
