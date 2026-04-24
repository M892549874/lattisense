#include <iostream>
#include <vector>
#include <cmath>
#include "fhe_ops_lib/fhe_lib_v2.h"
using namespace fhe_ops_lib;

int main() {
    uint64_t N = 1 << 14;
    CkksParameter param = CkksParameter::create_parameter(N);
    int level = 5;
    CkksContext ctx = CkksContext::create_random_context(param, level);
    double scale = 1LL << 40;

    std::vector<double> plain = {2.0};
    CkksPlaintext pt = ctx.encode(plain, level, scale);
    CkksCiphertext ct = ctx.encrypt_asymmetric(pt);

    // 计算幂次 x^2 ... x^6
    CkksCiphertext3 x2_3 = ctx.mult(ct, ct);
    CkksCiphertext x2 = ctx.relinearize(x2_3);
    x2 = ctx.rescale(x2, scale);
    CkksCiphertext3 x3_3 = ctx.mult(x2, ct);
    CkksCiphertext x3 = ctx.relinearize(x3_3);
    x3 = ctx.rescale(x3, scale);
    CkksCiphertext3 x4_3 = ctx.mult(x3, ct);
    CkksCiphertext x4 = ctx.relinearize(x4_3);
    x4 = ctx.rescale(x4, scale);
    CkksCiphertext3 x5_3 = ctx.mult(x4, ct);
    CkksCiphertext x5 = ctx.relinearize(x5_3);
    x5 = ctx.rescale(x5, scale);
    CkksCiphertext3 x6_3 = ctx.mult(x5, ct);
    CkksCiphertext x6 = ctx.relinearize(x6_3);
    x6 = ctx.rescale(x6, scale);

    // 泰勒展开：1 + x + x^2/2! + ... + x^6/6!
    CkksPlaintext one = ctx.encode({1.0}, level, scale);
    CkksCiphertext exp_ct = ctx.add_plain(ct, one);
    CkksPlaintext half = ctx.encode({0.5}, level, scale);
    exp_ct = ctx.add(exp_ct, ctx.mult_plain(x2, half));
    CkksPlaintext sixth = ctx.encode({1.0/6.0}, level, scale);
    exp_ct = ctx.add(exp_ct, ctx.mult_plain(x3, sixth));
    CkksPlaintext twenty4th = ctx.encode({1.0/24.0}, level, scale);
    exp_ct = ctx.add(exp_ct, ctx.mult_plain(x4, twenty4th));
    CkksPlaintext one20th = ctx.encode({1.0/120.0}, level, scale);
    exp_ct = ctx.add(exp_ct, ctx.mult_plain(x5, one20th));
    CkksPlaintext seven20th = ctx.encode({1.0/720.0}, level, scale);
    exp_ct = ctx.add(exp_ct, ctx.mult_plain(x6, seven20th));

    // 倒数近似（常数，仅演示）
    CkksPlaintext inv_pt = ctx.encode({0.5}, level, scale);
    CkksCiphertext inv = ctx.encrypt_asymmetric(inv_pt);

    // 归一化
    CkksCiphertext3 res3 = ctx.mult(exp_ct, inv);
    CkksCiphertext res = ctx.relinearize(res3);
    res = ctx.rescale(res, scale);

    CkksPlaintext pres = ctx.decrypt(res);
    std::vector<double> decoded = ctx.decode(pres);
    std::cout << "Homomorphic Softmax (single element): " << decoded[0] << std::endl;
    std::cout << "Plain Softmax (single element): 1.0" << std::endl;
    return 0;
}