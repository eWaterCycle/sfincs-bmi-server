# Install sfincs-bmi-server + its dependencies: sfincs, grpc4bmi, grpc, bmi-c, bmi-cxx

# Implementation follows https://github.com/eWaterCycle/grpc4bmi/blob/main/test/heat-images/cxx-bmi20/Dockerfile
# Also see https://github.com/Deltares/SFINCS/blob/main/source/Dockerfile

FROM debian:buster AS builder

# Install build deps
RUN apt-get update && apt-get install -qy git build-essential cmake autoconf libtool pkg-config libssl-dev

# Compile gRPC
WORKDIR /opt/grpc
ARG GRPC_VERSION=v1.51.1
RUN echo ${GRPC_VERSION}
RUN git clone -b ${GRPC_VERSION} --depth 1 https://github.com/grpc/grpc . && git submodule update --init
WORKDIR /opt/grpc/cmake/build
RUN cmake -DgRPC_INSTALL=ON  -DgRPC_SSL_PROVIDER=package -DgRPC_BUILD_TESTS=OFF -DBUILD_SHARED_LIBS=ON ../.. \
    && make -j6 && make install && ldconfig

# Install bmi-cxx
ARG BMICXX_VERSION=v2.0
RUN git clone -b ${BMICXX_VERSION} https://github.com/csdms/bmi-cxx.git /opt/bmi-cxx
WORKDIR /opt/bmi-cxx/build
RUN cmake .. && make install

# Install bmi-c
ARG BMIC_VERSION=v2.1
RUN git clone -b ${BMIC_VERSION} https://github.com/csdms/bmi-c.git /opt/bmi-c
WORKDIR /opt/bmi-c/build
RUN cmake .. && make install

# Install grpc4bmi
RUN git clone https://github.com/eWaterCycle/grpc4bmi /opt/grpc4bmi
WORKDIR /opt/grpc4bmi/cpp/build
RUN cmake .. && make install

# TODO Get everything above from a shared base container?

# Install sfincs
# https://hub.docker.com/r/deltares/sfincs-cpu/tags
# Has /usr/local/lib/libsfincs which already exposes the basic modelling interface
# ...
# FROM deltares/sfincs-cpu:sfincs-v2.0.2-Blockhaus-Release-Q2-2023

# Build sfincs-bmi-server
RUN g++ -o sfincs_bmi_server sfincs_bmi_server.cxx sfincs_bmi.cxx `pkg-config --libs protobuf grpc++ grpc` -Wl,--no-as-needed -lgrpc++_reflection -ldl -lgrpc4bmi

# Expose entrypoint
ENV BMI_PORT=50051
ENTRYPOINT ["/usr/local/bin/sfincs_bmi_server"]
