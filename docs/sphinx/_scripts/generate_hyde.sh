#!/bin/bash

#very simple for now, look in common locations
find_hyde() {
    BUILD_DIR=$1
    if [ -f $BUILD_DIR/hyde ]
    then
        echo $BUILD_DIR/hyde
        return 0
    fi
    if [ -f $BUILD_DIR/Debug/hyde ]
    then
        echo $BUILD_DIR/Debug/hyde
        return 0
    fi
    if [ -f $BUILD_DIR/Release/hyde ]
    then
        echo $BUILD_DIR/Release/hyde
        return 0
    fi
    return 1
}   

EXE_DIR=$(dirname "$0")

pushd ${EXE_DIR} > /dev/null

#HYDE_ROOT=/Users/lums658/Contrib/hyde

HYDE_ROOT=/home/nwmath/hyde


HYDE_PATH=`find_hyde "${HYDE_ROOT}/build"`
echo HYDE_PATH is ${HYDE_PATH}

# NWGRAPH_ROOT=/Users/lums658/Working/NWmath/NWgr
NWGRAPH_ROOT=/tmp/foo/NWgr

NWGRAPH_SRC=${NWGRAPH_ROOT}/include

HYDE_DST_ROOT=${NWGRAPH_ROOT}/docs/sphinx/libref

NWGRAPH_INCLUDES="-I${NWGRAPH_SRC} -I${NWGRAPH_SRC}/algorithms -I${NWGRAPH_SRC}/util -I/tmp/foo/NWut/include"

for CUR_LIB in ${NWGRAPH_SRC} ${NWGRAPH_SRC}/algorithms ${NWGRAPH_SRC}/util  ${NWGRAPH_SRC}/detail ; do
    HYDE_SRC_ROOT=${NWGRAPH_SRC}

    for CUR_FILE in ${CUR_LIB}/*.hpp ; do
	
	echo "Processing $CUR_FILE"
	
	CUR_COMMAND="${HYDE_PATH} -hyde-very-verbose -hyde-src-root=${HYDE_SRC_ROOT} -hyde-yaml-dir=${HYDE_DST_ROOT} -hyde-update --auto-toolchain-includes "$CUR_FILE" -- -std=c++17 ${NWGRAPH_INCLUDES}"

	echo $CUR_COMMAND
	eval $CUR_COMMAND
	
    done
done

popd > /dev/null
