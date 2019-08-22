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
	${OBJECTDIR}/Src/NHOCCParameters.o \
	${OBJECTDIR}/Src/NHOCameraParameters.o \
	${OBJECTDIR}/Src/NHOConfiguration.o \
	${OBJECTDIR}/Src/NHOMotorParameters.o \
	${OBJECTDIR}/Src/NHORoverParameters.o \
	${OBJECTDIR}/Src/NHOSensorParameters.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-std=c++0x
CXXFLAGS=-std=c++0x

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libconfiguration.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libconfiguration.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libconfiguration.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libconfiguration.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libconfiguration.a

${OBJECTDIR}/Src/NHOCCParameters.o: Src/NHOCCParameters.cpp
	${MKDIR} -p ${OBJECTDIR}/Src
	${RM} "$@.d"
	$(COMPILE.cc) -g -IInc -I../Utils/Inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Src/NHOCCParameters.o Src/NHOCCParameters.cpp

${OBJECTDIR}/Src/NHOCameraParameters.o: Src/NHOCameraParameters.cpp
	${MKDIR} -p ${OBJECTDIR}/Src
	${RM} "$@.d"
	$(COMPILE.cc) -g -IInc -I../Utils/Inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Src/NHOCameraParameters.o Src/NHOCameraParameters.cpp

${OBJECTDIR}/Src/NHOConfiguration.o: Src/NHOConfiguration.cpp
	${MKDIR} -p ${OBJECTDIR}/Src
	${RM} "$@.d"
	$(COMPILE.cc) -g -IInc -I../Utils/Inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Src/NHOConfiguration.o Src/NHOConfiguration.cpp

${OBJECTDIR}/Src/NHOMotorParameters.o: Src/NHOMotorParameters.cpp
	${MKDIR} -p ${OBJECTDIR}/Src
	${RM} "$@.d"
	$(COMPILE.cc) -g -IInc -I../Utils/Inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Src/NHOMotorParameters.o Src/NHOMotorParameters.cpp

${OBJECTDIR}/Src/NHORoverParameters.o: Src/NHORoverParameters.cpp
	${MKDIR} -p ${OBJECTDIR}/Src
	${RM} "$@.d"
	$(COMPILE.cc) -g -IInc -I../Utils/Inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Src/NHORoverParameters.o Src/NHORoverParameters.cpp

${OBJECTDIR}/Src/NHOSensorParameters.o: Src/NHOSensorParameters.cpp
	${MKDIR} -p ${OBJECTDIR}/Src
	${RM} "$@.d"
	$(COMPILE.cc) -g -IInc -I../Utils/Inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Src/NHOSensorParameters.o Src/NHOSensorParameters.cpp

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
