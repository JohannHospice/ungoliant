mkdir tmp && cd tmp
curl https://curl.haxx.se/download/curl-7.52.1.tar.gz > curl-7.52.1.tar.gz
tar zxvf curl-7.52.1.tar.gz
cd curl-7.52.1
./configure
make
sudo make install 
cd ../../ && rm -rf tmp