# Get list of sources.
file(GLOB_RECURSE SOURCES "*.cpp")

set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)

get_filename_component(binary_name ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# Add the executable.
add_executable("main_${binary_name}" ${SOURCES})
set_target_properties("main_${binary_name}" PROPERTIES OUTPUT_NAME "${binary_name}")

target_compile_features("main_${binary_name}" PRIVATE cxx_std_17)
target_link_libraries("main_${binary_name}" PRIVATE OpenSSL::Crypto)

# Install application.
install(TARGETS "main_${binary_name}" DESTINATION "bin")
