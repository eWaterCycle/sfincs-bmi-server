#include "bmi_grpc_server.h"
#include "sfincs_bmi.hxx"

int main(int argc, char *argv[]) {
  SfincsBmi *model = new SfincsBmi();
  run_bmi_server(model, argc, argv);
  delete model;
  return 0;
}
