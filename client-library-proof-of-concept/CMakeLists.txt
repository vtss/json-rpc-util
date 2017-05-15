CMAKE_MINIMUM_REQUIRED(VERSION 2.8.5)
PROJECT(vtss-json-rpc-client-library)


OPTION(JSONCPP_WITH_TESTS "Compile and (for jsoncpp_check) run JsonCpp test executables" OFF)
OPTION(JSONCPP_WITH_POST_BUILD_UNITTEST "Automatically run unit-tests as a post build step" OFF)
OPTION(JSONCPP_WITH_WARNING_AS_ERROR "Force compilation to fail if a warning occurs" OFF)
OPTION(JSONCPP_WITH_PKGCONFIG_SUPPORT "Generate and install .pc files" OFF)
OPTION(JSONCPP_WITH_CMAKE_PACKAGE "Generate and install cmake package files" OFF)

add_definitions(-std=c++11 -Wall -fmax-errors=5)
ADD_SUBDIRECTORY(./jsoncpp/)

INCLUDE_DIRECTORIES(./jsoncpp/include/)
INCLUDE_DIRECTORIES(./restclient-cpp/include/)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

add_library(restclient-cpp ./restclient-cpp/source/restclient.cpp)

# ADD_CUSTOM_COMMAND(
#     OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/vtss-client.cxx ${CMAKE_CURRENT_BINARY_DIR}/vtss-client.hxx
#     COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/vtss-json-spec-autogen ${CMAKE_CURRENT_SOURCE_DIR}/vtss-json-rpc-example.spec
#     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/vtss-json-spec-autogen ${CMAKE_CURRENT_SOURCE_DIR}/vtss-json-rpc-example.spec
# )

ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/vtss-client.cxx ${CMAKE_CURRENT_BINARY_DIR}/vtss-client.hxx
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/vtss-json-spec-autogen ${CMAKE_CURRENT_SOURCE_DIR}/Serval-TxRefNID-json.spec
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/vtss-json-spec-autogen ${CMAKE_CURRENT_SOURCE_DIR}/Serval-TxRefNID-json.spec
)


add_library(vtss-client SHARED ${CMAKE_CURRENT_BINARY_DIR}/vtss-client.cxx)
#target_link_libraries(vtss-client restclient-cpp jsoncpp_lib_static curl)

add_executable(example-raw-json example-raw-json.cxx)
target_link_libraries(example-raw-json restclient-cpp jsoncpp_lib_static curl)

add_executable(example-vtss-client-lib example-vtss-client-lib.cxx)
target_link_libraries(example-vtss-client-lib vtss-client restclient-cpp jsoncpp_lib_static curl)

