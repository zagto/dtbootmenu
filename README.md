# Kexec-Hardboot based bootloader/boot menu for booting modern Linux kernels on ASUS TF300T

supports device trees (DTB)

# Installation
## Requirements
- ASUS TF300T tablet, or TF700T, TF201 (untested)
- PC with `adb` and `fastboot` installed
- `boot.blob` and `android-bootfiles.zip` (available from releases page of this repo)
- microSD or USB drive with an image of your favorite linux distribution for ARM


## Setup Android Dual-Booting (optional)

This loader will be installed to the tablet's boot partition. The Android kernel 
and initrd, which would normally be there, need to be moved to a different place.

extract the android bootfiles and upload the directory using `adb` as `/sdcard/boot`:
```
unzip android-bootfiles.zip
adb push android-bootfiles /sdcard/boot
```

Now you should have the following files in `/sdcard/boot`:
- `zImage-android`: Kernel image for Android
- `initrd-android`: Ramdisk image for Android
- `android.dtbootmenu`: Loader entry for this loader.

Important: The zImage and initrd files are dependent on the Android ROM you use. The ones
provided in this repo are from KatKiss version #039. If you use another ROM you have to
extract them from your ROMs `boot.blob` using `blobunpack` and `abootimg`.


## Prepare the linux drive

## Flash the loader

Put the tablet into fastboot mode by holding the `Volume Down` key while starting. Run the 
following to flash:
```
fastboot flash boot.img
fastboot reboot
```



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

