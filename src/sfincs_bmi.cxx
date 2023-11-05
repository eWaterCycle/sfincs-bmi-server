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
extern "C" int initialize(const char *c_config_file);
extern "C" int finalize();

extern "C" int get_component_name(char *name);

extern "C" int update();
extern "C" int update_until(double t);

extern "C" int get_start_time(double *tstart);
extern "C" int get_end_time(double *tend);
extern "C" int get_time_step(double *dt);
extern "C" int get_time_units(char *units);
extern "C" int get_current_time(double *tcurrent);

extern "C" int get_var_type(const char *c_var_name, char *c_type);

// These function are also exported but not in the BMI spec
extern "C" int get_value(const char *c_var_name, void *dest, int *n);
extern "C" int get_value_at_indices(const char *name, void *dest, int *inds, int count);
extern "C" int get_value_ptr(const char *name, void *dest_ptr);

extern "C" int set_value(const char *c_var_name, float *xptr);
extern "C" int set_value_at_indices(const char *name, int *inds, int count, void *src);

extern "C" int get_var_shape(const char *c_var_name, int *var_shape); // should be get_grid_shape
// extern "C" int get_var_rank(char *c_var_name, int *rank); // should be get_grid_rank

// The get_grid_* functions should accept grid id as first argument
extern "C" int get_grid_type(char *c_type);
extern "C" int get_grid_rank(int *rank);
extern "C" int get_grid_size(int *size);
extern "C" int get_grid_x(double *x);
extern "C" int get_grid_y(double *y);

namespace
{
  std::string &rtrim(std::string &str)
  {
    str.erase(str.find_last_not_of(' ') + 1);
    return str;
  }
}

// Model control functions.
void SfincsBmi::Initialize(std::string config_file)
{
  // Convert c-string to character array
  const int length = config_file.length();
  char *c_config_file = new char[length + 1];
  strcpy(c_config_file, config_file.c_str());
  initialize(c_config_file);
  delete[] c_config_file;
}
void SfincsBmi::Update()
{
  if (update() != 0)
  {
    throw BmiError();
  }
}
void SfincsBmi::UpdateUntil(double time)
{
  update_until(time);
}
void SfincsBmi::Finalize()
{
  if (finalize() != 0)
  {
    throw BmiError();
  }
}

// Model information functions.
std::string SfincsBmi::GetComponentName()
{
  char component_name[256];
  if (get_component_name(component_name) != 0)
  {
    throw BmiError();
  }
  std::string str(component_name);
  return rtrim(str);
}
int SfincsBmi::GetInputItemCount()
{
  return this->GetInputVarNames().size();
}
int SfincsBmi::GetOutputItemCount()
{
  return this->GetOutputVarNames().size();
}
std::vector<std::string> SfincsBmi::GetInputVarNames()
{
  // TODO: get from fortran
  return {"zs",
          "zb",
          "qsrc_1",
          "qsrc_2",
          "tsrc",
          "zst_bnd"};
}
std::vector<std::string> SfincsBmi::GetOutputVarNames()
{
  // TODO: get from fortran
  return {
      "z_xz",
      "z_yz",
      "zs",
      "zb",
      "qsrc_1",
      "qsrc_2",
      "xsrc",
      "ysrc",
      "tsrc",
      "zst_bnd"};
}

// Variable information functions
int SfincsBmi::GetVarGrid(std::string name)
{
  // TODO get from fortran
  if (name == "zs"){
    return 0;
  } else if (name=="z_xz"){
    return 0;
  } else if (name=="z_yz"){
    return 0;
  } else if (name=="zb"){
    return 0;
  } else if (name=="zst_bnd"){
    return 0;
  } else if (name=="qsrc_1"){
    return 1;
  } else if (name=="qsrc_2"){
    return 1;
  } else if (name=="xsrc"){
    return 1;
  } else if (name=="ysrc"){
    return 1;
  } else if (name=="tsrc"){
    return 2;
  } else {
    throw BmiError();
  };
}
std::string SfincsBmi::GetVarType(std::string name)
{
  char c_type[6]; // Always returns float
  if (get_var_type(name.c_str(), c_type) != 0)
  {
    throw BmiError();
  }
  return "float32";
  // return std::string(c_type);
}
std::string SfincsBmi::GetVarUnits(std::string name)
{
  // TODO get from fortran
  // Units from https://sfincs.readthedocs.io/en/latest/output.html#output-description
  if (name == "zs" || name == "zb")
  {
    return "m above reference level";
  }
  // TODO others
  return "";
}
int SfincsBmi::GetVarItemsize(std::string name)
{
  // TODO get from fortran
  return sizeof(float);
}
int SfincsBmi::GetVarNbytes(std::string name)
{
  int itemsize = this->GetVarItemsize(name);
  int gridsize = this->GetGridSize(this->GetVarGrid(name));
  return itemsize * gridsize;
}
std::string SfincsBmi::GetVarLocation(std::string name)
{
  // TODO get from fortran
  return "node";
}

