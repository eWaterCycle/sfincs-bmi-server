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

## Development container

To aid development, this repo is configured for use with [vscode
devcontainers](https://code.visualstudio.com/docs/devcontainers/containers). In
vscode, install the devcontainers extension, then from the command pallete
choose "Dev Containers: Open Folder in Container". This will build the container
mount your working directory, and open the remote environment in vscode. The c++
and docker extensions are automatically be loaded.


## Debugging/testing

```
# Write test functions in sfincs_bmi_test.cxx
cd src
g++ -o test.out sfincs_bmi_test.cxx sfincs_bmi.cxx
./test.out
```

Note: Got issues like described here https://stackoverflow.com/a/33395489
Solved by adding
```
#include <string>
#include <vector>
```
to /opt/bmi-css/bmi.hxx and recompiling with
```
cd /opt/bmi-cxx/build
make .. && make install
```
