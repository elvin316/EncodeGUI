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

#include "encodegui.hpp"

void EncodeGUI::hideInterpGpu() {
    if (CHECKED(_ui->UseGPUCB)) {
        SET_ENABLED(_ui->GPUInterpDD);
        SET_ENABLED(_ui->GPUThreadDD);
        SET_ENABLED(_ui->GPUThreadInterpLabel);
    }
    else {
        SET_DISABLED(_ui->GPUInterpDD);
        SET_DISABLED(_ui->GPUThreadDD);
        SET_DISABLED(_ui->GPUThreadInterpLabel);
    }
}

#include <QtCore/QDir>
#include <QtCore/QFileInfoList>

void EncodeGUI::refreshRifeModels() {
    // 1. Refresh Vulkan / NCNN models from vs/plugins/models
    QString ncnnModelsDir = QDir::toNativeSeparators(QCoreApplication::applicationDirPath() +
#ifdef Q_OS_WINDOWS
        QString("\\vs\\plugins\\models")
#else
        QString("/vs/plugins/models")
#endif
    );

    QDir ncnnDir(ncnnModelsDir);
    int currentVkModel = _ui->RIFEModelVKDD->currentData().isValid() ? _ui->RIFEModelVKDD->currentData().toInt() : -1;
    _ui->RIFEModelVKDD->blockSignals(true);
    _ui->RIFEModelVKDD->clear();

    struct NcnnModelDef {
        int id;
        const char *name;
        const char *dir;
    };

    static const NcnnModelDef kAllNcnnModels[] = {
        { 0,  "rife",                        "rife" },
        { 1,  "rife-HD",                     "rife-HD" },
        { 2,  "rife-UHD",                    "rife-UHD" },
        { 3,  "rife-anime",                  "rife-anime" },
        { 4,  "rife-v2",                     "rife-v2" },
        { 5,  "rife-v2.3",                   "rife-v2.3" },
        { 6,  "rife-v2.4",                   "rife-v2.4" },
        { 7,  "rife-v3.0",                   "rife-v3.0" },
        { 8,  "rife-v3.1",                   "rife-v3.1" },
        { 9,  "rife-v3.9 (fast)",            "rife-v3.9" },
        { 10, "rife-v3.9 (ensemble)",        "rife-v3.9_ensembleTrue" },
        { 11, "rife-v4 (fast)",              "rife-v4" },
        { 12, "rife-v4 (ensemble)",          "rife-v4_ensembleTrue" },
        { 13, "rife-v4.1 (fast)",            "rife-v4.1" },
        { 14, "rife-v4.1 (ensemble)",        "rife-v4.1_ensembleTrue" },
        { 15, "rife-v4.2 (fast)",            "rife-v4.2" },
        { 16, "rife-v4.2 (ensemble)",        "rife-v4.2_ensembleTrue" },
        { 17, "rife-v4.3 (fast)",            "rife-v4.3" },
        { 18, "rife-v4.3 (ensemble)",        "rife-v4.3_ensembleTrue" },
        { 19, "rife-v4.4 (fast)",            "rife-v4.4" },
        { 20, "rife-v4.4 (ensemble)",        "rife-v4.4_ensembleTrue" },
        { 21, "rife-v4.5",                   "rife-v4.5" },
        { 22, "rife-v4.5 (ensemble)",        "rife-v4.5_ensembleTrue" },
        { 23, "rife-v4.6",                   "rife-v4.6" },
        { 24, "rife-v4.6 (ensemble)",        "rife-v4.6_ensembleTrue" },
        { 25, "rife-v4.7",                   "rife-v4.7" },
        { 26, "rife-v4.7 (ensemble)",        "rife-v4.7_ensembleTrue" },
        { 27, "rife-v4.8",                   "rife-v4.8" },
        { 28, "rife-v4.8 (ensemble)",        "rife-v4.8_ensembleTrue" },
        { 29, "rife-v4.9",                   "rife-v4.9" },
        { 30, "rife-v4.9 (ensemble)",        "rife-v4.9_ensembleTrue" },
        { 31, "rife-v4.10",                  "rife-v4.10" },
        { 32, "rife-v4.10 (ensemble)",       "rife-v4.10_ensembleTrue" },
        { 33, "rife-v4.11",                  "rife-v4.11" },
        { 34, "rife-v4.11 (ensemble)",       "rife-v4.11_ensembleTrue" },
        { 35, "rife-v4.12",                  "rife-v4.12" },
        { 36, "rife-v4.12 (ensemble)",       "rife-v4.12_ensembleTrue" },
        { 37, "rife-v4.12-lite",             "rife-v4.12-lite" },
        { 38, "rife-v4.12-lite (ensemble)",  "rife-v4.12-lite_ensembleTrue" },
        { 39, "rife-v4.13",                  "rife-v4.13" },
        { 40, "rife-v4.13 (ensemble)",       "rife-v4.13_ensembleTrue" },
        { 41, "rife-v4.13-lite",             "rife-v4.13-lite" },
        { 42, "rife-v4.13-lite (ensemble)",  "rife-v4.13-lite_ensembleTrue" },
        { 43, "rife-v4.14",                  "rife-v4.14" },
        { 44, "rife-v4.14 (ensemble)",       "rife-v4.14_ensembleTrue" },
        { 45, "rife-v4.14-lite",             "rife-v4.14-lite" },
        { 46, "rife-v4.14-lite (ensemble)",  "rife-v4.14-lite_ensembleTrue" },
        { 47, "rife-v4.15",                  "rife-v4.15" },
        { 48, "rife-v4.15 (ensemble)",       "rife-v4.15_ensembleTrue" },
        { 49, "rife-v4.15-lite",             "rife-v4.15-lite" },
        { 50, "rife-v4.15-lite (ensemble)",  "rife-v4.15-lite_ensembleTrue" },
        { 51, "rife-v4.16-lite",             "rife-v4.16-lite" },
        { 52, "rife-v4.16-lite (ensemble)",  "rife-v4.16-lite_ensembleTrue" },
        { 53, "rife-v4.17",                  "rife-v4.17" },
        { 54, "rife-v4.17 (ensemble)",       "rife-v4.17_ensembleTrue" },
        { 55, "rife-v4.17-lite",             "rife-v4.17-lite" },
        { 56, "rife-v4.17-lite (ensemble)",  "rife-v4.17-lite_ensembleTrue" },
        { 57, "rife-v4.18",                  "rife-v4.18" },
        { 58, "rife-v4.18 (ensemble)",       "rife-v4.18_ensembleTrue" },
        { 59, "rife-v4.19-beta",             "rife-v4.19-beta" },
        { 60, "rife-v4.19-beta (ensemble)",  "rife-v4.19-beta_ensembleTrue" },
        { 61, "rife-v4.20",                  "rife-v4.20" },
        { 62, "rife-v4.20 (ensemble)",       "rife-v4.20_ensembleTrue" },
        { 63, "rife-v4.21",                  "rife-v4.21" },
        { 64, "rife-v4.22",                  "rife-v4.22" },
        { 65, "rife-v4.22-lite",             "rife-v4.22-lite" },
        { 66, "rife-v4.23-beta",             "rife-v4.23-beta" },
        { 67, "rife-v4.24",                  "rife-v4.24" },
        { 68, "rife-v4.24 (ensemble)",       "rife-v4.24_ensembleTrue" },
        { 69, "rife-v4.25",                  "rife-v4.25" },
        { 70, "rife-v4.25-lite",             "rife-v4.25-lite" },
        { 71, "rife-v4.25-heavy",            "rife-v4.25-heavy" },
        { 72, "rife-v4.26",                  "rife-v4.26" },
        { 73, "rife-v4.26-large",            "rife-v4.26-large" }
    };

    int ncnnFound = 0;
    if (ncnnDir.exists()) {
        for (const auto &m : kAllNcnnModels) {
            QString p1 = ncnnModelsDir + "/" + QString(m.dir);
            QString p2 = ncnnModelsDir + "/" + QString(m.dir) + QString("_ensembleFalse");
            bool exists = (QDir(p1).exists() && (QFile(p1 + "/flownet.param").exists() || QFile(p1 + "/flownet.bin").exists())) ||
                          (QDir(p2).exists() && (QFile(p2 + "/flownet.param").exists() || QFile(p2 + "/flownet.bin").exists()));
            if (exists) {
                _ui->RIFEModelVKDD->addItem(QString(m.name), m.id);
                ncnnFound++;
            }
        }
    }

    if (ncnnFound == 0) {
        // Fallback: populate all models if no models detected on disk yet
        for (const auto &m : kAllNcnnModels) {
            _ui->RIFEModelVKDD->addItem(QString(m.name), m.id);
        }
    }

    int restoredVkIdx = -1;
    if (currentVkModel >= 0) {
        for (int i = 0; i < _ui->RIFEModelVKDD->count(); i++) {
            if (_ui->RIFEModelVKDD->itemData(i).toInt() == currentVkModel) {
                restoredVkIdx = i;
                break;
            }
        }
    }
    if (restoredVkIdx >= 0) {
        _ui->RIFEModelVKDD->setCurrentIndex(restoredVkIdx);
    } else if (_ui->RIFEModelVKDD->count() > 0) {
        _ui->RIFEModelVKDD->setCurrentIndex(0);
    }
    _ui->RIFEModelVKDD->blockSignals(false);

    #ifdef Q_OS_WINDOWS
    // 2. Refresh CUDA (HolyWu vs-rife) models
    QString currentCudaModel = _ui->RIFEModelCADD->currentData().isValid()
        ? _ui->RIFEModelCADD->currentData().toString()
        : _ui->RIFEModelCADD->currentText().remove(QString("v"));

    _ui->RIFEModelCADD->blockSignals(true);
    _ui->RIFEModelCADD->clear();

    static const char *kAllCudaModels[] = {
        "4.0", "4.1", "4.2", "4.3", "4.4", "4.5", "4.6", "4.7", "4.8", "4.9",
        "4.10", "4.11", "4.12", "4.12.lite", "4.13", "4.13.lite", "4.14", "4.14.lite",
        "4.15", "4.15.lite", "4.16.lite", "4.17", "4.17.lite", "4.18", "4.19",
        "4.20", "4.21", "4.22", "4.22.lite", "4.23", "4.24", "4.25", "4.25.lite",
        "4.25.heavy", "4.26", "4.26.heavy"
    };

    QStringList cudaSearchDirs;
    cudaSearchDirs << QDir::toNativeSeparators(QCoreApplication::applicationDirPath() + QString("\\vs\\vsrife\\models"));
    cudaSearchDirs << QDir::toNativeSeparators(QCoreApplication::applicationDirPath() + QString("\\vs\\vsrife"));
    cudaSearchDirs << QDir::toNativeSeparators(QCoreApplication::applicationDirPath() + QString("\\vs\\Lib\\site-packages\\vsrife\\models"));

    QStringList detectedCudaModels;
    for (const QString &dirPath : cudaSearchDirs) {
        QDir cDir(dirPath);
        if (cDir.exists()) {
            QStringList filters;
            filters << QString("flownet_v*.pkl") << QString("flownet_v*.engine");
            QFileInfoList files = cDir.entryInfoList(filters, QDir::Files);
            for (const QFileInfo &f : files) {
                QString fn = f.baseName();
                if (fn.startsWith(QString("flownet_v"))) {
                    QString mName = fn.mid(9);
                    if (!detectedCudaModels.contains(mName)) {
                        detectedCudaModels.append(mName);
                    }
                }
            }
        }
    }

    if (!detectedCudaModels.isEmpty()) {
        for (const auto &m : kAllCudaModels) {
            if (detectedCudaModels.contains(QString(m))) {
                _ui->RIFEModelCADD->addItem(QString("v%1").arg(m), QString(m));
                detectedCudaModels.removeAll(QString(m));
            }
        }
        for (const QString &extra : detectedCudaModels) {
            _ui->RIFEModelCADD->addItem(QString("v%1").arg(extra), extra);
        }
    } else {
        for (const auto &m : kAllCudaModels) {
            _ui->RIFEModelCADD->addItem(QString("v%1").arg(m), QString(m));
        }
    }

    int restoredCudaIdx = -1;
    if (!currentCudaModel.isEmpty()) {
        for (int i = 0; i < _ui->RIFEModelCADD->count(); i++) {
            if (_ui->RIFEModelCADD->itemData(i).toString() == currentCudaModel) {
                restoredCudaIdx = i;
                break;
            }
        }
    }
    if (restoredCudaIdx >= 0) {
        _ui->RIFEModelCADD->setCurrentIndex(restoredCudaIdx);
    } else if (_ui->RIFEModelCADD->count() > 0) {
        int v426Idx = _ui->RIFEModelCADD->findText(QString("v4.26"));
        if (v426Idx >= 0)
            _ui->RIFEModelCADD->setCurrentIndex(v426Idx);
        else
            _ui->RIFEModelCADD->setCurrentIndex(_ui->RIFEModelCADD->count() - 1);
    }
    _ui->RIFEModelCADD->blockSignals(false);
    #endif

    modelVK();
}

