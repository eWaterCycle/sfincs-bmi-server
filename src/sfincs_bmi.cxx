// C++ Wrapper around sfincs-bmi fortran code
// See https://grpc4bmi.readthedocs.io/en/latest/server/Cpp.html#c-c-fortran
// See https://github.com/Deltares/SFINCS/tree/main
//
// Implemented after bmi-heat c++ example, version 2.1:
// https://github.com/csdms/bmi-example-cxx/blob/2ede037a29e34d66c274768caad1a891a70c7897/heat/bmi_heat.cxx

// SFINCS BMI implementation here:
// https://github.com/Deltares/SFINCS/blob/main/source/src/sfincs_bmi.f90
// Note: get_var not exported but it is actually implemented

#include <cstring>
#include "sfincs_bmi.hxx"

// // sfincs_bmi.f90 implements/exports the following bmi functions:
// extern "C" int initialize();
// extern "C" int update(double dt);
// extern "C" int finalize();
// // extern "C" void get_var_shape(char *c_var_name, int *var_shape);  // Not a
// // BMI function
// extern "C" void get_var_type(char *c_var_name, char *c_type);
// // extern "C" void get_var(char *c_var_name, real *x);  // Not declared as
// // public in fortran code
// extern "C" void get_var_rank(char *c_var_name, int *rank);
// extern "C" void set_var(char *c_var_name, float *xptr);
// extern "C" void get_start_time(double *tstart);
// extern "C" void get_end_time(double *tend);
// extern "C" void get_time_step(double *deltat);
// extern "C" void get_current_time(double *tcurrent);

// Model control functions.
void SfincsBmi::Initialize(std::string config_file) {
  // TODO: implement
  throw NotImplemented();
}
void SfincsBmi::Update() {
  // // TODO: get dt from model parameters
  // int status = update(1.);
  // if (status != 0) {
  //   throw BmiError();
  // }
  throw NotImplemented();
}
void SfincsBmi::UpdateUntil(double time) {
  // TODO: implement
  throw NotImplemented();
}
void SfincsBmi::Finalize() {
  // TODO: implement
  throw NotImplemented();
}

// Model information functions.
std::string SfincsBmi::GetComponentName() {
  return "Sfincs hydrodynamic model (C)";
}
int SfincsBmi::GetInputItemCount() {
  // TODO: implement
  throw NotImplemented();
}
int SfincsBmi::GetOutputItemCount() {
  // TODO: implement
  throw NotImplemented();
}
std::vector<std::string> SfincsBmi::GetInputVarNames() {
  // TODO: implement
  throw NotImplemented();
}
std::vector<std::string> SfincsBmi::GetOutputVarNames() {
  // TODO: implement
  throw NotImplemented();
}

// Variable information functions
int SfincsBmi::GetVarGrid(std::string name) {
  // TODO: implement
  throw NotImplemented();
}
std::string SfincsBmi::GetVarType(std::string name) {
  // TODO: implement
  throw NotImplemented();
}
std::string SfincsBmi::GetVarUnits(std::string name) {
  // TODO: implement
  throw NotImplemented();
}
int SfincsBmi::GetVarItemsize(std::string name) {
  // TODO: implement
  throw NotImplemented();
}
int SfincsBmi::GetVarNbytes(std::string name) {
  // TODO: implement
  throw NotImplemented();
}
std::string SfincsBmi::GetVarLocation(std::string name) {
  // TODO: implement
  throw NotImplemented();
}

double SfincsBmi::GetCurrentTime() {
  // TODO: implement
  throw NotImplemented();
}
double SfincsBmi::GetStartTime() {
  // TODO: implement
  throw NotImplemented();
}
double SfincsBmi::GetEndTime() {
  // TODO: implement
  throw NotImplemented();
}
std::string SfincsBmi::GetTimeUnits() {
  // TODO: implement
  throw NotImplemented();
}
double SfincsBmi::GetTimeStep() {
  // TODO: implement
  throw NotImplemented();
}

// Variable getters
void SfincsBmi::GetValue(std::string name, void *dest) {
  // TODO: implement
  throw NotImplemented();
}
void *SfincsBmi::GetValuePtr(std::string name) {
  // TODO: implement
  throw NotImplemented();
}
void SfincsBmi::GetValueAtIndices(std::string name, void *dest, int *inds,
                                  int count) {
  // TODO: implement
  throw NotImplemented();
}

// Variable setters
void SfincsBmi::SetValue(std::string name, void *src) {
  // TODO: implement
  throw NotImplemented();
}
void SfincsBmi::SetValueAtIndices(std::string name, int *inds, int count,
                                  void *src) {
  // TODO: implement
  throw NotImplemented();
}

// Grid information functions
int SfincsBmi::GetGridRank(const int grid) {
  // TODO: implement
  throw NotImplemented();
}
int SfincsBmi::GetGridSize(const int grid) {
  // TODO: implement
  throw NotImplemented();
}
std::string SfincsBmi::GetGridType(const int grid) {
  // TODO: implement
  throw NotImplemented();
}

void SfincsBmi::GetGridShape(const int grid, int *shape) {
  // TODO: implement
  throw NotImplemented();
}
void SfincsBmi::GetGridSpacing(const int grid, double *spacing) {
  // TODO: implement
  throw NotImplemented();
}
void SfincsBmi::GetGridOrigin(const int grid, double *origin) {
  // TODO: implement
  throw NotImplemented();
}

void SfincsBmi::GetGridX(const int grid, double *x) {
  // TODO: implement
  throw NotImplemented();
}
void SfincsBmi::GetGridY(const int grid, double *y) {
  // TODO: implement
  throw NotImplemented();
}
void SfincsBmi::GetGridZ(const int grid, double *z) {
  // TODO: implement
  throw NotImplemented();
}

int SfincsBmi::GetGridNodeCount(const int grid) {
  // TODO: implement
  throw NotImplemented();
}
int SfincsBmi::GetGridEdgeCount(const int grid) {
  // TODO: implement
  throw NotImplemented();
}
int SfincsBmi::GetGridFaceCount(const int grid) {
  // TODO: implement
  throw NotImplemented();
}

void SfincsBmi::GetGridEdgeNodes(const int grid, int *edge_nodes) {
  // TODO: implement
  throw NotImplemented();
}
void SfincsBmi::GetGridFaceEdges(const int grid, int *face_edges) {
  // TODO: implement
  throw NotImplemented();
}
void SfincsBmi::GetGridFaceNodes(const int grid, int *face_nodes) {
  // TODO: implement
  throw NotImplemented();
}
void SfincsBmi::GetGridNodesPerFace(const int grid, int *nodes_per_face) {
  // TODO: implement
  throw NotImplemented();
}
