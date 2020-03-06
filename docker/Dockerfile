FROM ubuntu:18.04

RUN apt-get update && apt-get install -y --no-install-recommends wget unzip git make \
 srecord bc xz-utils gcc python curl python-pip python-dev build-essential \
 && python -m pip install --upgrade pip setuptools

RUN pip install -U platformio
RUN pio platform install espressif8266@2.2.3 --with-package framework-arduinoespressif8266
RUN pio platform install espressif8266 --with-package framework-arduinoespressif8266

RUN /usr/local/bin/platformio update

# now this pio project is dealing with platformio 4.0 features
RUN /usr/local/bin/platformio upgrade --dev

# Prebuild one project to install the complete environment
RUN git clone https://github.com/thorsten-l/ESP8266-Arduino-Info.git
WORKDIR /ESP8266-Arduino-Info
RUN cp lib/App/App.hpp.sample lib/App/App.hpp
RUN /usr/local/bin/platformio run
RUN rm -fr /ESP8266-Arduino-Info

RUN mkdir /workdir
WORKDIR /workdir

CMD ["/bin/bash"]
# CMD [ "/usr/local/bin/platformio", "run" ]
