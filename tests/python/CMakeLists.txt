execute_process(COMMAND ${PYTHON_EXECUTABLE} -c "import sys; sys.stdout.write('%s.%s' % (sys.version_info.major, sys.version_info.minor))" OUTPUT_VARIABLE PYTHON_MAJOR_DOT_MINOR_VERSION)
CONFIGURE_FILE("run_nosetests.sh.in"  "run_nosetests.sh")
ADD_TEST(test_python sh run_nosetests.sh)
