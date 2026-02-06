# Build Guide

## Prerequisites

### Daisy Toolchain

Follow the official guide at https://daisy.audio/tutorials/cpp-dev-env/ to install:

- **ARM GCC cross-compiler** (arm-none-eabi-gcc 10.3 or later)
- **Make** (GNU Make 4.x)
- **OpenOCD** (for ST-Link debugging, optional)
- **dfu-util** (for USB firmware flashing)

#### macOS

```bash
brew install armmbed/formulae/arm-none-eabi-gcc
brew install make dfu-util
```

#### Linux (Ubuntu/Debian)

```bash
sudo apt install gcc-arm-none-eabi make dfu-util
```

#### Windows

Install the [Daisy Toolchain installer](https://daisy.audio/tutorials/cpp-dev-env/) which bundles all required tools.

## Clone & Build

```bash
# Clone with submodules
git clone --recursive https://github.com/yourusername/corrode.git
cd corrode

# Build libDaisy
cd lib/libDaisy
make
cd ../..

# Build DaisySP
cd lib/DaisySP
make
cd ../..

# Build Corrode firmware
make
```

The output binary is `build/corrode.bin`.

## Flash Firmware

### USB DFU (recommended)

1. Connect USB Micro-B cable to the Daisy Patch Submodule
2. Enter DFU mode: hold **BOOT** → press **RESET** → release **BOOT**
3. Flash:
   ```bash
   make program-dfu
   ```

### Daisy Web Programmer (no toolchain needed)

1. Enter DFU mode (see above)
2. Open https://electro-smith.github.io/Programmer/
3. Drag `build/corrode.bin` onto the page
4. Click "Program"

## Development Setup (VS Code)

The `.vscode/` directory includes pre-configured settings for:

- **IntelliSense** (C/C++ extension): ARM cross-compilation paths and defines
- **Cortex-Debug**: Step-through debugging via ST-Link/V2 probe (optional)

Install these VS Code extensions:
- C/C++ (Microsoft)
- Cortex-Debug

### Debug Build

```bash
make clean
make DEBUG=1
```

Then use the "Cortex Debug" launch configuration in VS Code with an ST-Link probe connected to the Daisy's SWD header.

## Troubleshooting

**"arm-none-eabi-gcc: command not found"**
Ensure the ARM GCC toolchain is in your PATH.

**"No DFU capable USB device available"**
The Daisy is not in DFU mode. Hold BOOT, press RESET, release BOOT. Check that dfu-util can see the device: `dfu-util -l`

**Build errors in libDaisy**
Make sure submodules are initialized: `git submodule update --init --recursive`

**Audio output is silent**
Check that Gate In 1 is receiving a gate signal. Try sending a static high gate and turning CTRL 1 (Cutoff) fully clockwise.
