set(MODULE io)
set(BASE "${CMAKE_CURRENT_SOURCE_DIR}")

set(
    SOURCES
    "${BASE}/binary.cpp"
    "${BASE}/io.cpp"
    "${BASE}/laszip.cpp"
    "${BASE}/zstandard.cpp"
)

set(
    HEADERS
    "${BASE}/binary.hpp"
    "${BASE}/io.hpp"
    "${BASE}/laszip.hpp"
    "${BASE}/zstandard.hpp"
)

install(FILES ${HEADERS} DESTINATION include/entwine/${MODULE})
add_library(${MODULE} OBJECT ${SOURCES})
compiler_options(${MODULE})
