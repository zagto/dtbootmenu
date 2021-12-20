# Kexec-Hardboot based bootloader/boot menu for booting modern Linux kernels on ASUS TF300T

supports device trees (DTB). This repo contains the frontend. The kernel with the actual hardboot patch is found [here](https://github.com/zagto/tf300t-dtboot-kernel).

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


## Prepare the Linux disk
In the `/boot` directory, where the kernels and initrds are, add your zImage (we assume here you want to use
 a custom kernel from the [Grate](https://github.com/grate-driver/linux.git) poject, which includes drivers
 needed for our device that are not yet upstreamed).
 
Now, add a file called `mylinux.dtboomenu` with the following contents:
```
kernel: <name of your zImage file>
initrd: <name of initrd file that comes with distro>
title: <title the entry should have in the menu>
cmdline: console=tty0 root=<root device> rw
```
The root drive is the typical Linux block device path your Linux installation is on, for example `/dev/mmcblk1p1`
for first partition of the microSD, or `/dev/sda1` for first partition of USB drive.

Complete example:

```
kernel: zImage-grate
initrd: initrd.img
title: Debian
cmdline: console=tty0 root=/dev/mmcblk1p1 rw
```

## Flash the loader

Put the tablet into fastboot mode by holding the `Volume Down` key while starting. Run the 
following to flash:
```
fastboot flash boot boot.blob
fastboot reboot
```

# Building from Sources
## `boot.blob`
### Requirements
- GCC Cross-compiler targeting armv7 Linux with C++ support. In this example the target is called
`arm-unknown-linux-uclibcgnueabi`. Change accordingly if yours calls the target differently.
- [blobtools](https://github.com/AndroidRoot/BlobTools), `abootimg`, `adb`, `fastboot`
- everything required to compile a Linux kernel

### Building the Kexec-Hardboot kernel
```
git clone https://github.com/zagto/tf300t-dtboot-kernel
cd tf300t-dtboot-kernel
export ARCH=arm
export CROSS_COMPILE=arm-unknown-linux-uclibcgnueabi-
make tf300t_dtbootloader_defconfig
make zImage -j16
```

### Building the user-space kexec binary
```
git clone git://git.kernel.org/pub/scm/utils/kernel/kexec/kexec-tools.git
cd kexec-tools
./bootstrap
LDFLAGS=-static ./configure --host=arm-unknown-linux-uclibcgnueabi
make -j16
```

### Building the menu binary from this repo
```
cd src
make CXX=arm-unknown-linux-uclibcgnueabi-g++ CXXFLAGS=-static
```

### Combining it all together
```
./create-boot-blob.sh tf300t-dtboot-kernel/arch/arm/boot/zImage kexec-tools/build/sbin/kexec src/dtbootmenu
```

## DTBs
```
git clone https://github.com/grate-driver/linux.git
cd linux
export ARCH=arm
make tegra30-asus-tf201.dtb
make tegra30-asus-tf300t.dtb
make tegra30-asus-tf700t.dtb
```
