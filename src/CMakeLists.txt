cmake_minimum_required(VERSION 3.0)
project(sfincs_bmi_server CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(BMICXX REQUIRED IMPORTED_TARGET bmicxx)
message("--   bmicxx include - ${BMICXX_INCLUDE_DIRS}")
include_directories(${BMICXX_INCLUDE_DIRS})
pkg_check_modules(GRPC4BMI REQUIRED grpc4bmi)

add_executable(sfincs_bmi_server sfincs_bmi.cxx sfincs_bmi_server.cxx)

target_include_directories(sfincs_bmi_server PRIVATE)

target_link_libraries(sfincs_bmi_server
                      ${GRPC4BMI_LINK_LIBRARIES}
                      )

install(TARGETS sfincs_bmi_server
        RUNTIME DESTINATION bin)
