#!/bin/bash

cd ..

for pioenv in dev1 dev1lite obi_v1 obi_v2 bw_shp6 shelly1
do
  echo "Build environment: $pioenv"
  platformio run -s -t clean -e $pioenv
  platformio run -s -e $pioenv
  cp .pioenvs/$pioenv/firmware-*.bin firmware
done
