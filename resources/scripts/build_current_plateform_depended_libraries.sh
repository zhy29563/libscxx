#!/bin/bash

clear
########################################################################################################################
# 目录
########################################################################################################################
SCT_DIR=$(cd "$(dirname "$0")" && pwd) # scripts
RES_DIR=$(dirname "${SCT_DIR}")        # resources
PJT_DIR=$(dirname "${RES_DIR}")        # projects
ENV_DIR=${PJT_DIR}/envs                # environments
PKG_DIR=${ENV_DIR}/packages            # dependency packages
SRC_DIR=${ENV_DIR}/sources             # dependency sources
LIB_DIR=${ENV_DIR}/libs                # dependency libs

# 创建目录
mkdir -p "${ENV_DIR}" "${PKG_DIR}" "${SRC_DIR}" "${LIB_DIR}"

########################################################################################################################
# 变量
########################################################################################################################
TCL_ARCH=$(gcc -v 2>&1 | grep Target | awk '{print $2}')
TCL_VERS=$(gcc -v 2>&1 | grep 'gcc version' | awk '{print $3}')
TCL_NAME="${TCL_ARCH}-v${TCL_VERS}"

########################################################################################################################
# 函数
########################################################################################################################
function download_library() {
  # download and unpack package
  # supprt .zip and .tar.* package
  # usage:
  #   download_library package_url install_name

  local PKG_URL=$1
  local INS_DIR=$2
  echo "PKG_URL = ${PKG_URL}"
  echo "INS_DIR = ${INS_DIR}"

  # get extension name
  local PKG_EXT_NAME=${PKG_URL##*.}
  if [ "${PKG_EXT_NAME}" != "zip" ]; then
    local PKG_NAME_NO_EXT=${PKG_URL%.*}
    local PKG_SECOND_EXT_NAME=${PKG_NAME_NO_EXT##*.}
    if [ "${PKG_SECOND_EXT_NAME}" = "tar" ]; then
      PKG_EXT_NAME=${PKG_SECOND_EXT_NAME}.${PKG_EXT_NAME}
    else
      echo "error:unsupported extention: ${PKG_SECOND_EXT_NAME}.${PKG_EXT_NAME}"
      exit
    fi
  fi
  echo "PKG_EXT_NAME = ${PKG_EXT_NAME}"

  # download
  local FULL_NEW_PKG_NAME=${PKG_DIR}/${INS_DIR}.${PKG_EXT_NAME}
  echo "FULL_NEW_PKG_NAME=${FULL_NEW_PKG_NAME}"
  if [ ! -e "${FULL_NEW_PKG_NAME}" ]; then
    if ! wget "${PKG_URL}" -O "${FULL_NEW_PKG_NAME}"; then
      echo "error:download package: ${PKG_URL} failed."
      exit
    fi
  fi

  # get lib src dir name in package
  local LIB_SRC_DIR_NAME
  if [ "${PKG_EXT_NAME}" = "zip" ]; then
    LIB_SRC_DIR_NAME=$(unzip -l "${FULL_NEW_PKG_NAME}" | grep / | grep -v -e "zip$" | awk -F "[ /]+" '{print $5}' | sort | uniq)
  else
    LIB_SRC_DIR_NAME=$(tar -tf "${FULL_NEW_PKG_NAME}" | awk -F "/" '{print $1}' | sort | uniq)
  fi
  echo "LIB_SRC_DIR_NAME=${LIB_SRC_DIR_NAME}"

  if [ -z "${LIB_SRC_DIR_NAME}" ]; then
    echo "error:selected name for extracted files is failed."
    exit
  fi

  # unpack lib src to src dir
  local FULL_OLD_LIB_SRC_NAME=${SRC_DIR}/${LIB_SRC_DIR_NAME}
  local FULL_NEW_LIB_SRC_NAME=${SRC_DIR}/${INS_DIR}
  echo "FULL_OLD_LIB_SRC_NAME=${FULL_OLD_LIB_SRC_NAME}"
  echo "FULL_NEW_LIB_SRC_NAME=${FULL_NEW_LIB_SRC_NAME}"
  if [ ! -e "${FULL_NEW_LIB_SRC_NAME}" ]; then
    if [ "${PKG_EXT_NAME}" = "zip" ]; then
      unzip -q -d "${SRC_DIR}" -o "${FULL_NEW_PKG_NAME}"
    else
      tar -xf "${FULL_NEW_PKG_NAME}" -C "${SRC_DIR}"
    fi
    mv "${FULL_OLD_LIB_SRC_NAME}" "${FULL_NEW_LIB_SRC_NAME}"
  fi
}

function build_cmake() {
  # build lib for base on cmake
  # usage：
  #   build_cmake package_url install_name build_params

  local PKG_URL=$1
  local INS_DIR=$2
  shift 2

  # $@ the array of parameter string
  # $* a string of all parameters
  local BLD_PMS=$*
  echo "PKG_URL=${PKG_URL}"
  echo "INS_DIR=${INS_DIR}"
  echo "BLD_PMS=${BLD_PMS}"
  download_library "${PKG_URL}" "${INS_DIR}"

  local FULL_SRC_PATH=${SRC_DIR}/${INS_DIR}
  local FULL_BLT_PATH=${FULL_SRC_PATH}/build_${TCL_NAME}
  local FULL_INS_PATH=${LIB_DIR}/${TCL_NAME}/${INS_DIR}

  rm -rf "${FULL_BLT_PATH}"
  if cmake -S "${FULL_SRC_PATH}" -B "${FULL_BLT_PATH}" -DCMAKE_INSTALL_PREFIX="${FULL_INS_PATH}" ${BLD_PMS}; then
    if ! cmake --build "${FULL_BLT_PATH}" --target install -j; then
      echo "build ${PKG_URL} is failed."
    fi
  else
    echo "configure ${PKG_URL} is failed."
  fi
}

function build_auto_tools() {
  # build lib for base on autoconf
  # usage：
  #   build_cmake package_url install_name build_params

  local PKG_URL=$1
  local INS_DIR=$2
  shift 2

  # $@ the array of parameter string
  # $* a string of all parameters
  local BLD_PMS=$*
  echo "PKG_URL=${PKG_URL}"
  echo "INS_DIR=${INS_DIR}"
  echo "BLD_PMS=${BLD_PMS}"
  download_library "${PKG_URL}" "${INS_DIR}"

  local FULL_SRC_PATH=${SRC_DIR}/${INS_DIR}
  local FULL_BLT_PATH=${FULL_SRC_PATH}/build_${TCL_NAME}
  local FULL_INS_PATH=${LIB_DIR}/${TCL_NAME}/${INS_DIR}

  cd "${FULL_SRC_PATH}" || {
    echo "switch to ${FULL_SRC_PATH} failed."
    exit
  }

  if [ -e autogen.sh ]; then
    bash autogen.sh
  fi

  if [ -e configure.ac ] || [ -e configure.in ]; then
    autoreconf -fiv
  fi

  rm -rf "${FULL_BLT_PATH}"
  mkdir -p "${FULL_BLT_PATH}"
  cd "${FULL_BLT_PATH}" || {
    echo "switch to ${FULL_BLT_PATH} failed."
    exit
  }

  if "${OLDPWD}/configure" --prefix="${FULL_INS_PATH}" ${BLD_PMS}; then
    if ! make install; then
      echo "build ${PKG_URL} is failed."
    fi
  else
    echo "configure ${PKG_URL} is failed."
  fi
}

########################################################################################################################
# 编译
########################################################################################################################
build_cmake https://github.com/cameron314/readerwriterqueue/archive/refs/tags/v1.0.6.tar.gz libreaderwriterqueue-v1.0.6