void EncodeGUI::modelVK() {
    if (!VideoInfo::getFrameRate().isEmpty() && !VideoInfo::getFrameRate().contains(QString("?"))) {
        double inFps = VideoInfo::getFrameRate().toDouble();
        if (inFps > 0.0) {
            _ui->OutputFPSNUD->setValue(inFps * 2.0);
        }
    }

    int modelId = _ui->RIFEModelVKDD->currentData().isValid()
        ? _ui->RIFEModelVKDD->currentData().toInt()
        : _ui->RIFEModelVKDD->currentIndex();

    if (modelId >= 0 && modelId < 9) {
        SET_VISIBLE(_ui->Times2Label);
        
        #ifdef Q_OS_WINDOWS
        SET_VISIBLE(_ui->EqualsLabel);
        #endif

        SET_DISABLED(_ui->OutputFPSNUD);
    }
    else {
        SET_INVISIBLE(_ui->Times2Label);

        #ifdef Q_OS_WINDOWS
        SET_INVISIBLE(_ui->EqualsLabel);
        #endif

        SET_ENABLED(_ui->OutputFPSNUD);
    }
}

#ifdef Q_OS_WINDOWS
void EncodeGUI::hideInterpGpuCB() {
    refreshRifeModels();

    switch (_ui->BackendDD->currentIndex()) {
    case 0:
        _ui->UseGPUCB->setChecked(true);
        SET_DISABLED(_ui->UseGPUCB);
        SET_DISABLED(_ui->GPUThreadDD);
        SET_INVISIBLE(_ui->Times2Label);
        SET_INVISIBLE(_ui->EqualsLabel);
        SET_ENABLED(_ui->OutputFPSNUD);

        if (_ui->ToolInterpDD->currentIndex() != 0) {
            SET_VISIBLE(_ui->RIFEModelCADD);
            SET_INVISIBLE(_ui->RIFEModelVKDD);
        }
        else if (_ui->ToolInterpDD->currentIndex() == 0) {
            _ui->ModelInterpDD->removeItem(2);
            _ui->ModelInterpDD->setCurrentIndex(0);
        }

        _ui->BackendDD->removeItem(2);

        break;
    case 1:
        if (_ui->ToolInterpDD->currentIndex() != 0) {
            SET_INVISIBLE(_ui->RIFEModelCADD);
            SET_VISIBLE(_ui->RIFEModelVKDD);
        }
        else
            if (_ui->ModelInterpDD->count() != 3)
                _ui->ModelInterpDD->addItem(QString("Slow"));

        if (_ui->ToolInterpDD->currentIndex() == 1) {
            SET_VISIBLE(_ui->Times2Label);

            _ui->UseGPUCB->setChecked(true);
            SET_DISABLED(_ui->UseGPUCB);

            modelVK();
        }

        SET_ENABLED(_ui->GPUThreadDD);

        _ui->BackendDD->removeItem(2);

        break;
    }
}

