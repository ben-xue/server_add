project(zone_svr)

cmake_minimum_required(VERSION 3.10)

include(${CMAKE_SOURCE_DIR}/CMakeComm.txt)

set(EXECUTABLE_OUTPUT_PATH "${home_dir}/runenv/zone_svr/bin")

include_directories(${lib_dir}/tdr_parser)

add_src_subdirs(source_files  "./comm" "./frame" "./logic" "./attr" "./model" "../comm" "../logic_comm" "../logic_comm/base"
        "../logic_comm/iocfg" "../logic_comm/attr" "../logic_comm/torm" "../logic_comm/config" "../logic_comm/tunnel" "../logic_comm/coroutine"
        "../logic_comm/tdr_parser"  "../logic_comm/json" "../../protocol/src" "../../resource/src" )

message(STATUS "source_files:${source_files}")

add_executable(zone_svr ${source_files})
#set(lib_link  "${lib_co_home}/lib/libcolib.a" ${lib_link})
#set(lib_link  ${lib_link})
set(CMAKE_EXE_LINKER_FLAGS "${lib_link_flag}")
target_link_libraries(zone_svr ${lib_link}) 
