#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

function echo_info {
    echo -e "${GREEN}${1}${NC}"
}

echo_info "[+] START SECube umount"
sudo umount /media/SECube
sudo rm -rf /media/SECube
ls -al /media
mount -l | grep /media/SECube
echo_info "[+] END SECube umount"