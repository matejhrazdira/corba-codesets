cmake_minimum_required(VERSION 3.1)

project(corba-codesets)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin/")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "bin/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "bin/")

# "common" library
file(GLOB IDLS "./src/idl/*.idl")
foreach(GENERATE_IDL ${IDLS})
	get_filename_component(IDLNAME ${GENERATE_IDL} NAME_WE)
	set(OUT_NAME src/gen/${IDLNAME})
	list(APPEND IDL_COMPILED_FILES ${OUT_NAME}C.h ${OUT_NAME}C.cpp ${OUT_NAME}S.h ${OUT_NAME}S.cpp)
	add_custom_command(
		OUTPUT ${OUT_NAME}C.h ${OUT_NAME}C.cpp ${OUT_NAME}S.h ${OUT_NAME}S.cpp
		COMMAND tao_idl -Cw -o ./src/gen/ ${GENERATE_IDL}
		COMMENT "Processing ${GENERATE_IDL}"
	)
endforeach(GENERATE_IDL)
set_source_files_properties(${IDL_COMPILED_FILES} PROPERTIES GENERATED TRUE)
file(GLOB COMMON_SRC "./src/common/*.cpp")
add_library(common ${COMMON_SRC} ${IDL_COMPILED_FILES})

# CP850 to ISO8859-1 conversion library
file(GLOB CP850_FACTORY "./src/cp850factory/*.cpp")
add_library(IBM850_ISO8859 SHARED ${CP850_FACTORY})

# dependencies
set(CORBA_LIBS TAO_CosNaming TAO_RTEvent_Serv TAO_RTEvent_Skel TAO_RTEvent TAO_Svc_Utils TAO_Messaging TAO_PI TAO_CodecFactory TAO_Valuetype TAO_PortableServer TAO_AnyTypeCode TAO ACE)
set(ALL_LIBS common ${CORBA_LIBS})

# client
file(GLOB CLIENT_SRC "./src/client/*.cpp")
add_executable(client ${CLIENT_SRC})
target_link_libraries(client ${ALL_LIBS})

# server
file(GLOB SERVER_SRC "./src/server/*.cpp")
add_executable(server ${SERVER_SRC})
target_link_libraries(server ${ALL_LIBS})
