# OpenVX

## How to build

-----
```bash
#!/bin/bash

ROOT_DIR="${PWD}/OpenVX/"
mkdir -p "${ROOT_DIR}" && cd "${ROOT_DIR}"

# Clonning and building 'KhronosGroup' OpenVX implementation
KHRONOSGROUP_ROOT_DIR="${ROOT_DIR}/KhronosGroup/"
KHRONOSGROUP_OPENVX_ROOT_DIR="${KHRONOSGROUP_ROOT_DIR}/OpenVX-sample-impl"
KHRONOSGROUP_OPENVX_BUILD_DIR="${KHRONOSGROUP_OPENVX_ROOT_DIR}/build/"
KHRONOSGROUP_OPENVX_INSTALL_DIR="${KHRONOSGROUP_OPENVX_ROOT_DIR}/install/Linux/x64/Release/"
KHRONOSGROUP_OPENVX_INCLUDE_DIR="${KHRONOSGROUP_OPENVX_INSTALL_DIR}/include/"
KHRONOSGROUP_OPENVX_BIN_DIR="${KHRONOSGROUP_OPENVX_INSTALL_DIR}/bin/"

mkdir -p "${KHRONOSGROUP_ROOT_DIR}" && cd "${KHRONOSGROUP_ROOT_DIR}"

git clone --recursive https://github.com/KhronosGroup/OpenVX-sample-impl.git
# mkdir -p "${KHRONOSGROUP_OPENVX_BUILD_DIR}" && cd "${KHRONOSGROUP_OPENVX_BUILD_DIR}"
# cmake .. -DCMAKE_BUILD_TYPE=Release -DEXPERIMENTAL_USE_VENUM=OFF
# make -j$(nproc)
cd "${KHRONOSGROUP_OPENVX_ROOT_DIR}"
python3 Build.py --os=Linux --conf=Release --conf_vision --enh_vision

# Clonning and building 'dterletskiy' OpenVX examples implementation
TDA_ROOT_DIR="${ROOT_DIR}/dterletskiy/"
TDA_OPENVX_ROOT_DIR="${TDA_ROOT_DIR}/OpenVX/"
TDA_OPENVX_BUILD_DIR="${TDA_OPENVX_ROOT_DIR}/build/"
TDA_OPENVX_INSTALL_DIR="${TDA_OPENVX_ROOT_DIR}/install/"
TDA_OPENVX_INCLUDE_DIR="${TDA_OPENVX_INSTALL_DIR}/include/"
TDA_OPENVX_BIN_DIR="${TDA_OPENVX_INSTALL_DIR}/bin/"

mkdir -p "${TDA_ROOT_DIR}" && cd "${TDA_ROOT_DIR}"

git clone git@github.com:dterletskiy/OpenVX.git
mkdir -p "${TDA_OPENVX_BUILD_DIR}" && cd "${TDA_OPENVX_BUILD_DIR}"
cmake -DOPENVX_ROOT="${KHRONOSGROUP_OPENVX_INSTALL_DIR}" ..
make

# Running 'dterletskiy' example
export LD_LIBRARY_PATH="${KHRONOSGROUP_OPENVX_BIN_DIR}:${LD_LIBRARY_PATH}"
./example
```
-----
