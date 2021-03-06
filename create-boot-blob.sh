#!/bin/bash
set -e

if [ "$#" -ne 3 ]; then
    echo "Usage: $0 <zImage> <kexec binary> <dtbootmenu binary>"
    exit 1
fi

ZIMAGE="$1"
KEXEC="$2"
DTBOOTMENU="$3"


rm -rf out
mkdir -p out/initrd
cp "$KEXEC" out/initrd/kexec
cp "$DTBOOTMENU" out/initrd/init
chmod +x out/initrd/*
cp *.dtb out/initrd/

pushd out/initrd
find . -print0 | cpio --null -ov --format=newc | gzip -9 > ../initrd.img
popd

abootimg --create out/boot.img -k "$ZIMAGE" -f bootimg.cfg -r out/initrd.img
blobpack out/boot.blob.tosign LNX out/boot.img
echo -ne "-SIGNED-BY-SIGNBLOB-\0\0\0\0\0\0\0\0" | cat - out/boot.blob.tosign > out/boot.blob

echo "Output written to out/boot.blob"

