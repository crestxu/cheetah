#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-MacOSX
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/43898991/request_parser.o \
	${OBJECTDIR}/_ext/43898991/pub_sub_manager.o \
	${OBJECTDIR}/_ext/43898991/connection_manager.o \
	${OBJECTDIR}/_ext/43898991/response.o \
	${OBJECTDIR}/_ext/43898991/main.o \
	${OBJECTDIR}/_ext/43898991/token_handler.o \
	${OBJECTDIR}/_ext/43898991/server_connection.o \
	${OBJECTDIR}/_ext/43898991/http_service.o \
	${OBJECTDIR}/_ext/43898991/db_utils.o \
	${OBJECTDIR}/_ext/43898991/logger.o \
	${OBJECTDIR}/_ext/43898991/stream_exception.o \
	${OBJECTDIR}/_ext/43898991/token_monitor.o \
	${OBJECTDIR}/_ext/43898991/server_session.o \
	${OBJECTDIR}/_ext/43898991/request_body.o \
	${OBJECTDIR}/_ext/43898991/stream.o \
	${OBJECTDIR}/_ext/43898991/request.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L/opt/local/lib -lboost_filesystem-mt -lboost_program_options-mt -lboost_system-mt -lmysqlcppconn-static

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/cheetah

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/cheetah: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/cheetah ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/43898991/request_parser.o: ../../request_parser.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/43898991
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/opt/local/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/43898991/request_parser.o ../../request_parser.cpp

${OBJECTDIR}/_ext/43898991/pub_sub_manager.o: ../../pub_sub_manager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/43898991
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/opt/local/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/43898991/pub_sub_manager.o ../../pub_sub_manager.cpp

${OBJECTDIR}/_ext/43898991/connection_manager.o: ../../connection_manager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/43898991
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/opt/local/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/43898991/connection_manager.o ../../connection_manager.cpp

${OBJECTDIR}/_ext/43898991/response.o: ../../response.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/43898991
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/opt/local/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/43898991/response.o ../../response.cpp

${OBJECTDIR}/_ext/43898991/main.o: ../../main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/43898991
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/opt/local/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/43898991/main.o ../../main.cpp

${OBJECTDIR}/_ext/43898991/token_handler.o: ../../token_handler.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/43898991
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/opt/local/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/43898991/token_handler.o ../../token_handler.cpp

${OBJECTDIR}/_ext/43898991/server_connection.o: ../../server_connection.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/43898991
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/opt/local/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/43898991/server_connection.o ../../server_connection.cpp

${OBJECTDIR}/_ext/43898991/http_service.o: ../../http_service.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/43898991
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/opt/local/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/43898991/http_service.o ../../http_service.cpp

${OBJECTDIR}/_ext/43898991/db_utils.o: ../../db_utils.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/43898991
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/opt/local/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/43898991/db_utils.o ../../db_utils.cpp

${OBJECTDIR}/_ext/43898991/logger.o: ../../logger.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/43898991
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/opt/local/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/43898991/logger.o ../../logger.cpp

${OBJECTDIR}/_ext/43898991/stream_exception.o: ../../stream_exception.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/43898991
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/opt/local/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/43898991/stream_exception.o ../../stream_exception.cpp

${OBJECTDIR}/_ext/43898991/token_monitor.o: ../../token_monitor.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/43898991
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/opt/local/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/43898991/token_monitor.o ../../token_monitor.cpp

${OBJECTDIR}/_ext/43898991/server_session.o: ../../server_session.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/43898991
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/opt/local/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/43898991/server_session.o ../../server_session.cpp

${OBJECTDIR}/_ext/43898991/request_body.o: ../../request_body.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/43898991
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/opt/local/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/43898991/request_body.o ../../request_body.cpp

${OBJECTDIR}/_ext/43898991/stream.o: ../../stream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/43898991
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/opt/local/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/43898991/stream.o ../../stream.cpp

${OBJECTDIR}/_ext/43898991/request.o: ../../request.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/43898991
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/opt/local/include -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/43898991/request.o ../../request.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/cheetah

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
