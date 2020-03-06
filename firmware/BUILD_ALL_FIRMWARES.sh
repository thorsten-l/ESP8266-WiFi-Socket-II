#!/bin/bash

cd ..

for pioenv in dev1 dev1lite obi_v1 obi_v2 bw_shp6 bw_shp6_v11 shelly1
do
  echo "Build environment: $pioenv"
  platformio run -s -t clean -e $pioenv
  platformio run -s -e $pioenv
  cp .pio/build/$pioenv/firmware-*.bin firmware
done
