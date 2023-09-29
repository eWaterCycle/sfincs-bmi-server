# sfincs-bmi-server

grpc4bmi wrapper around Delteras/sfincs BMI for use with eWaterCycle.

* See https://github.com/Deltares/SFINCS/tree/main
* See https://grpc4bmi.readthedocs.io/en/latest/server/Cpp.html#c-c-fortran

To interop with grpc, a c++ wrapper is added on top of the fortran code. So we have:

sfincs_bmi.f90 --> sfincs_bmi.cxx --> sfincs_bmi_server.cxx

The c++ wrapper is modelled after [bmi-heat c++ example, version
2.1](https://github.com/csdms/bmi-example-cxx/blob/2ede037a29e34d66c274768caad1a891a70c7897/heat/bmi_heat.hxx).

For an example see https://github.com/eWaterCycle/hype-bmi/. Note that this uses
an older version of the BMI spec.

## Using it

Build docker image with

```shell
docker build -t sfincs-bmiserver .
```

To interact with it through a grpc4bmi client:

```py
from grpc4bmi.bmi_client_docker import BmiClientDocker

model = BmiClientDocker(image='sfincs-bmiserver', image_port=50051, work_dir="./")
model.get_component_name()
# 'Sfincs hydrodynamic model (C)'

del model
```

See the example notebook shipped with this repo.

See https://grpc4bmi.readthedocs.io/en/latest/container/usage.html#docker

## Development container

To aid development, this repo is configured for use with [vscode
devcontainers](https://code.visualstudio.com/docs/devcontainers/containers). In
vscode, install the devcontainers extension, then from the command pallete
choose "Dev Containers: Open Folder in Container". This will build the container
mount your working directory, and open the remote environment in vscode. The c++
and docker extensions are automatically be loaded.

To rebuild the server without rebuilding Docker image use

```sh
cd src
mkdir build
cmake ..
make
# Produces a `./sfincs_bmi_server` executable.
./sfincs_bmi_server
```

To interact with it through a grpc4bmi client:

```py
import grpc
from grpc4bmi.bmi_grpc_client import BmiClient

model = BmiClient(grpc.insecure_channel("localhost:50051"))

model.get_component_name()
# 'Sfincs hydrodynamic model (C)'
```

## Publish image

After build, publish image to https://github.com/orgs/eWaterCycle/packages with

```shell
docker tag sfincs-bmiserver ghcr.io/ewatercycle/sfincs-bmiserver:sfincs-v2.0.2-Blockhaus-Release-Q2-2023
docker push ghcr.io/ewatercycle/sfincs-bmiserver:sfincs-v2.0.2-Blockhaus-Release-Q2-2023
```