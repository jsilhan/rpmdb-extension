file(GLOB swdb_srcs ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

ADD_LIBRARY(libswdb SHARED ${swdb_srcs})
SET_TARGET_PROPERTIES(libswdb PROPERTIES OUTPUT_NAME "swdb")
SET_TARGET_PROPERTIES(libswdb PROPERTIES SOVERSION 0)

# sqlite3
INCLUDE(FindSqlite3)
INCLUDE_DIRECTORIES(${SQLITE3_INCLUDE_DIR})
TARGET_LINK_LIBRARIES(libswdb ${SQLITE3_LIBRARIES})

set(swdb_headers
    swdb.hpp
    record.hpp
    query.hpp)

IF (CMAKE_SIZEOF_VOID_P MATCHES "8")
  SET (LIB_SUFFIX "64")
ENDIF ()

SET (LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}")

INSTALL(FILES ${swdb_headers} DESTINATION include/swdb)
INSTALL(TARGETS libswdb LIBRARY DESTINATION ${LIB_INSTALL_DIR})
