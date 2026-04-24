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
    double a_val = 3.0;
    std::vector<double> plain = {a_val};
    CkksPlaintext pt = ctx.encode(plain, level, scale);
    CkksCiphertext ct = ctx.encrypt_asymmetric(pt);

    CkksPlaintext y0_pt = ctx.encode({0.33}, level, scale);
    CkksCiphertext y = ctx.encrypt_asymmetric(y0_pt);

    int iter = 0;   // 修改为 0,1,2,3 分别测试

    for (int i = 0; i < iter; ++i) {
        CkksCiphertext3 ay = ctx.mult(ct, y);
        CkksCiphertext ay_relin = ctx.relinearize(ay);
        CkksCiphertext ay_rescale = ctx.rescale(ay_relin, scale);
        CkksPlaintext two_pt = ctx.encode({2.0}, 0, scale);
        CkksCiphertext two_minus = ctx.add_plain(ctx.negate(ay_rescale), two_pt);
        CkksCiphertext3 new_y = ctx.mult(y, two_minus);
        y = ctx.relinearize(new_y);
        y = ctx.rescale(y, scale);
    }

    CkksPlaintext pres = ctx.decrypt(y);
    std::vector<double> dec = ctx.decode(pres);
    double approx = dec[0];
    double true_inv = 1.0 / a_val;
    std::cout << "Iterations " << iter << ": inv_approx = " << approx
              << ", true = " << true_inv
              << ", error = " << std::abs(approx - true_inv) / true_inv * 100 << "%" << std::endl;
    return 0;
}