double SfincsBmi::GetCurrentTime()
{
  double t;
  if (get_current_time(&t) != 0)
  {
    throw BmiError();
  }
  return t;
}
double SfincsBmi::GetStartTime()
{
  double starttime;
  if (get_start_time(&starttime) != 0)
  {
    throw BmiError();
  }
  return starttime;
}
double SfincsBmi::GetEndTime()
{
  double endtime;
  if (get_end_time(&endtime) != 0)
  {
    throw BmiError();
  }
  return endtime;
}
std::string SfincsBmi::GetTimeUnits()
{
  char time_unit[2]; // Always returns "s"
  if (get_time_units(time_unit) != 0)
  {
    throw BmiError();
  }
  return std::string(time_unit);
}
double SfincsBmi::GetTimeStep()
{
  double dt;
  if (get_time_step(&dt) != 0)
  {
    throw BmiError();
  }
  return dt;
}

// Variable getters
void SfincsBmi::GetValue(std::string name, void *dest)
{
  int grid_id = this->GetVarGrid(name);
  int n = this->GetGridSize(grid_id);
  if (get_value(name.c_str(), dest, &n) != 0)
  // if (get_value_ptr(name.c_str(), dest) != 0)
  {
    throw BmiError();
  }
}
void *SfincsBmi::GetValuePtr(std::string name)
{
  // TODO: implement
  throw NotImplemented();
}
void SfincsBmi::GetValueAtIndices(std::string name, void *dest, int *inds, int count)
{
  if (get_value_at_indices(name.c_str(), dest, inds, count) != 0)
  {
    throw BmiError();
  }
}

// Variable setters
void SfincsBmi::SetValue(std::string name, void *src)
{
  if (set_value(name.c_str(), (float *)src) != 0)
  {
    throw BmiError();
  }
}
void SfincsBmi::SetValueAtIndices(std::string name, int *inds, int count, void *src)
{
  if (set_value_at_indices(name.c_str(), inds, count, (float *)src) != 0)
  {
    throw BmiError();
  }
}

// Grid information functions
int SfincsBmi::GetGridRank(const int grid)
// TODO grid id is not passed on to fortran
{
  int rank;
  get_grid_rank(&rank);
  return rank;
}
int SfincsBmi::GetGridSize(const int grid)
{
  // TODO get size based on grid id instead of variable name
  int success = 0;
  int shape[1];
  int* size = shape;
  if (grid == 0){
    success = get_var_shape("zs", shape);
  } else if (grid == 1){
    success = get_var_shape("qsrc_1", shape);
  } else if (grid == 2){
    success = get_var_shape("tsrc", shape);
  } else {
    throw BmiError();
  };
  if (success != 0){
    throw BmiError();
  }
  return *size;
}
std::string SfincsBmi::GetGridType(const int grid)
{
  char c_type[13]; // Always returns unstructered or rectilinear
  if (get_grid_type(c_type) != 0)
  {
    throw BmiError();
  }
  return std::string(c_type);
}

void SfincsBmi::GetGridShape(const int grid, int *shape)
{
  // Not in BMI spec for unstructured grids
  throw NotImplemented();
}
void SfincsBmi::GetGridSpacing(const int grid, double *spacing)
{
  // TODO: implement
  throw NotImplemented();
}
void SfincsBmi::GetGridOrigin(const int grid, double *origin)
{
  // TODO: implement
  throw NotImplemented();
}

void SfincsBmi::GetGridX(const int grid, double *x)
{
  std::cout << "GetGridX was called" << std::endl;
  if (get_grid_x(x) != 0)
  {
    throw BmiError();
  }
}
void SfincsBmi::GetGridY(const int grid, double *y)
{
  if (get_grid_y(y) != 0)
  {
    throw BmiError();
  }
}
void SfincsBmi::GetGridZ(const int grid, double *z)
{
  // TODO: implement
  throw NotImplemented();
}

int SfincsBmi::GetGridNodeCount(const int grid)
{
  // TODO: implement
  throw NotImplemented();
}
int SfincsBmi::GetGridEdgeCount(const int grid)
{
  // TODO: implement
  throw NotImplemented();
}
int SfincsBmi::GetGridFaceCount(const int grid)
{
  // TODO: implement
  throw NotImplemented();
}

void SfincsBmi::GetGridEdgeNodes(const int grid, int *edge_nodes)
{
  // TODO: implement
  throw NotImplemented();
}
void SfincsBmi::GetGridFaceEdges(const int grid, int *face_edges)
{
  // TODO: implement
  throw NotImplemented();
}
void SfincsBmi::GetGridFaceNodes(const int grid, int *face_nodes)
{
  // TODO: implement
  throw NotImplemented();
}
void SfincsBmi::GetGridNodesPerFace(const int grid, int *nodes_per_face)
{
  // TODO: implement
  throw NotImplemented();
}
