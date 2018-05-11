cmake_minimum_required(VERSION 3.5)

project(ShieldServer)

set(LIB_FILES
	src/shieldsql.c
)

add_library(shieldsql
	SHARED
	${LIB_FILES}
)

target_include_directories(shieldsql
	PUBLIC
	${PROJECT_SOURCE_DIR}/include
)

set(SRC_FILES
	src/server.c
	src/shield_main.c
)

add_compile_options(-W -Wall)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME}
	PUBLIC
	${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
	PUBLIC
	shieldsql
	-lm -lpthread -lmysqlclient
)
