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
	${OBJECTDIR}/Src/NHODEXM.o \
	${OBJECTDIR}/Src/NHORover.o \
	${OBJECTDIR}/main.o


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
LDLIBSOPTIONS=-L../../../../wiringPi/wiringPi ../Sensor/dist/Debug/GNU-Linux/libsensor.a ../Network/dist/Debug/GNU-Linux/libnetwork.a ../Utils/dist/Debug/GNU-Linux/libutils.a -lpthread ../RaspiCam/Lib/libraspicam.so -lwiringPi ../Actuator/dist/Debug/GNU-Linux/libactuator.a ../Configuration/dist/Debug/GNU-Linux/libconfiguration.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/rover

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/rover: ../Sensor/dist/Debug/GNU-Linux/libsensor.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/rover: ../Network/dist/Debug/GNU-Linux/libnetwork.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/rover: ../Utils/dist/Debug/GNU-Linux/libutils.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/rover: ../RaspiCam/Lib/libraspicam.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/rover: ../Actuator/dist/Debug/GNU-Linux/libactuator.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/rover: ../Configuration/dist/Debug/GNU-Linux/libconfiguration.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/rover: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/rover ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/Src/NHODEXM.o: Src/NHODEXM.cpp
	${MKDIR} -p ${OBJECTDIR}/Src
	${RM} "$@.d"
	$(COMPILE.cc) -g -D_RASPBIAN -IInc -I../Network/Inc -I../Sensor/Inc -I../Utils/Inc -I../RaspiCam/Inc -I../../../../wiringPi/wiringPi -I../Configuration/Inc -I../Actuator/Inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Src/NHODEXM.o Src/NHODEXM.cpp

${OBJECTDIR}/Src/NHORover.o: Src/NHORover.cpp
	${MKDIR} -p ${OBJECTDIR}/Src
	${RM} "$@.d"
	$(COMPILE.cc) -g -D_RASPBIAN -IInc -IInc -I../Network/Inc -I../Sensor/Inc -I../Utils/Inc -I../RaspiCam/Inc -I../../../../wiringPi/wiringPi -I../Configuration/Inc -I../Actuator/Inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Src/NHORover.o Src/NHORover.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -D_RASPBIAN -IInc -I../Network/Inc -I../Sensor/Inc -I../Utils/Inc -I../RaspiCam/Inc -I../../../../wiringPi/wiringPi -I../Configuration/Inc -I../Actuator/Inc -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:
	cd ../Sensor && ${MAKE}  -f Makefile CONF=Debug
	cd ../Network && ${MAKE}  -f Makefile CONF=Debug
	cd ../Utils && ${MAKE}  -f Makefile CONF=Debug
	cd ../Actuator && ${MAKE}  -f Makefile CONF=Debug
	cd ../Configuration && ${MAKE}  -f Makefile CONF=Debug
	cd ../Utils && ${MAKE}  -f Makefile CONF=Debug
	cd ../Sensor && ${MAKE}  -f Makefile CONF=Debug
	cd ../Network && ${MAKE}  -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} -r ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libraspicam.so
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/rover

# Subprojects
.clean-subprojects:
	cd ../Sensor && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../Network && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../Utils && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../Actuator && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../Configuration && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../Utils && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../Sensor && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../Network && ${MAKE}  -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
