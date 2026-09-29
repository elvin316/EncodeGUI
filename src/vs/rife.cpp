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

#include "vapoursynth.hpp"

#ifdef Q_OS_WINDOWS
/// <summary>
/// Configures RIFE in its CUDA implementation (HolyWu vs-rife).
/// </summary>
/// <param name="id">Sets the GPU id index.</param>
/// <param name="model">Model string (e.g. 4.26, 4.25, 4.6, 4.0).</param>
/// <param name="num">Numerator for output FPS.</param>
/// <param name="den">Denominator for output FPS.</param>
/// <param name="scale">Scale factor for optical flow processing.</param>
/// <param name="sc">Scene change detection (True/False).</param>
/// <param name="trt">TensorRT acceleration (True/False).</param>
/// <returns>String representation of the argument.</returns>
QString VapourSynth::rifeCuda(int id, QString model, int num, int den, double scale, QString sc, QString trt) {
	QString trtOpt;
	if (trt == QString("True") && model != QString("4.0") && model != QString("4.1"))
		trtOpt = QString(", trt=True, trt_cache_dir=_trt_dir");
	return(QString("clip = RIFE(clip, device_index=%1, model='%2', auto_download=True, fps_num=%3, fps_den=%4, scale=%5, sc=%6%7)\n\n")
		.arg(id).arg(model).arg(num).arg(den).arg(scale).arg(sc).arg(trtOpt));
}
#endif

/// <summary>
/// Configures RIFE in it's NCNN implementation.
/// </summary>
/// <param name="model">Sets the model index to use.</param>
/// <param name="id">Sets the GPU id index to use.</param>
/// <param name="thread">Sets the number of GPU threads to use.</param>
/// <param name="tta">Enables test time augmentation for better interpolation quality.</param>
/// <param name="uhd">Enables or disabled UHD mode for better frame rendering.</param>
/// <param name="sc">Enables or disables scene change detection.</param>
/// <param name="skip">Skip interpolating static frames (requires VMAF plugin).</param>
/// <param name="skipThreshold">PSNR threshold for static frames.</param>
/// <param name="modelPath">Custom model path to load model from.</param>
/// <returns>String representation of the argument.</returns>
QString VapourSynth::rifeNcnn(int model, int id, int thread, QString tta, QString uhd, QString sc, bool skip, double skipThreshold, const QString &modelPath) {
	QString extras;
	if (skip)
		extras += QString(", skip=True, skip_threshold=%1").arg(skipThreshold);
	if (!modelPath.isEmpty())
		extras += QString(", model_path=r\"%1\"").arg(QString(modelPath).replace(QString("\\"), QString("/")));
	return(QString("clip = core.rife.RIFE(clip, model=%1, gpu_id=%2, gpu_thread=%3, tta=%4, uhd=%5, sc=%6%7)\n\n").arg(model).arg(id).arg(thread).arg(tta).arg(uhd).arg(sc).arg(extras));
}

/// <summary>
/// Configures RIFE in it's NCNN implementation with custom output FPS.
/// </summary>
/// <param name="model">Sets the model index to use.</param>
/// <param name="id">Sets the GPU id index to use.</param>
/// <param name="thread">Sets the number of GPU threads to use.</param>
/// <param name="num">Numerator for output FPS.</param>
/// <param name="den">Denominator for output FPS.</param>
/// <param name="tta">Enables test time augmentation for better interpolation quality.</param>
/// <param name="uhd">Enables or disabled UHD mode for better frame rendering.</param>
/// <param name="sc">Enables or disables scene change detection.</param>
/// <param name="skip">Skip interpolating static frames (requires VMAF plugin).</param>
/// <param name="skipThreshold">PSNR threshold for static frames.</param>
/// <param name="modelPath">Custom model path to load model from.</param>
/// <returns>String representation of the argument.</returns>
QString VapourSynth::rifeNcnnNew(int model, int id, int thread, int num, int den, QString tta, QString uhd, QString sc, bool skip, double skipThreshold, const QString &modelPath) {
	QString extras;
	if (skip)
		extras += QString(", skip=True, skip_threshold=%1").arg(skipThreshold);
	if (!modelPath.isEmpty())
		extras += QString(", model_path=r\"%1\"").arg(QString(modelPath).replace(QString("\\"), QString("/")));
	return(QString("clip = core.rife.RIFE(clip, model=%1, gpu_id=%2, gpu_thread=%3, fps_num=%4, fps_den=%5, tta=%6, uhd=%7, sc=%8%9)\n\n").arg(model).arg(id).arg(thread).arg(num).arg(den).arg(tta).arg(uhd).arg(sc).arg(extras));
}
