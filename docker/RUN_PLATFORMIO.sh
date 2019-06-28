#!/bin/sh
cd ..
docker run -it --rm \
  -v `pwd`:/workdir \
  --name platformio platformio \
  /usr/local/bin/platformio run \
    -e dev1 \
    -e dev1lite \
    -e obi_v1 \
    -e obi_v2 \
    -e bw_shp6 \
    -e shelly1
