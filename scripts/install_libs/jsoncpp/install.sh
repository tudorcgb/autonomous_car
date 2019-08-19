git clone https://github.com/open-source-parsers/jsoncpp
cd jsoncpp
mkdir build
cd build
cmake ..
make
sudo make install
cd ../..
rm -rf jsoncpp
