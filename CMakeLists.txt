cmake_minimum_required(VERSION 3.5)

project(shieldserver)

add_compile_options(-W -Wall)

# Set include directories for library and binary
include_directories(PUBLIC
	${PROJECT_SOURCE_DIR}/include
)

# Create library
set(LIB_FILES
	src/shieldsql.c
)

add_library(shieldsql
	SHARED
	${LIB_FILES}
)

# Create binary
set(SRC_FILES
	src/server.c
	src/shield_main.c
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

include(FindPkgConfig)
pkg_check_modules(PKGS REQUIRED mysqlclient)

target_link_libraries(${PROJECT_NAME}
	PUBLIC
	shieldsql
	-lm -lpthread ${PKGS_LIBRARIES}
)

# Setup install
# Binary
install(TARGETS ${PROJECT_NAME}
	DESTINATION bin
)

# Library
install(TARGETS shieldsql
	LIBRARY DESTINATION lib
)

# Create package
set(DEB_VERSION_MAJOR 1)
set(DEB_VERSION_MINOR 0)
set(DEB_VERSION "${DEB_VERSION_MAJOR}.${DEB_VERSION_MINOR}")

# Tell CPack to generate a .deb package
set(CPACK_GENERATOR "DEB")

# Set package install path
set(CPACK_PACKAGING_INSTALL_PREFIX /usr/local)

# Set an author
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Hoegeun Kwon")

# Set a Package Version
set(CPACK_PACKAGE_VERSION ${DEB_VERSION})

# Include CPack
include(CPack)
