wget https://apt.llvm.org/llvm.sh
chmod +x llvm.sh
sudo ./llvm.sh 22
sudo apt update
sudo apt install -y libc++-22-dev libc++abi-22-dev

sudo update-alternatives --install /usr/bin/clang   clang   /usr/bin/clang-22   1000
sudo update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-22 1000
sudo update-alternatives --set clang   /usr/bin/clang-22
sudo update-alternatives --set clang++ /usr/bin/clang++-22
clang --version