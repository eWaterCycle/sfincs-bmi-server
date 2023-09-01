#ifndef SFINCS_BMI_INCLUDED
#define SFINCS_BMI_INCLUDED

#include <string>
#include <vector> // TODO: vector not imported in bmi-heat.hxx, is that a bug?
#include <stdexcept>
#include <bmi.hxx> // requires https://github.com/csdms/bmi-cxx

class NotImplemented: public std::logic_error {
public:
  NotImplemented() : std::logic_error("Not Implemented"){};
};

class BmiError: public std::logic_error {
public:
  BmiError() : std::logic_error("Unexpected result from model"){};
};

class SfincsBmi: public bmi::Bmi {
public:
  // Model control functions.
  void Initialize(std::string config_file) override;
  void Update() override;
  void UpdateUntil(double time) override;
  void Finalize() override;

  // Model information functions.
  std::string GetComponentName() override;
  int GetInputItemCount() override;
  int GetOutputItemCount() override;
  std::vector<std::string> GetInputVarNames() override;
  std::vector<std::string> GetOutputVarNames() override;

  // Variable information functions
  int GetVarGrid(std::string name) override;
  std::string GetVarType(std::string name) override;
  std::string GetVarUnits(std::string name) override;
  int GetVarItemsize(std::string name) override;
  int GetVarNbytes(std::string name) override;
  std::string GetVarLocation(std::string name) override;

  double GetCurrentTime() override;
  double GetStartTime() override;
  double GetEndTime() override;
  std::string GetTimeUnits() override;
  double GetTimeStep() override;

  // Variable getters
  void GetValue(std::string name, void *dest) override;
  void *GetValuePtr(std::string name) override;
  void GetValueAtIndices(std::string name, void *dest, int *inds,
                         int count) override;

  // Variable setters
  void SetValue(std::string name, void *src) override;
  void SetValueAtIndices(std::string name, int *inds, int count,
                         void *src) override;

  // Grid information functions
  int GetGridRank(const int grid) override;
  int GetGridSize(const int grid) override;
  std::string GetGridType(const int grid) override;

  void GetGridShape(const int grid, int *shape) override;
  void GetGridSpacing(const int grid, double *spacing) override;
  void GetGridOrigin(const int grid, double *origin) override;

  void GetGridX(const int grid, double *x) override;
  void GetGridY(const int grid, double *y) override;
  void GetGridZ(const int grid, double *z) override;

  int GetGridNodeCount(const int grid) override;
  int GetGridEdgeCount(const int grid) override;
  int GetGridFaceCount(const int grid) override;

  void GetGridEdgeNodes(const int grid, int *edge_nodes) override;
  void GetGridFaceEdges(const int grid, int *face_edges) override;
  void GetGridFaceNodes(const int grid, int *face_nodes) override;
  void GetGridNodesPerFace(const int grid, int *nodes_per_face) override;
};

#endif
