#include "scanner_thread.h"

#include <QDir>
#include <QFile>
#include <QBuffer>
#include <QImageReader>

#include "name_utils.h"

ScannerThread::ScannerThread(QObject* parent) : QThread(parent)
{
  qRegisterMetaType<ScanStat>("ScanStat");

  delete_processed_ = true;
  fix_extensions_ = true;
  stopped_ = false;

  f_info_ = "<span style=\"color:#4169E1;\">%1</span>";
  f_warn_ = "<span style=\"color:#BA55D3;\">%1</span>";
  f_err_  = "<span style=\"color:#DC143C;\">%1</span>";
}

void ScannerThread::setSourceDir(const QString& path)
{
  src_path_ = path;
}

void ScannerThread::setTargetDir(const QString& path)
{
  dst_path_ = path;
}

void ScannerThread::setDeleteProcessed(bool enable)
{
  delete_processed_ = enable;
}

void ScannerThread::setFixExtensions(bool enable)
{
  fix_extensions_ = enable;
}

void ScannerThread::stop()
{
  stopped_ = true;
}

void ScannerThread::run()
{
  emit logMessage(f_info_.arg(tr("scan started:")) + " " + QDir::toNativeSeparators(src_path_));

  stopped_ = false;
  st_.clear();
  emit updateStat(st_);

  scanPath(src_path_);

  if (stopped_) {
    emit logMessage(f_warn_.arg(tr("scan cancelled")));
  } else {
    emit logMessage(f_info_.arg(tr("scan finished")));
  }
}

void ScannerThread::scanPath(const QString& path)
{
  if (stopped_) return;

  QDir cur_dir(path);
  QFileInfoList items = cur_dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Hidden);

  for (auto& item : items) {
    if (item.isDir()) {
      scanPath(item.absoluteFilePath());
      continue;
    }

    if (item.isFile()) {
      ++st_.files_found;
      processFile(item.absoluteFilePath());
      continue;
    }
  }

  // only empty directory will removed
  if (delete_processed_) cur_dir.rmdir(path);
}

void ScannerThread::processFile(const QString& filename)
{
  if (stopped_) return;

  QFile file_device(filename);
  if (!file_device.open(QIODevice::ReadOnly)) return;

  QByteArray file_data = file_device.peek(8192);

  QBuffer buffer_device(&file_data);

  QImageReader image_reader(&buffer_device);

  QString res_dir_name;

  if (!image_reader.canRead()) {
    res_dir_name = tr("unsupported");
    emit logMessage(f_warn_.arg(tr("unsupported file:")) + " " + QDir::toNativeSeparators(filename));
  } else {
    QSize img_res = image_reader.size();
    if (!img_res.isValid()) {
      file_data = file_device.readAll();
      buffer_device.close();
      buffer_device.setBuffer(&file_data);
      image_reader.setDevice(&buffer_device);
      img_res = image_reader.size();
    }

    if (!img_res.isValid()) img_res = image_reader.read().size();

    if (img_res.isNull()) {
      res_dir_name = tr("unsupported");
      emit logMessage(f_warn_.arg(tr("unsupported file:")) + " " + QDir::toNativeSeparators(filename));
    } else {
      ++st_.files_procesed;
      QString res_dir_mask("%1x%2");
      res_dir_name = res_dir_mask.arg(img_res.width()).arg(img_res.height());
    }
  }

  file_device.close();

  QDir dst_dir(dst_path_);
  if (!dst_dir.exists(res_dir_name)) dst_dir.mkdir(res_dir_name);

  QString dst_filename = dst_dir.absoluteFilePath(res_dir_name) + "/" + getFileName(filename).toString();

  if (!image_reader.format().isEmpty()) {
    QString type_ext = getTypeExtension(image_reader.format());
    if (getExtension(dst_filename).toLower() != type_ext.toLower()) {
      if (fix_extensions_) {
        dst_filename = changeExtension(dst_filename, type_ext);
        emit logMessage(f_info_.arg(tr("incorrect extension fixed:")) + " " + QDir::toNativeSeparators(filename) + " -> " + type_ext);
      } else {
        emit logMessage(f_err_.arg(tr("incorrect extension detected:")) + " " + QDir::toNativeSeparators(filename) + " -> " + type_ext);
      }
    }
  }

  if (QFile::exists(dst_filename)) dst_filename = genCopyName(dst_filename);

  if (delete_processed_) {
    if (!QFile::rename(filename, dst_filename)) emit logMessage(f_err_.arg(tr("file not moved:")) + " " + QDir::toNativeSeparators(filename));
  } else {
    if (!QFile::copy(filename, dst_filename)) emit logMessage(f_err_.arg(tr("file not copied:")) + " " + QDir::toNativeSeparators(filename));
  }

  emit updateStat(st_);
}
