FROM ubuntu:18.04

WORKDIR /home

RUN apt-get update && apt-get install -y make openmpi-bin libopenmpi-dev fftw3-dev time ssh

# This is only needed for debugging purpose
RUN apt-get install -y vim

COPY ./ ./

RUN cd libcseife && make

RUN cd src && make denise

CMD make test
