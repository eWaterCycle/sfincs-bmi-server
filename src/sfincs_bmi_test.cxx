#include <bmi.hxx>
#include "sfincs_bmi.hxx"
#include <iostream>
#include <string>

int main() {
    // Test instantiate model
    bmi::Bmi *model = new SfincsBmi();

    // Test get component name
    std::string name = "Sfincs hydrodynamic model (C)";
    if (name == model->GetComponentName()){
        std::cout << "pass\n";
    }

    // Test initialize
    std::string config_file = "sfincs.inp";
    model->Initialize(config_file);

    return 0;
}
