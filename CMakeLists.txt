cmake_minimum_required(VERSION 3.10)
project(optHirrygated)

# Configurações gerais
set(CMAKE_CXX_STANDARD 17)
set(SRC_DIR src)
set(INCLUDE_DIR include)

# Diretórios do CPLEX/Concert
set(SYSTEM x86-64_linux)
set(LIBFORMAT static_pic)
set(CPLEXDIR /opt/ibm/ILOG/CPLEX_Studio2211/cplex)
set(CONCERTDIR /opt/ibm/ILOG/CPLEX_Studio2211/concert)
set(CPLEX_INCLUDE_DIR "${CPLEXDIR}/include")
set(CONCERT_INCLUDE_DIR "${CONCERTDIR}/include")
set(CPLEX_LIB_DIR "${CPLEXDIR}/lib/${SYSTEM}/${LIBFORMAT}")
set(CONCERT_LIB_DIR "${CONCERTDIR}/lib/${SYSTEM}/${LIBFORMAT}")

# Subdiretórios externos
add_subdirectory(lib/OpenXLSX)

# Include directories
include_directories(
        ${INCLUDE_DIR}
        lib/OpenXLSX/include
        ${CPLEX_INCLUDE_DIR}
        ${CONCERT_INCLUDE_DIR}
)

# Link directories
link_directories(
        #${CPLEX_LIB_DIR}
        #${CONCERT_LIB_DIR}
)

# Define macro padrão CPLEX
add_definitions(-DIL_STD)

# Fonte principal
# Gera lista de todos os .cpp
file(GLOB_RECURSE ALL_SOURCES "${SRC_DIR}/*.cpp")

# Cria lista filtrada ignorando arquivos Exact*
set(SOURCES "")
foreach(F ${ALL_SOURCES})
    get_filename_component(NAME ${F} NAME_WE)
    if(NOT NAME MATCHES "^Exact")  # ignora Exact.cpp
        list(APPEND SOURCES ${F})
    endif()
endforeach()

# Cria o executável
add_executable(${PROJECT_NAME} ${SOURCES})

# Link das bibliotecas
target_link_libraries(${PROJECT_NAME}
        OpenXLSX
        #ilocplex
        #cplex
        #concert
        pthread
        dl
)