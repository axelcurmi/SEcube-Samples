#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

function echo_info {
    echo -e "${GREEN}${1}${NC}"
}

echo_info "[+] START SECube mount"
sudo mkdir /media/SECube
sudo mount -t vfat /dev/sdc1 /media/SECube
ls -al /media/SECube
mount -l | grep /media/SECube
echo_info "[+] END SECube mount"