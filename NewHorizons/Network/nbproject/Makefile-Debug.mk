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
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/Src/NHOAckMessage.o \
	${OBJECTDIR}/Src/NHOBroadcastEmitter.o \
	${OBJECTDIR}/Src/NHOCameraDataMessage.o \
	${OBJECTDIR}/Src/NHOEmitter.o \
	${OBJECTDIR}/Src/NHOFullDuplexConnectedEmitter.o \
	${OBJECTDIR}/Src/NHOHEMMessage.o \
	${OBJECTDIR}/Src/NHOImageSizeMessage.o \
	${OBJECTDIR}/Src/NHOMessage.o \
	${OBJECTDIR}/Src/NHOMessageFactory.o \
	${OBJECTDIR}/Src/NHOReflexFullDuplexConnectedEmitter.o


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
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libnetwork.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libnetwork.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libnetwork.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libnetwork.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libnetwork.a

${OBJECTDIR}/Src/NHOAckMessage.o: Src/NHOAckMessage.cpp
	${MKDIR} -p ${OBJECTDIR}/Src
	${RM} "$@.d"
	$(COMPILE.cc) -g -D_RASPBIAN -IInc -I../Sensor/Inc -I../Utils/Inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Src/NHOAckMessage.o Src/NHOAckMessage.cpp

${OBJECTDIR}/Src/NHOBroadcastEmitter.o: Src/NHOBroadcastEmitter.cpp
	${MKDIR} -p ${OBJECTDIR}/Src
	${RM} "$@.d"
	$(COMPILE.cc) -g -D_RASPBIAN -IInc -I../Sensor/Inc -I../Utils/Inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Src/NHOBroadcastEmitter.o Src/NHOBroadcastEmitter.cpp

${OBJECTDIR}/Src/NHOCameraDataMessage.o: Src/NHOCameraDataMessage.cpp
	${MKDIR} -p ${OBJECTDIR}/Src
	${RM} "$@.d"
	$(COMPILE.cc) -g -D_RASPBIAN -IInc -I../Sensor/Inc -I../Utils/Inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Src/NHOCameraDataMessage.o Src/NHOCameraDataMessage.cpp

${OBJECTDIR}/Src/NHOEmitter.o: Src/NHOEmitter.cpp
	${MKDIR} -p ${OBJECTDIR}/Src
	${RM} "$@.d"
	$(COMPILE.cc) -g -D_RASPBIAN -IInc -I../Sensor/Inc -I../Utils/Inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Src/NHOEmitter.o Src/NHOEmitter.cpp

${OBJECTDIR}/Src/NHOFullDuplexConnectedEmitter.o: Src/NHOFullDuplexConnectedEmitter.cpp
	${MKDIR} -p ${OBJECTDIR}/Src
	${RM} "$@.d"
	$(COMPILE.cc) -g -D_RASPBIAN -IInc -I../Sensor/Inc -I../Utils/Inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Src/NHOFullDuplexConnectedEmitter.o Src/NHOFullDuplexConnectedEmitter.cpp

${OBJECTDIR}/Src/NHOHEMMessage.o: Src/NHOHEMMessage.cpp
	${MKDIR} -p ${OBJECTDIR}/Src
	${RM} "$@.d"
	$(COMPILE.cc) -g -D_RASPBIAN -IInc -I../Sensor/Inc -I../Utils/Inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Src/NHOHEMMessage.o Src/NHOHEMMessage.cpp

${OBJECTDIR}/Src/NHOImageSizeMessage.o: Src/NHOImageSizeMessage.cpp
	${MKDIR} -p ${OBJECTDIR}/Src
	${RM} "$@.d"
	$(COMPILE.cc) -g -D_RASPBIAN -IInc -I../Sensor/Inc -I../Utils/Inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Src/NHOImageSizeMessage.o Src/NHOImageSizeMessage.cpp

${OBJECTDIR}/Src/NHOMessage.o: Src/NHOMessage.cpp
	${MKDIR} -p ${OBJECTDIR}/Src
	${RM} "$@.d"
	$(COMPILE.cc) -g -D_RASPBIAN -IInc -I../Sensor/Inc -I../Utils/Inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Src/NHOMessage.o Src/NHOMessage.cpp

${OBJECTDIR}/Src/NHOMessageFactory.o: Src/NHOMessageFactory.cpp
	${MKDIR} -p ${OBJECTDIR}/Src
	${RM} "$@.d"
	$(COMPILE.cc) -g -D_RASPBIAN -IInc -I../Sensor/Inc -I../Utils/Inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Src/NHOMessageFactory.o Src/NHOMessageFactory.cpp

${OBJECTDIR}/Src/NHOReflexFullDuplexConnectedEmitter.o: Src/NHOReflexFullDuplexConnectedEmitter.cpp
	${MKDIR} -p ${OBJECTDIR}/Src
	${RM} "$@.d"
	$(COMPILE.cc) -g -D_RASPBIAN -IInc -I../Sensor/Inc -I../Utils/Inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Src/NHOReflexFullDuplexConnectedEmitter.o Src/NHOReflexFullDuplexConnectedEmitter.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
