project(dwappversions)

add_executable(dwappversions main.cpp)

find_library(LIBBOOSTFILESYSTEMA NAMES libboost_filesystem.a)
find_library(LIBBOOSTSYSTEMA NAMES libboost_system.a)

target_link_libraries(dwappversions ${LIBBOOSTSYSTEMA} ${LIBBOOSTFILESYSTEMA})

install(TARGETS dwappversions RUNTIME DESTINATION bin)
