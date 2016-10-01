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

#ifndef NAME_UTILS_H
#define NAME_UTILS_H

#include <QString>

QStringRef getFileName(const QString& filename);

QString getExtension(const QString& filename);

QString genCopyName(const QString& filename);

QString changeExtension(const QString& filename, const QString& new_ext);

QString getTypeExtension(const QString& format);

#endif // NAME_UTILS_H
