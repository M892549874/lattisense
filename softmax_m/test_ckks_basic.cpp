#include <iostream>
#include <vector>
#include "fhe_ops_lib/fhe_lib_v2.h"
using namespace fhe_ops_lib;

int main() {
    uint64_t N = 1 << 14;
    CkksParameter param = CkksParameter::create_parameter(N);
    int level = 2;
    CkksContext ctx = CkksContext::create_random_context(param, level);
    double scale = 1LL << 40;

    std::vector<double> plain = {1.23, 4.56};
    CkksPlaintext pt = ctx.encode(plain, level, scale);
    CkksCiphertext ct = ctx.encrypt_asymmetric(pt);
    CkksPlaintext pt2 = ctx.decrypt(ct);
    std::vector<double> dec = ctx.decode(pt2);

    std::cout << "Decrypted: ";
    for (auto v : dec) std::cout << v << " ";
    std::cout << std::endl;
    return 0;
}