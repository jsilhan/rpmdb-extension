FIND_PACKAGE(PythonInterp)
FIND_PACKAGE(PythonLibs)
FIND_PACKAGE(Boost COMPONENTS python)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIRS})

PYTHON_ADD_MODULE(swdb swdb-py.cpp)
target_link_libraries(swdb libswdb ${Boost_LIBRARIES})
