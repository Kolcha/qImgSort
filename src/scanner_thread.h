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

#ifndef SCANNER_THREAD_H
#define SCANNER_THREAD_H

#include <QThread>

class ScannerThread : public QThread
{
  Q_OBJECT
public:
  struct ScanStat {
    int files_found;
    int files_procesed;

    ScanStat() :
      files_found(0),
      files_procesed(0)
    {}

    void clear() {
      files_found = 0;
      files_procesed = 0;
    }
  };

  explicit ScannerThread(QObject* parent = nullptr);

signals:
  void logMessage(const QString& msg);
  void updateStat(const ScanStat& st);

public slots:
  void setSourceDir(const QString& path);
  void setTargetDir(const QString& path);

  void setDeleteProcessed(bool enable);
  void setFixExtensions(bool enable);

  void stop();

protected:
  void run() override;

private:
  void scanPath(const QString& path);
  void processFile(const QString& filename);

  QString src_path_;
  QString dst_path_;

  bool delete_processed_;
  bool fix_extensions_;

  volatile bool stopped_;

  ScanStat st_;

  QString f_info_;
  QString f_warn_;
  QString f_err_;
};

#endif // SCANNER_THREAD_H
