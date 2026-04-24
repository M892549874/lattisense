#include <iostream>
#include <vector>
#include "fhe_ops_lib/fhe_lib_v2.h"
using namespace fhe_ops_lib;

int main() {
    uint64_t N = 1 << 14;
    CkksParameter param = CkksParameter::create_parameter(N);
    int level = 3;
    CkksContext ctx = CkksContext::create_random_context(param, level);
    double scale = 1LL << 40;

    std::vector<double> a = {2.0}, b = {3.0};
    CkksPlaintext pt_a = ctx.encode(a, level, scale);
    CkksPlaintext pt_b = ctx.encode(b, level, scale);
    CkksCiphertext ct_a = ctx.encrypt_asymmetric(pt_a);
    CkksCiphertext ct_b = ctx.encrypt_asymmetric(pt_b);

    // 加法
    CkksCiphertext sum = ctx.add(ct_a, ct_b);
    CkksPlaintext psum = ctx.decrypt(sum);
    std::vector<double> s = ctx.decode(psum);
    std::cout << "Sum: " << s[0] << std::endl;

    // 乘法
    CkksCiphertext3 prod3 = ctx.mult(ct_a, ct_b);
    CkksCiphertext prod = ctx.relinearize(prod3);
    prod = ctx.rescale(prod, scale);
    CkksPlaintext pprod = ctx.decrypt(prod);
    std::vector<double> p = ctx.decode(pprod);
    std::cout << "Product: " << p[0] << std::endl;

    return 0;
}