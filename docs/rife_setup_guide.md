# RIFE Frame Interpolation Setup Guide (CUDA & TensorRT)

EncodeGUI supports multiple high-performance RIFE (Real-Time Intermediate Flow Estimation) interpolation backends:
1. **NCNN / Vulkan**: Works out-of-the-box on Windows and macOS across NVIDIA, AMD, and Intel GPUs.
2. **CUDA (HolyWu `vs-rife`)**: PyTorch-based CUDA acceleration for NVIDIA GPUs supporting models from `v4.0` up to `v4.26`.
3. **TensorRT**: Hardware-optimized neural network compilation for NVIDIA RTX GPUs providing maximum interpolation speed.

---

## 1. System Requirements

- **GPU**: NVIDIA GeForce GTX / RTX series (CUDA-compatible). 
  - *RTX 20, 30, 40, and 50 series GPUs with Tensor Cores will achieve maximum performance with TensorRT.*
- **Drivers**: Latest NVIDIA Game Ready or Studio Driver installed from [nvidia.com](https://www.nvidia.com/Download/index.aspx).
- **VapourSynth / Python**: EncodeGUI bundles its portable VapourSynth and Python runtime inside the `vs\` directory in the application folder.

---

## 2. Installing HolyWu `vs-rife` (CUDA Backend)

HolyWu `vs-rife` requires PyTorch with CUDA support.

1. Open **Command Prompt (`cmd.exe`)** as Administrator.
2. Navigate to your EncodeGUI installation's `vs` directory:
   ```cmd
   cd "C:\Program Files\EncodeGUI\vs"
   ```
   *(Replace with the actual path where your EncodeGUI is installed)*

3. Upgrade pip and build tools:
   ```cmd
   python.exe -m pip install -U packaging setuptools wheel
   ```

4. Install HolyWu `vsrife` and PyTorch with CUDA:
   ```cmd
   python.exe -m pip install -U vsrife
   python.exe -m pip install -U torch torchvision --extra-index-url https://download.pytorch.org/whl/cu124
   ```
   > **Note:** If your NVIDIA driver does not support CUDA 12.4, you can replace `cu124` with `cu121` or `cu118`.

---

## 3. Installing TensorRT (Optional, for Maximum Performance)

TensorRT compiles RIFE models into optimized `.engine` binaries tailored for your specific GPU architecture.

To install TensorRT:
1. In the same command prompt inside `EncodeGUI\vs`:
   ```cmd
   python.exe -m pip install -U tensorrt torch-tensorrt --extra-index-url https://download.pytorch.org/whl/cu124 --extra-index-url https://pypi.nvidia.com
   ```

2. **First Run Behavior with TensorRT**:
   - Check the **TensorRT** option in EncodeGUI's video interpolation settings.
   - When you start your first encode with a model (e.g. `v4.26`), TensorRT will compile an optimized `.engine` file (`flownet_v4.26.engine`) matching your GPU and video resolution.
   - **The initial compilation takes 2 to 10 minutes.** This is normal.
   - Subsequent encodes with that model will load the compiled engine instantly with significantly higher FPS and reduced VRAM consumption.
   - *Note: TensorRT is supported on RIFE models `4.2` and newer.*

---

## 4. Model Management & Dynamic Detection

EncodeGUI features **dynamic model detection** to keep the interface clean:
- **Vulkan / NCNN**: Scans `vs\plugins\models\` for downloaded model folders (e.g. `rife-v4.26`, `rife-v4.6`, `rife-v2.3`, etc.).
- **CUDA / HolyWu**: Scans `vs\vsrife\models\` (and `vs\Lib\site-packages\vsrife\models\`) for `flownet_v*.pkl` and `.engine` files.
- The dropdown menus dynamically display **only models present on disk**, eliminating clutter.

### How to obtain models:
- **On-Demand Auto-Download**:
  EncodeGUI passes `auto_download=True`. If you select a model that has not yet been downloaded, HolyWu `vs-rife` will automatically download the required weights when the encode starts.
- **Bulk Pre-Download (All 36 Models)**:
  To download all available HolyWu models at once for offline use, run:
  ```cmd
  cd "C:\Program Files\EncodeGUI\vs"
  python.exe -m vsrife
  ```
  Once downloaded into `vs\vsrife\models\`, EncodeGUI will detect them on startup and show all downloaded models in the CUDA dropdown.
