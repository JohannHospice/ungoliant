mkdir tmp
cd tmp
wget https://github.com/htacg/tidy-html5/archive/master.zip
unzip master.zip
cd master/build/cmake
cmake ../.. -DCMAKE_BUILD_TYPE=Release [-DCMAKE_INSTALL_PREFIX=/path/for/install]
make
sudo make install
cd ../../ && rm -rf tmp