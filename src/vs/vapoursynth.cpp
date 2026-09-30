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
		"import sys, os, tempfile\n"
		"try:\n"
		"    import site\n"
		"    site.main()\n"
		"except Exception:\n"
		"    pass\n"
		"for _p in [r'%1', r'%2', r'%3']:\n"
		"    if os.path.exists(_p) and _p not in sys.path:\n"
		"        sys.path.insert(0, _p)\n\n"
		"_trt_dir = os.path.join(os.getenv('LOCALAPPDATA', tempfile.gettempdir()), 'EncodeGUI', 'trt_cache')\n"
		"try:\n"
		"    os.makedirs(_trt_dir, exist_ok=True)\n"
		"except Exception:\n"
		"    pass\n\n"
		"try:\n"
		"    import tensorrt\n"
		"    if not hasattr(tensorrt, '__version__'):\n"
		"        _tv = getattr(tensorrt, 'version', None)\n"
		"        if not _tv:\n"
		"            try:\n"
		"                import importlib.metadata\n"
		"                _tv = importlib.metadata.version('tensorrt')\n"
		"            except Exception:\n"
		"                _tv = '10.0.0'\n"
		"        tensorrt.__version__ = str(_tv)\n"
		"except Exception:\n"
		"    pass\n\n"
		"try:\n"
		"    import torch_tensorrt\n"
		"    if not hasattr(torch_tensorrt, '__version__'):\n"
		"        torch_tensorrt.__version__ = '2.0.0'\n"
		"except Exception:\n"
		"    pass\n\n"
		"try:\n"
		"    import torch\n"
		"    if hasattr(torch, 'load'):\n"
		"        _orig_torch_load = torch.load\n"
		"        def _safe_torch_load(*args, **kwargs):\n"
		"            kwargs.pop('mmap', None)\n"
		"            return _orig_torch_load(*args, **kwargs)\n"
		"        torch.load = _safe_torch_load\n"
		"    if hasattr(torch, 'device'):\n"
		"        _orig_device = torch.device\n"
		"        class _SafeDeviceType(type):\n"
		"            def __instancecheck__(cls, instance):\n"
		"                return isinstance(instance, _orig_device)\n"
		"            def __subclasscheck__(cls, subclass):\n"
		"                return issubclass(subclass, _orig_device)\n"
		"        class _SafeDevice(metaclass=_SafeDeviceType):\n"
		"            def __new__(cls, *args, **kwargs):\n"
		"                if args and (args[0] == 'meta' or str(args[0]) == 'meta'):\n"
		"                    args = ('cpu',) + args[1:]\n"
		"                elif kwargs.get('type') == 'meta':\n"
		"                    kwargs['type'] = 'cpu'\n"
		"                return _orig_device(*args, **kwargs)\n"
		"        torch.device = _SafeDevice\n"
		"    if hasattr(torch, 'set_default_device'):\n"
		"        _orig_set_default_device = torch.set_default_device\n"
		"        def _safe_set_default_device(dev):\n"
		"            if dev == 'meta' or str(dev) == 'meta':\n"
		"                dev = 'cpu'\n"
		"            return _orig_set_default_device(dev)\n"
		"        torch.set_default_device = _safe_set_default_device\n"
		"    if hasattr(torch.nn.Module, 'load_state_dict'):\n"
		"        _orig_load_state_dict = torch.nn.Module.load_state_dict\n"
		"        def _safe_load_state_dict(self, state_dict, strict=True, assign=False):\n"
		"            try:\n"
		"                return _orig_load_state_dict(self, state_dict, strict=strict, assign=assign)\n"
		"            except TypeError:\n"
		"                return _orig_load_state_dict(self, state_dict, strict=strict)\n"
		"        torch.nn.Module.load_state_dict = _safe_load_state_dict\n"
		"    if not hasattr(torch, 'set_float32_matmul_precision'):\n"
		"        torch.set_float32_matmul_precision = lambda *a, **k: None\n"
		"except Exception:\n"
		"    pass\n\n"
		"try:\n"
		"    from tqdm import tqdm\n"
		"except Exception:\n"
		"    import types\n"
		"    _t_mod = types.ModuleType('tqdm')\n"
		"    class _Tqdm:\n"
		"        def __init__(self, *a, **k): pass\n"
		"        def __enter__(self): return self\n"
		"        def __exit__(self, *a): pass\n"
		"        def update(self, *a): pass\n"
		"    _t_mod.tqdm = _Tqdm\n"
		"    sys.modules['tqdm'] = _t_mod\n\n"
		"try:\n"
		"    import requests\n"
		"except Exception:\n"
		"    import types, urllib.request\n"
		"    class _ReqResponse:\n"
		"        def __init__(self, resp):\n"
		"            self._resp = resp\n"
		"            self.headers = resp.headers\n"
		"        def iter_content(self, chunk_size=4096):\n"
		"            while True:\n"
		"                chunk = self._resp.read(chunk_size)\n"
		"                if not chunk:\n"
		"                    break\n"
		"                yield chunk\n"
		"    class _RequestsMod(types.ModuleType):\n"
		"        @staticmethod\n"
		"        def get(url, stream=False):\n"
		"            req = urllib.request.Request(url, headers={'User-Agent': 'Mozilla/5.0'})\n"
		"            return _ReqResponse(urllib.request.urlopen(req))\n"
		"    sys.modules['requests'] = _RequestsMod('requests')\n\n"
		"try:\n"
		"    from vsrife import rife as _vs_rife_raw\n"
		"    def RIFE(*args, **kwargs):\n"
		"        _req_trt = kwargs.get('trt', False)\n"
		"        if _req_trt and (not hasattr(torch, 'export') or not hasattr(torch.export, 'export')):\n"
		"            kwargs['trt'] = False\n"
		"            kwargs.pop('trt_cache_dir', None)\n"
		"        if kwargs.get('fps_num') and kwargs.get('fps_den'):\n"
		"            from fractions import Fraction\n"
		"            _f_num = kwargs.pop('fps_num')\n"
		"            _f_den = kwargs.pop('fps_den')\n"
		"            if len(args) > 0 and hasattr(args[0], 'fps') and args[0].fps > 0:\n"
		"                _factor = (Fraction(_f_num, _f_den) / args[0].fps).limit_denominator(100)\n"
		"                if _factor <= 1:\n"
		"                    _factor = Fraction(2, 1)\n"
		"            else:\n"
		"                _factor = Fraction(2, 1)\n"
		"            kwargs['factor_num'] = _factor.numerator\n"
		"            kwargs['factor_den'] = _factor.denominator\n"
		"        _m = kwargs.get('model', '4.26')\n"
		"        try:\n"
		"            import vsrife\n"
		"            _mf = os.path.join(vsrife.model_dir, f\"flownet_v{_m}.pkl\")\n"
		"            if not os.path.exists(_mf):\n"
		"                _found = False\n"
		"                for _alt in [os.path.join(r'%1', 'models'), os.path.join(r'%1', 'vsrife', 'models')]:\n"
		"                    _cand = os.path.join(_alt, f\"flownet_v{_m}.pkl\")\n"
		"                    if os.path.exists(_cand) and os.path.getsize(_cand) > 0:\n"
		"                        import shutil\n"
		"                        os.makedirs(vsrife.model_dir, exist_ok=True)\n"
		"                        shutil.copyfile(_cand, _mf)\n"
		"                        _found = True\n"
		"                        break\n"
		"                if not _found:\n"
		"                    os.makedirs(vsrife.model_dir, exist_ok=True)\n"
		"                    open(_mf, 'wb').close()\n"
		"        except Exception:\n"
		"            pass\n"
		"        try:\n"
		"            return _vs_rife_raw(*args, **kwargs)\n"
		"        except Exception as _err:\n"
		"            if _req_trt and kwargs.get('trt', False):\n"
		"                kwargs['trt'] = False\n"
		"                kwargs.pop('trt_cache_dir', None)\n"
		"                return _vs_rife_raw(*args, **kwargs)\n"
		"            raise _err\n"
		"except ImportError:\n"
		"    try:\n"
		"        from vsrife import RIFE\n"
		"    except ImportError as _e:\n"
		"        raise ImportError(f\"Could not load RIFE module. Ensure 'vsrife' and 'torch' are installed in the EncodeGUI vs environment: {_e}\")\n\n"
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