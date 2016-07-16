#ifndef NAME_UTILS_H
#define NAME_UTILS_H

#include <QString>

QStringRef getFileName(const QString& filename);

QString getExtension(const QString& filename);

QString genCopyName(const QString& filename);

QString changeExtension(const QString& filename, const QString& new_ext);

QString getTypeExtension(const QString& format);

#endif // NAME_UTILS_H
