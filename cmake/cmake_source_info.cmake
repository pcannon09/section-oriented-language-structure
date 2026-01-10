# Directory Setup
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(TESTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests")
set(INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/inc/${projectName}")
set(VENDOR_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor")

# Source Files
set(SOLS_CORE_HEADERS_LIBS
		${INC_DIR}/SOLSpredefines.h

		${INC_DIR}/core/SOLS_callSystemCmd.h
		${INC_DIR}/core/SOLS_parserConf.hpp

		${INC_DIR}/core/wesi/WESI.h
)

set(SOLS_CORE_SOURCES_LIBS
		${SRC_DIR}/core/SOLS_callSystemCmd.c
		${SRC_DIR}/core/SOLS_parserConf.cpp

		${SRC_DIR}/core/wesi/WESI.c
)

set(SOLS_LIB_HEADERS_LIBS
		${INC_DIR}/lib/SOLSParser.hpp
)

set(SOLS_LIB_SOURCES_LIBS
		${SRC_DIR}/lib/SOLSParser.cpp
)

set(SOLS_SOURCES
		${SRC_DIR}/main.cpp
)

