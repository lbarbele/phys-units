gtest = dependency('gtest')

test_rational = executable(
  'rational', 'rational.cpp',
  include_directories: include_dir,
  install: false,
  dependencies: gtest)

test_units = executable(
  'units', 'units.cpp',
  include_directories: include_dir,
  install: false)

test('rational', test_rational)
test('units', test_units)