void EncodeGUI::toolInterp() {
    switch (_ui->ToolInterpDD->currentIndex()) {
    case 0:
        SET_ENABLED(_ui->BackendDD);
        SET_VISIBLE(_ui->SceneChangeLabel);
        SET_VISIBLE(_ui->SceneChangeCB);
        SET_VISIBLE(_ui->ModelInterpLabel);
        SET_VISIBLE(_ui->SCThresholdLabel);
        SET_VISIBLE(_ui->SCThresholdNUD);
        SET_INVISIBLE(_ui->RIFEModelVKDD);
        SET_VISIBLE(_ui->GPUThreadInterpLabel);
        SET_VISIBLE(_ui->GPUThreadDD);
        SET_ENABLED(_ui->OutputFPSNUD);
        SET_INVISIBLE(_ui->EqualsLabel);
        SET_INVISIBLE(_ui->ParamsCB);
        SET_INVISIBLE(_ui->InterpModeLabel);
        SET_INVISIBLE(_ui->RIFEModelCADD);
        SET_INVISIBLE(_ui->ShaderLabel);
        SET_INVISIBLE(_ui->ShaderDD);
        SET_INVISIBLE(_ui->InterpModeDD);
        SET_INVISIBLE(_ui->ArtefactMaskLabel);
        SET_INVISIBLE(_ui->ArtefactMaskDD);
        SET_VISIBLE(_ui->ModelInterpDD);
        SET_INVISIBLE(_ui->Times2Label);
        SET_ENABLED(_ui->OutputFPSNUD);

        if (!CHECKED(_ui->BatchCB) && !VideoInfo::getFrameRate().contains(QString("?")))
            _ui->OutputFPSNUD->setMinimum(VideoInfo::getFrameRate().toDouble() * static_cast<double>(1.25));

        if (_ui->BackendDD->currentIndex() == 0 || _ui->BackendDD->currentIndex() == 2) {
            _ui->ModelInterpDD->removeItem(2);
            _ui->ModelInterpDD->setCurrentIndex(0);
        }
        else
            if (_ui->ModelInterpDD->count() != 3)
                _ui->ModelInterpDD->addItem(QString("Slow"));

        _ui->BackendDD->removeItem(2);

        hideInterpGpuCB();
        hideParams();
        break;
    case 1:
        SET_ENABLED(_ui->BackendDD);
        SET_VISIBLE(_ui->SceneChangeLabel);
        SET_VISIBLE(_ui->SceneChangeCB);
        SET_VISIBLE(_ui->ModelInterpLabel);
        SET_VISIBLE(_ui->SCThresholdLabel);
        SET_VISIBLE(_ui->SCThresholdNUD);
        SET_VISIBLE(_ui->GPUThreadInterpLabel);
        SET_VISIBLE(_ui->GPUThreadDD);
        SET_INVISIBLE(_ui->ShaderLabel);
        SET_INVISIBLE(_ui->ShaderDD);
        SET_INVISIBLE(_ui->InterpModeLabel);
        SET_INVISIBLE(_ui->InterpModeDD);
        SET_INVISIBLE(_ui->ArtefactMaskLabel);
        SET_INVISIBLE(_ui->ArtefactMaskDD);
        SET_INVISIBLE(_ui->ParamsCB);
        SET_INVISIBLE(_ui->ModelInterpDD);

        if (_ui->BackendDD->currentIndex() == 1)
            modelVK();

        _ui->BackendDD->removeItem(2);

        hideInterpGpuCB();
        hideParams();
        break;
    case 2:
        SET_DISABLED(_ui->BackendDD);
        SET_INVISIBLE(_ui->SceneChangeLabel);
        SET_INVISIBLE(_ui->SceneChangeCB);
        SET_INVISIBLE(_ui->SCThresholdLabel);
        SET_INVISIBLE(_ui->SCThresholdNUD);
        SET_INVISIBLE(_ui->ModelInterpLabel);
        SET_INVISIBLE(_ui->RIFEModelVKDD);
        SET_INVISIBLE(_ui->GPUThreadInterpLabel);
        SET_INVISIBLE(_ui->GPUThreadDD);
        SET_ENABLED(_ui->OutputFPSNUD);
        SET_INVISIBLE(_ui->Times2Label);
        SET_VISIBLE(_ui->ShaderLabel);
        SET_VISIBLE(_ui->ShaderDD);
        SET_VISIBLE(_ui->ParamsCB);
        SET_VISIBLE(_ui->InterpModeLabel);
        SET_VISIBLE(_ui->InterpModeDD);
        SET_INVISIBLE(_ui->EqualsLabel);
        SET_VISIBLE(_ui->ArtefactMaskLabel);
        SET_VISIBLE(_ui->ArtefactMaskDD);
        SET_INVISIBLE(_ui->RIFEModelCADD);
        SET_INVISIBLE(_ui->ModelInterpDD);

        _ui->BackendDD->addItem(QString("OpenCL"));
        _ui->BackendDD->setCurrentIndex(2);

        if (_ui->BackendDD->currentIndex() == 2)
            SET_ENABLED(_ui->UseGPUCB);
        else
            _ui->BackendDD->setCurrentIndex(2);
        if (CHECKED(_ui->ParamsCB))
            hideParams();

        _ui->OutputFPSNUD->setMinimum(5);

        break;
    }
}
#endif

