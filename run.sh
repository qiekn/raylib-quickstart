#!/bin/bash

# Config
PROJECT_NAME="main"

WEBBUILD="build"
BUILD_DIR="build"

DEBUGGER="lldb"

# Ensure build dir exists
if [ ! -d "${BUILD_DIR}" ]; then
  echo "[INFO] ${BUILD_DIR} not found, mkdir ${BUILD_DIR}and running cmake..."
  cmake -S . -B "${BUILD_DIR}" || exit 1
fi

if [ "$1" = "debug" ]; then
  ${DEBUGGER} "./${BUILD_DIR}/${PROJECT_NAME}"
elif [ "$1" = "web" ]; then
  make -j"$(nproc)" -C "${WEBBUILD}" && "./${WEBBUILD}/out/index.html"
else
  # Default: build for desktop
  make -j"$(nproc)" -C "${BUILD_DIR}" && "./${BUILD_DIR}/${PROJECT_NAME}"
fi

# vim: ft=sh ts=2 sw=2 et
