# CryptoPlayground 

Playing with Crypto

# Setup for Ubuntu 24.04

# update

```bash
sudo apt-get update
sudo apt-get upgrade
sudo apt-get dist-upgrade
```

# Essentials and C++ libs

```bash
sudo apt-get install -y unzip build-essential cmake gdb git wget cmake libfmt-dev libblas-dev libopenblas-dev libfftw3-dev
sudo apt-get install -y python3 python3-pip python-is-python3 python3-venv
sudo apt-get install -y liblapacke-dev liblapack-dev libboost-all-dev libopencv-dev libhdf5-dev
sudo apt-get install -y libjson-c-dev libx11-dev openjdk-21-jdk ninja-build gnuplot vim libcpuinfo-dev libspdlog-dev
sudo apt-get install -y clang clang-tidy clang-format clangd mold lldb cmake ccache pkg-config
```

- link with -lopenblas ou -lblas -llapack instead of -latlas

# In VS Code install Extensions

- C/C++ (ms-vscode.cpptools)
- CMake Tools (ms-vscode.cmake-tools)
- clangd (llvm-vs-code-extensions.vscode-clangd) – optional, great IntelliSense
- CodeLLDB (Vadim Chugunov)

# Linear Algebra 

```bash
sudo apt install -y libeigen3-dev xtensor-dev libxtensor-blas-dev libomp-dev

sudo apt install -y libarrayfire-unified3 libarrayfire-unified-dev libarrayfire-opencl3
sudo apt install -y libarrayfire-cpu3 libarrayfire-cpu-dev
sudo apt install -y libdlib-dev libdlib-data
```

# Create/Activate Python Env

```bash
python3 -m venv .venv
source .venv/bin/activate
pip install --upgrade pip
pip install -r requirements.txt
pip install torch==2.7.1 --index-url https://download.pytorch.org/whl/cpu
```




