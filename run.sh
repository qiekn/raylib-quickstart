#!/bin/bash

# Config
PROJECT_NAME="game"

WEBBUILD="build"
BUILD_DIR="build"

DEBUGGER="gdb -q"

# Ensure build dir exists
if [ ! -d "${BUILD_DIR}" ]; then
  echo "[INFO] ${BUILD_DIR} not found, mkdir ${BUILD_DIR}and running cmake..."
  cmake -S . -B "${BUILD_DIR}" -G Ninja || exit 1
fi

if [ "$1" = "debug" ]; then
  ${DEBUGGER} "./${BUILD_DIR}/${PROJECT_NAME}"
elif [ "$1" = "web" ]; then
  cmake --build ${WEBBUILD} -j  && "./${WEBBUILD}/out/index.html"
else
  # Default: build for desktop
  cmake --build ${BUILD_DIR} -j  && "./${BUILD_DIR}/${PROJECT_NAME}"
fi

# vim: ft=sh ts=2 sw=2 et
