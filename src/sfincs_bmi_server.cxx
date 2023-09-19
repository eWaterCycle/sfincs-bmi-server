#include "bmi_grpc_server.h"
#include <bmi.hxx>
#include "sfincs_bmi.hxx"

int main(int argc, char *argv[]) {
  bmi::Bmi *model = new SfincsBmi();
  run_bmi_server(model, argc, argv);
  delete model;
  return 0;
}