void EncodeGUI::autoAjustU() {
    if (CHECKED(_ui->AutoAdjCB)) {
        SET_ENABLED(_ui->AutoAdjDD);
        autoAdjustUD();
    }
    else {
        SET_ENABLED(_ui->Width2xNUD);
        SET_ENABLED(_ui->Height2xNUD);
        SET_DISABLED(_ui->AutoAdjDD);
    }
}

void EncodeGUI::autoAdjHeight2x() {
    if (CHECKED(_ui->AutoAdjCB) && !_ui->Height2xNUD->isEnabled() && VideoInfo::getHeight() != 0) {
        double multi = static_cast<double>(VideoInfo::getHeight()) / VideoInfo::getWidth();
        _ui->Height2xNUD->setValue((int)(_ui->Width2xNUD->value() * multi));
    }
}

void EncodeGUI::autoAdjWidth2x() {
    if (CHECKED(_ui->AutoAdjCB) && !_ui->Width2xNUD->isEnabled() && VideoInfo::getHeight() != 0) {
        double multi = static_cast<double>(VideoInfo::getWidth()) / VideoInfo::getHeight();
        _ui->Width2xNUD->setValue((int)(_ui->Height2xNUD->value() * multi));
    }
}

#ifdef Q_OS_WINDOWS
void EncodeGUI::hideParams() {
    if (CHECKED(_ui->ParamsCB) && _ui->ToolInterpDD->currentIndex() == 2) {
        SET_DISABLED(_ui->UseGPUCB);
        SET_DISABLED(_ui->UseGPUCB);
        SET_INVISIBLE(_ui->OutputFPSNUD);
        SET_INVISIBLE(_ui->OutFPSLabel);
        SET_VISIBLE(_ui->SmoothLabel);
        SET_VISIBLE(_ui->SmoothTxtBox);
        SET_VISIBLE(_ui->AnalyseLabel);
        SET_VISIBLE(_ui->AnalyseTxtBox);
        SET_VISIBLE(_ui->SuperLabel);
        SET_VISIBLE(_ui->SuperTxtBox);
        SET_INVISIBLE(_ui->InterpModeLabel);
        SET_INVISIBLE(_ui->InterpModeDD);
        SET_INVISIBLE(_ui->ShaderLabel);
        SET_INVISIBLE(_ui->ShaderDD);
        SET_INVISIBLE(_ui->ArtefactMaskLabel);
        SET_INVISIBLE(_ui->ArtefactMaskDD);
    }
    else {
        SET_INVISIBLE(_ui->SmoothLabel);
        SET_INVISIBLE(_ui->SmoothTxtBox);
        SET_INVISIBLE(_ui->AnalyseLabel);
        SET_INVISIBLE(_ui->AnalyseTxtBox);
        SET_INVISIBLE(_ui->SuperLabel);
        SET_INVISIBLE(_ui->SuperTxtBox);

        if (_ui->ToolInterpDD->currentIndex() == 2) {
            SET_VISIBLE(_ui->InterpModeLabel);
            SET_VISIBLE(_ui->InterpModeDD);
            SET_VISIBLE(_ui->ShaderLabel);
            SET_VISIBLE(_ui->ShaderDD);
            SET_VISIBLE(_ui->ArtefactMaskLabel);
            SET_VISIBLE(_ui->ArtefactMaskDD);
            SET_ENABLED(_ui->UseGPUCB);
        }

        SET_VISIBLE(_ui->OutputFPSNUD);
        SET_VISIBLE(_ui->OutFPSLabel);
    }
}
#endif

