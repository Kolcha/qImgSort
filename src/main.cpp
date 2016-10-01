/*
    qImgSort - sort image files by resolution
    Copyright (C) 2011-2016  Nick Korotysh <nick.korotysh@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "main_window.h"

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  a.setApplicationName("qImgSort");
  a.setApplicationVersion("2.1.3");
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
