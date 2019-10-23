# CMake generated Testfile for 
# Source directory: /Users/Luke/Development/FEL/pjc-02
# Build directory: /Users/Luke/Development/FEL/pjc-02/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(direct "/Users/Luke/Development/FEL/pjc-02/build/tests-direct")
add_test(runner-direct "/Users/Luke/Development/FEL/pjc-02/build/tests-runner-direct")
add_test(runner-subprocess "/Users/Luke/Development/FEL/pjc-02/build/tests-runner-subprocess")
set_tests_properties(runner-subprocess PROPERTIES  ENVIRONMENT "TEST_CMD=\"./symcalc\"")
