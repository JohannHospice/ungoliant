mkdir tmp
cd tmp
curl https://cmake.org/files/v3.8/cmake-3.8.0-rc1.tar.gz > cmake-3.8.0-rc1.tar.gz
tar zxvf cmake-3.8.0-rc1.tar.gz
cd cmake-3.8.0-rc1
./configure
make
sudo make install
cd ../../ && rm -rf tmp