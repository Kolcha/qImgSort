#include "name_utils.h"

#include <QFile>

QStringRef getFileName(const QString& filename)
{
  return filename.midRef(filename.lastIndexOf('/') + 1);
}

int findExtension(const QString& filename)
{
  QStringRef name = getFileName(filename);
  int last_dot = name.lastIndexOf('.');
  if (last_dot == -1 || last_dot == 0) return -1;
  if (name.length() - last_dot - 1 > 4) return -1;
  return filename.lastIndexOf('.') + 1;
}

QString getBaseName(const QString& filename)
{
  int name_start = filename.lastIndexOf('/') + 1;
  int ext_start = findExtension(filename);
  if (ext_start == -1) return filename.mid(name_start);
  return filename.mid(name_start, ext_start - name_start - 1);
}

QString getExtension(const QString& filename)
{
  int ext_start = findExtension(filename);
  if (ext_start == -1) return QString();
  return filename.mid(ext_start);
}

QString genCopyName(const QString& filename)
{
  QString new_name_mask("%1 (%2)");

  int ext_start = findExtension(filename);
  if (ext_start != -1) new_name_mask += "." + filename.mid(ext_start);

  QString src_part = filename.mid(0, ext_start != -1 ? ext_start - 1 : ext_start);

  QString new_name;
  int n = 1;
  do {
    new_name = new_name_mask.arg(src_part).arg(n++);
  } while (QFile::exists(new_name));

  return new_name;
}

QString changeExtension(const QString& filename, const QString& new_ext)
{
  if (new_ext.isEmpty()) return filename;
  int ext_start = findExtension(filename);
  if (ext_start == -1) return filename + "." + new_ext;
  return filename.mid(0, ext_start) + new_ext;
}

QString getTypeExtension(const QString& format)
{
  if (format.toLower() == "bmp") return QString("bmp");
  if (format.toLower() == "gif") return QString("gif");
  if (format.toLower() == "jpeg") return QString("jpg");
  if (format.toLower() == "png") return QString("png");
  if (format.toLower() == "pbm") return QString("pbm");
  if (format.toLower() == "pgm") return QString("pgm");
  if (format.toLower() == "ppm") return QString("ppm");
  if (format.toLower() == "xbm") return QString("xbm");
  if (format.toLower() == "xpm") return QString("xpm");
  if (format.toLower() == "svg") return QString("svg");

  if (format.toLower() == "dds") return QString("dds");
  if (format.toLower() == "icns") return QString("icns");
  if (format.toLower() == "jp2") return QString("jp2");
  if (format.toLower() == "mng") return QString("mng");
  if (format.toLower() == "tga") return QString("tga");
  if (format.toLower() == "tiff") return QString("tiff");
  if (format.toLower() == "wbmp") return QString("wbmp");
  if (format.toLower() == "webp") return QString("webp");

  if (format.toLower() == "ico") return QString("ico");

  return QString();
}
