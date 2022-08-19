# setup project information
project('phys-units', 'cpp', default_options: ['cpp_std=c++20'])
# setup base include directory
include_dir = include_directories('include')
# add subdirectories
subdir('tests')
# configure inlude directory to be installed
install_subdir('include', install_dir: 'include', strip_directory: true)