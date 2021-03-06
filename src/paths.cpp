/***********************************************************************
 *
 * Copyright (C) 2013, 2014 Graeme Gott <graeme@gottcode.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

// Need this to find the old data locations from Qt 4 when using Qt 5.
// QDesktopServices::storageLocation() is deprecated and returns a different
// path than QStandardPaths::writableLocation().
#define QT_DISABLE_DEPRECATED_BEFORE 0x000000

#include "paths.h"

#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QStringList>

//-----------------------------------------------------------------------------

QString Paths::dataPath()
{
	static QString path = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
	return path;
}

//-----------------------------------------------------------------------------

QString Paths::oldDataPath()
{
	QStringList oldpaths;
	QString oldpath;

	// Data path from Qt 4 version of 1.4
	oldpaths.append(QDesktopServices::storageLocation(QDesktopServices::DataLocation));

	// Data path from 1.0
#if defined(Q_OS_MAC)
	oldpath = QDir::homePath() + "/Library/Application Support/GottCode/FocusWriter/";
#elif defined(Q_OS_UNIX)
	oldpath = QString::fromLocal8Bit(qgetenv("XDG_DATA_HOME"));
	if (oldpath.isEmpty()) {
		oldpath = QDir::homePath() + "/.local/share";
	}
	oldpath += "/focuswriter";
#else
	oldpath = QDir::homePath() + "/Application Data/GottCode/FocusWriter/";
#endif
	if (!oldpaths.contains(oldpath)) {
		oldpaths.append(oldpath);
	}

	// Check if an old data location exists
	oldpath.clear();
	for (const QString& testpath : oldpaths) {
		if (QFile::exists(testpath)) {
			oldpath = testpath;
			break;
		}
	}

	return oldpath;
}

//-----------------------------------------------------------------------------