void EncodeGUI::autoAdjustUD() {
    if (CHECKED(_ui->AutoAdjCB))
        switch (_ui->AutoAdjDD->currentIndex()) {
        case 0:
            SET_DISABLED(_ui->Height2xNUD);
            SET_ENABLED(_ui->Width2xNUD);
            break;
        case 1:
            SET_DISABLED(_ui->Width2xNUD);
            SET_ENABLED(_ui->Height2xNUD);
            break;
        }
}

void EncodeGUI::modelUpScaleGB() {
    if (_ui->ModelUpscaleDD->currentIndex() == 3 && _ui->ToolUpscaleDD->currentIndex() == 0) {
        SET_DISABLED(_ui->Width2xNUD);
        SET_DISABLED(_ui->Height2xNUD);
        SET_DISABLED(_ui->AutoAdjCB);
        SET_DISABLED(_ui->AutoAdjDD);

        if (VideoInfo::getHeight() != 0) {
            _ui->Height2xNUD->setValue(VideoInfo::getHeight());
            _ui->Width2xNUD->setValue(VideoInfo::getWidth());
        }
    }
    else {
        SET_ENABLED(_ui->AutoAdjCB);
        SET_ENABLED(_ui->AutoAdjDD);
        autoAdjustUD();

        if (VideoInfo::getHeight() != 0 && _ui->ToolUpscaleDD->currentIndex() == 0) {
            _ui->Height2xNUD->setValue(VideoInfo::getHeight() * 2);
            _ui->Width2xNUD->setValue(VideoInfo::getWidth() * 2);
        }
    }
}

void EncodeGUI::hideUpscale() {
    modelUpScaleGB();

    switch (_ui->ToolUpscaleDD->currentIndex()) {
    case 0:
        SET_INVISIBLE(_ui->TTA2xCB);
        SET_INVISIBLE(_ui->TTA2xLabel);
        SET_VISIBLE(_ui->NoiseReduc2xLabel);
        SET_VISIBLE(_ui->ModelUpscaleDD);
        SET_VISIBLE(_ui->ModelUpscaleLabel);
        SET_VISIBLE(_ui->Precision2xDD);
        SET_VISIBLE(_ui->Precision2xLabel);
        SET_VISIBLE(_ui->NoiseReduc2xDD);
        break;
    case 1:
        SET_VISIBLE(_ui->TTA2xCB);
        SET_VISIBLE(_ui->TTA2xLabel);
        SET_INVISIBLE(_ui->NoiseReduc2xLabel);
        SET_INVISIBLE(_ui->ModelUpscaleDD);
        SET_INVISIBLE(_ui->ModelUpscaleLabel);
        SET_INVISIBLE(_ui->Precision2xDD);
        SET_INVISIBLE(_ui->Precision2xLabel);
        SET_INVISIBLE(_ui->NoiseReduc2xDD);
        break;
    }
}