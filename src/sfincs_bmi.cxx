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
#include <iostream>
#include "sfincs_bmi.hxx"

// sfincs_bmi.f90 implements/exports the following bmi functions:
extern "C" int initialize(char *c_config_file);
extern "C" int update(double dt);  // Doesn't seem to update current time
extern "C" int finalize();
extern "C" void get_start_time(double *tstart);
extern "C" void get_end_time(double *tend);
extern "C" void get_time_step(double *dt);
extern "C" void get_current_time(double *tcurrent);
extern "C" void get_var_type(const char *c_var_name, char *c_type);

// These function are also exported but not in the BMI spec
extern "C" void get_var(const char *c_var_name, void *x);  // not exported; should be get_value?
extern "C" void set_var(const char *c_var_name, float *xptr);  // should be set get_value?
extern "C" void get_var_shape(const char *c_var_name, int *var_shape); // should be get_grid_shape
// extern "C" void get_var_rank(char *c_var_name, int *rank); // should be get_grid_rank

// The get_grid_* functions should accept grid id as first argument
extern "C" void get_grid_type(char *c_type);
extern "C" void get_grid_rank(int *rank);
extern "C" void get_grid_size(int *size);
extern "C" void get_grid_x(double *x);
extern "C" void get_grid_y(double *y);


// Model control functions.
void SfincsBmi::Initialize(std::string config_file) {
  // Convert c-string to character array
  const int length = config_file.length();
  char* c_config_file = new char[length + 1];
  strcpy(c_config_file, config_file.c_str());
  initialize(c_config_file);
  delete[] c_config_file;
}
void SfincsBmi::Update() {
  double dt = this->GetTimeStep();
  dt = 60.;  // The initial time step is super small (1e-6)
  // TODO make configurable? Sfincs uses adaptive time step internally.
  // Let's use a default "BMI user time step" of 60 seconds for now.
  int status = update(dt);
  if (status != 0) {
    throw BmiError();
  }
}
void SfincsBmi::UpdateUntil(double time) {
  double t = this->GetCurrentTime();
  while (t < time) {
    this->Update();
    t = this->GetCurrentTime();
  };
}
void SfincsBmi::Finalize() {
  int status = finalize();
  if (status != 0) {
    throw BmiError();
  }
}

// Model information functions.
std::string SfincsBmi::GetComponentName() {
  return "Sfincs hydrodynamic model (C)";
}
int SfincsBmi::GetInputItemCount() {
  return this->GetInputVarNames().size();
}
int SfincsBmi::GetOutputItemCount() {
  return this->GetOutputVarNames().size();
}
std::vector<std::string> SfincsBmi::GetInputVarNames() {
  // TODO: get from fortran 
  return {
    "zs", "zb", "qtsrc", "zst_bnd"
  };  
}
std::vector<std::string> SfincsBmi::GetOutputVarNames() {
  // TODO: get from fortran 
  return {
    "z_xz", "z_yz", "zs", "zb", "qtsrc", "zst_bnd"
  };
}

// Variable information functions
int SfincsBmi::GetVarGrid(std::string name) {
  // TODO get from fortran
  return 0;
}
std::string SfincsBmi::GetVarType(std::string name) {
  char c_type[6]; // Always returns float
  get_var_type(name.c_str(), c_type);
  std::string type_name(c_type);
  return type_name;
}
std::string SfincsBmi::GetVarUnits(std::string name) {
  // TODO get from fortran
  // Units from https://sfincs.readthedocs.io/en/latest/output.html#output-description
  if (name == "zs" || name == "zb") {
    return "m above reference level";
  }
  // TODO others
  return "";
}
int SfincsBmi::GetVarItemsize(std::string name) {
  // TODO get from fortran
  return sizeof(float);
}
int SfincsBmi::GetVarNbytes(std::string name) {
  int itemsize = this->GetVarItemsize(name);
  int gridsize = this->GetGridSize(this->GetVarGrid(name));
  return itemsize * gridsize;
}
std::string SfincsBmi::GetVarLocation(std::string name) {
  // TODO get from fortran
  return "node";
}

double SfincsBmi::GetCurrentTime() {
  double t;
  get_current_time(&t);
  return t;
}
double SfincsBmi::GetStartTime() {
  double starttime;
  get_start_time(&starttime);
  return starttime;
}
double SfincsBmi::GetEndTime() {
  double endtime;
  get_end_time(&endtime);
  return endtime;
}
std::string SfincsBmi::GetTimeUnits() {
  // same as unit for dthisout parameter 
  // at https://sfincs.readthedocs.io/en/latest/parameters.html
  return "s";
}
double SfincsBmi::GetTimeStep() {
  double dt;
  get_time_step(&dt);
  return dt;
}

// Variable getters
void SfincsBmi::GetValue(std::string name, void *dest) {
  get_var(name.c_str(), dest);
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
  set_var(name.c_str(), (float *) src);
}
void SfincsBmi::SetValueAtIndices(std::string name, int *inds, int count,
                                  void *src) {
  // TODO: implement
  throw NotImplemented();
}

// Grid information functions
int SfincsBmi::GetGridRank(const int grid) {
  int rank;
  get_grid_rank(&rank);
  return rank;
}
int SfincsBmi::GetGridSize(const int grid) {
  int size;
  get_grid_size(&size);
  return size;
}
std::string SfincsBmi::GetGridType(const int grid) {
  char c_type[13]; // Always returns unstructered or rectilinear
  get_grid_type(c_type);
  std::string type_name(c_type);
  return type_name;
}

void SfincsBmi::GetGridShape(const int grid, int *shape) {
  // TODO get shape based on grid id instead of hardcoded variable name
  get_var_shape("zs", shape);
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
  std::cout << "GetGridX was called" << std::endl;
  get_grid_x(x);
}
void SfincsBmi::GetGridY(const int grid, double *y) {
  get_grid_y(y);
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
