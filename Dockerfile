# Install sfincs-bmi-server + its dependencies: sfincs, grpc4bmi, grpc, bmi-c, bmi-cxx

# Implementation follows https://github.com/eWaterCycle/grpc4bmi/blob/main/test/heat-images/cxx-bmi20/Dockerfile
# Also see https://github.com/Deltares/SFINCS/blob/main/source/Dockerfile

# Build with:
# docker build -t sfincs-bmiserver .


#FROM deltares/sfincs-cpu:sfincs-v2.0.2-Blockhaus-Release-Q2-2023 as sfincs_container

FROM ubuntu:jammy AS sfincs_container

# Compile sfincs from a Github branch instead of from image
# with copy of Dockerfile at https://github.com/Deltares/SFINCS/blob/feature/48-extend-bmi-functionality/source/Dockerfile
ENV DEBIAN_FRONTEND=noninteractive
RUN apt clean && apt autoclean && apt update --fix-missing && apt upgrade -y && apt install -y libnetcdf-dev build-essential autoconf automake libtool gfortran gdb tzdata m4 git
WORKDIR /usr/src/sfincs
ARG SFINCS_VERSION=feature/48-extend-bmi-functionality
RUN git clone -b ${SFINCS_VERSION} https://github.com/Deltares/SFINCS . \
  && git config --global user.email "you@example.com" && git config --global user.name "Your Name" \
  # TODO remove this line when that commit is in $SFINCS_VERSION branch
  && git cherry-pick 2651b47b797c9acd05ccd8e6d737db19b0670217
WORKDIR /usr/src/sfincs/source
RUN chmod -R 777 autogen.sh
# -fallow-argument-mismatch needed for https://github.com/Unidata/netcdf-fortran/issues/212
ENV FCFLAGS="-fopenmp -O3 -fallow-argument-mismatch -w"
ENV FFLAGS="-fopenmp -O3 -fallow-argument-mismatch -w"
RUN autoreconf -ivf && ./autogen.sh && ./configure --disable-openacc && make && make install

# End of copy of Dockerfile at https://github.com/Deltares/SFINCS/blob/feature/48-extend-bmi-functionality/source/Dockerfile

# Inherit from the same base as sfincs to get compatible library versions
FROM ubuntu:jammy AS sfincs_bmi_container

# Install sfincs
COPY --from=sfincs_container /usr/local /usr/local

# Install build deps (including sfincs` dependency on netcdf)
RUN apt-get update && apt-get install -qy git build-essential cmake autoconf libtool pkg-config libssl-dev libnetcdf-dev gfortran

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

# Build sfincs-bmi-server (requires libsfincs)
COPY ./src /opt/sfincs-bmi
WORKDIR /opt/sfincs-bmi/build
RUN cmake .. && make install && ldconfig

# TODO can we make the container smaller?
# FROM debian:buster
# RUN apt-get update && apt-get install -qy libssl1.1 && rm -rf /var/lib/apt/lists/*
# COPY --from=sfincs_bmi_container /usr/local/bin/sfincs_bmi_server /usr/local/bin/sfincs_bmi_server
# COPY --from=sfincs_bmi_container /usr/local/lib/ /usr/local/lib/
# RUN ldconfig

# Expose entrypoint
ENV BMI_PORT=50051
ENTRYPOINT ["/usr/local/bin/sfincs_bmi_server"]

LABEL org.opencontainers.image.source https://github.com/eWaterCycle/sfincs-bmi-server
