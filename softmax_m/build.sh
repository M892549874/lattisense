#!/bin/bash
cd ~/LattiSense

g++ -std=c++17 test_ckks_basic.cpp -o test_ckks_basic \
    -I/home/kali/LattiSense/fhe_ops_lib \
    -I/home/kali/LattiSense/cxx_sdk_v2 \
    -I/home/kali/LattiSense/lib \
    -L/home/kali/LattiSense/build \
    -llattisense -lssl -lcrypto -lpthread

g++ -std=c++17 test_add_mul.cpp -o test_add_mul \
    -I/home/kali/LattiSense/fhe_ops_lib \
    -I/home/kali/LattiSense/cxx_sdk_v2 \
    -I/home/kali/LattiSense/lib \
    -L/home/kali/LattiSense/build \
    -llattisense -lssl -lcrypto -lpthread

g++ -std=c++17 my_softmax_simple.cpp -o my_softmax_simple \
    -I/home/kali/LattiSense/fhe_ops_lib \
    -I/home/kali/LattiSense/cxx_sdk_v2 \
    -I/home/kali/LattiSense/lib \
    -L/home/kali/LattiSense/build \
    -llattisense -lssl -lcrypto -lpthread

g++ -std=c++17 test_exp_degree.cpp -o test_exp_degree \
    -I/home/kali/LattiSense/fhe_ops_lib \
    -I/home/kali/LattiSense/cxx_sdk_v2 \
    -I/home/kali/LattiSense/lib \
    -L/home/kali/LattiSense/build \
    -llattisense -lssl -lcrypto -lpthread

g++ -std=c++17 test_inv_iter.cpp -o test_inv_iter \
    -I/home/kali/LattiSense/fhe_ops_lib \
    -I/home/kali/LattiSense/cxx_sdk_v2 \
    -I/home/kali/LattiSense/lib \
    -L/home/kali/LattiSense/build \
    -llattisense -lssl -lcrypto -lpthread

echo "All programs compiled successfully."