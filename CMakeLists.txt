cmake_minimum_required(VERSION 3.5)

project(ShieldServer)

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

target_link_libraries(${PROJECT_NAME}
	PUBLIC
	shieldsql
	-lm -lpthread -lmysqlclient
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
