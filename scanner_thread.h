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
};

#endif // SCANNER_THREAD_H
