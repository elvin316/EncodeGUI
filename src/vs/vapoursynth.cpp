/****************************************************************************
 * Copyright (C) 2022 DaGoose
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/

#include <QtCore/QDir>
#include <QtCore/QCoreApplication>
#include "vapoursynth.hpp"

/// <summary>
/// Sets the VapourSynth plugins to use.
/// </summary>
QString VapourSynth::plugin(QString path) {
	return(QString("core.std.LoadPlugin(\"%1\")\n").arg(path.replace(QString("\\"), QString("\\\\"))));
}

/// <summary>
/// Includes required python nodes.
/// </summary>
QString VapourSynth::include() {
	#ifdef Q_OS_WINDOWS
	QString vsDir = QDir::toNativeSeparators(QCoreApplication::applicationDirPath() + QString("\\vs"));
	QString siteDir = QDir::toNativeSeparators(QCoreApplication::applicationDirPath() + QString("\\vs\\Lib\\site-packages"));
	QString vsRifeDir = QDir::toNativeSeparators(QCoreApplication::applicationDirPath() + QString("\\vs\\vsrife"));
	return(QString(
		"# This file was generated using EncodeGUI\n\n"
		"import sys, os\n"
		"try:\n"
		"    import site\n"
		"    site.main()\n"
		"except Exception:\n"
		"    pass\n"
		"for _p in [r'%1', r'%2', r'%3']:\n"
		"    if os.path.exists(_p) and _p not in sys.path:\n"
		"        sys.path.insert(0, _p)\n\n"
		"try:\n"
		"    from vsrife import rife as RIFE\n"
		"except ImportError:\n"
		"    try:\n"
		"        from vsrife import RIFE\n"
		"    except ImportError as _e:\n"
		"        raise ImportError(f\"Could not load RIFE module. Ensure 'vsrife' and 'torch' are installed in the EncodeGUI vs environment: {_e}\")\n\n"
		"import tempfile\n"
		"import muvsfunc as mf\n"
		"import vapoursynth as vs\n"
		"from vapoursynth import core\n\n"
	).arg(vsDir).arg(siteDir).arg(vsRifeDir));
	#endif
	#ifdef Q_OS_DARWIN
	return(QString("# This file was generated using EncodeGUI\n\nimport os\nimport muvsfunc as mf\nimport vapoursynth as vs\nfrom vapoursynth import core\n\n"));
	#endif
}

/// <summary>
/// Configures the source video for piping.
/// </summary>
/// <param name="path">The source video file path.</param>
/// <returns>String representation of the script.</returns>
QString VapourSynth::input(QString path, QString id) {
	#ifdef Q_OS_WINDOWS
	return(QString("\nclip = core.lsmas.LWLibavSource(source=\"%1\", cachefile=tempfile.gettempdir() + \"\\%2.lwi\")\n\n").arg(path).replace(QString("\\"), QString("\\\\")).arg(id));
	#endif
	#ifdef Q_OS_DARWIN
	return(QString("\nclip = core.ffms2.Source(source=\"%1\", cachefile=os.getenv(\"HOME\") + \"/Library/Caches/TemporaryItems/%2.ffindex\")\n\n").arg(path).arg(id));
	#endif
}

/// <summary>
/// Enabled scene change detection for frame interpolation.
/// </summary>
/// <param name="threshold">The sensitivity level for the scene change detection.</param>
/// <returns>String representation of the script.</returns>
QString VapourSynth::scDetect(QString threshold) {
	return(QString("clip = core.misc.SCDetect(clip, threshold=%1)\n").arg(threshold));
}

/// <summary>
/// Concludes the clip for output pipe.
/// </summary>
/// <returns>String representation of the script.</returns>
QString VapourSynth::concludeClip() {
	return(QString("clip.set_output()"));
}

/// <summary>
/// Adds a new line to the script.
/// </summary>
/// <returns>String representation of the script.</returns>
QString VapourSynth::newLine() {
	return(QString("\n"));
}

/// <summary>
/// Anti-Aliasing (EIDAA)
/// </summary>
/// <returns>String representation of the script.</returns>
QString VapourSynth::antiA() {
	return(QString("clip = mf.ediaa(clip)\n\n"));
}