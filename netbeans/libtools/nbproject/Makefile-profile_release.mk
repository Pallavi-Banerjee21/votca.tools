#
# Gererated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran

# Include project Makefile
include Makefile_nb

# Object Directory
OBJECTDIR=build/profile_release/GNU-Linux-x86

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools/cubicspline.o \
	${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools/tokenizer.o \
	${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools/parcer.o \
	${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools/property.o \
	${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools/random.o \
	${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools/histogramnew.o \
	${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools/matrix.o \
	${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools/crosscorrelate.o \
	${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools/correlate.o \
	${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools/rangeparser.o \
	${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools/histogram.o \
	${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools/table.o \
	${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools/datacollection.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-I../../include -pg
CXXFLAGS=-I../../include -pg

# Fortran Compiler Flags
FFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS} ../../src/libtools/libtools.a

../../src/libtools/libtools.a: ${OBJECTFILES}
	${MKDIR} -p ../../src/libtools
	${RM} ../../src/libtools/libtools.a
	${AR} rv ../../src/libtools/libtools.a ${OBJECTFILES} 
	$(RANLIB) ../../src/libtools/libtools.a

${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools/cubicspline.o: ../../src/libtools/cubicspline.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools
	$(COMPILE.cc) -O3 -I../../include -o ${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools/cubicspline.o ../../src/libtools/cubicspline.cc

${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools/tokenizer.o: ../../src/libtools/tokenizer.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools
	$(COMPILE.cc) -O3 -I../../include -o ${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools/tokenizer.o ../../src/libtools/tokenizer.cc

${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools/parcer.o: ../../src/libtools/parcer.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools
	$(COMPILE.cc) -O3 -I../../include -o ${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools/parcer.o ../../src/libtools/parcer.cc

${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools/property.o: ../../src/libtools/property.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools
	$(COMPILE.cc) -O3 -I../../include -o ${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools/property.o ../../src/libtools/property.cc

${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools/random.o: ../../src/libtools/random.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools
	$(COMPILE.cc) -O3 -I../../include -o ${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools/random.o ../../src/libtools/random.cc

${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools/histogramnew.o: ../../src/libtools/histogramnew.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools
	$(COMPILE.cc) -O3 -I../../include -o ${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools/histogramnew.o ../../src/libtools/histogramnew.cc

${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools/matrix.o: ../../src/libtools/matrix.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools
	$(COMPILE.cc) -O3 -I../../include -o ${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools/matrix.o ../../src/libtools/matrix.cc

${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools/crosscorrelate.o: ../../src/libtools/crosscorrelate.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools
	$(COMPILE.cc) -O3 -I../../include -o ${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools/crosscorrelate.o ../../src/libtools/crosscorrelate.cc

${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools/correlate.o: ../../src/libtools/correlate.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools
	$(COMPILE.cc) -O3 -I../../include -o ${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools/correlate.o ../../src/libtools/correlate.cc

${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools/rangeparser.o: ../../src/libtools/rangeparser.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools
	$(COMPILE.cc) -O3 -I../../include -o ${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools/rangeparser.o ../../src/libtools/rangeparser.cc

${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools/histogram.o: ../../src/libtools/histogram.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools
	$(COMPILE.cc) -O3 -I../../include -o ${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools/histogram.o ../../src/libtools/histogram.cc

${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools/table.o: ../../src/libtools/table.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools
	$(COMPILE.cc) -O3 -I../../include -o ${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools/table.o ../../src/libtools/table.cc

${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools/datacollection.o: ../../src/libtools/datacollection.cc 
	${MKDIR} -p ${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools
	$(COMPILE.cc) -O3 -I../../include -o ${OBJECTDIR}/_ext/people/thnfs/homes/ruehle/src/tools/src/libtools/datacollection.o ../../src/libtools/datacollection.cc

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/profile_release
	${RM} ../../src/libtools/libtools.a

# Subprojects
.clean-subprojects:
