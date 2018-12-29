#!/bin/sh
$HOME/.platformio/penv/bin/python \
  $HOME/.platformio/packages/tool-espotapy/espota.py \
  --debug --progress -i "wifi-socket-ii.local" \
  --auth="otapass" -f "firmware-2.0.5.bin"
