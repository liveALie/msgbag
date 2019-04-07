current_path=`pwd`
cd ../build
rm -rf ./*
cmake ..
make -j4