# Simple boot menu for booting Linux kernels using Kexec Hardboot and DTB (device tree) on ASUS TF300T

## Requirements
### Tools on Host
- Cross-compiler toolchain with C++ support
- [blobtools](https://github.com/AndroidRoot/BlobTools), abootimg, fastboot

### Things you cross-compile 
- Kernel with Kexec Hardboot DTB support:
  <https://github.com/tzagorni/tf300t-dtboot-kernel>
- kexec-tools (requires zlib):
  <https://mirrors.edge.kernel.org/pub/linux/utils/kernel/kexec/>
- A device tree binary file for the device (`tegra30-tf300t.dtb`).
  In a kernel source tree that contains the right device tree source ([mine](https://github.com/tzagorni/linux-tf300t)), run:
  ```
  make tegra30-tf300t.dtb
  ```
  The resulting file will be in `arch/arm/dts/tegra30-tf300t.dtb`

*make sure to set `$CC`/`CXX` variables etc. for cross compiling and to use static linking*

## Building the boot menu binary `dtbootmenu`
```
pushd src
make
popd
```

## Setup Android Dual-Booting (optional)
*ideally, all of this should happen automatically in a "flashable zip" or something. But currently nothing like this is implemented.*

On the device, create a directory named `boot` in the internal "SD card" directory. Place the following files in it
- `zImage-android`: Kernel image for Android (extract from your ROM's installer zip using abootimg)
- `initrd-android`: Ramdisk image for Android (extract from your ROM's installer zip using abootimg)
- `android.dtbootmenu` with the following contents:
  ```
  kernel: zImage-android
  initrd: initrd-android
  title: Android
  legacy
  ```

## Flashing
You need:
- Kernel zImage binary (tf300t-dtboot-kernel)
- `tegra30-tf300t.dtb` file
- `kexec` binary
- `dtbootmenu` binary

```
./create-boot-blob.sh <zImage> <dtb file> <kexec binary> src/dtbootmenu
sudo fastboot flash boot boot.blob
```

