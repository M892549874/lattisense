#include <iostream>
#include <vector>
#include <cmath>
#include "fhe_ops_lib/fhe_lib_v2.h"
using namespace fhe_ops_lib;

int main() {
    uint64_t N = 1 << 14;
    CkksParameter param = CkksParameter::create_parameter(N);
    int level = 6;
    CkksContext ctx = CkksContext::create_random_context(param, level);
    double scale = 1LL << 40;
    double x_val = 2.0;
    std::vector<double> plain = {x_val};
    CkksPlaintext pt = ctx.encode(plain, level, scale);
    CkksCiphertext ct = ctx.encrypt_asymmetric(pt);

    // 计算幂次（最高6阶）
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

    int degree = 3;   // 修改为 4,5,6 分别测试

    CkksPlaintext one = ctx.encode({1.0}, level, scale);
    CkksCiphertext result = ctx.add_plain(ct, one);
    if (degree >= 2) {
        CkksPlaintext half = ctx.encode({0.5}, level, scale);
        result = ctx.add(result, ctx.mult_plain(x2, half));
    }
    if (degree >= 3) {
        CkksPlaintext sixth = ctx.encode({1.0/6.0}, level, scale);
        result = ctx.add(result, ctx.mult_plain(x3, sixth));
    }
    if (degree >= 4) {
        CkksPlaintext twenty4th = ctx.encode({1.0/24.0}, level, scale);
        result = ctx.add(result, ctx.mult_plain(x4, twenty4th));
    }
    if (degree >= 5) {
        CkksPlaintext one20th = ctx.encode({1.0/120.0}, level, scale);
        result = ctx.add(result, ctx.mult_plain(x5, one20th));
    }
    if (degree >= 6) {
        CkksPlaintext seven20th = ctx.encode({1.0/720.0}, level, scale);
        result = ctx.add(result, ctx.mult_plain(x6, seven20th));
    }

    CkksPlaintext pres = ctx.decrypt(result);
    std::vector<double> dec = ctx.decode(pres);
    double approx = dec[0];
    double true_exp = std::exp(x_val);
    std::cout << "Degree " << degree << ": exp_approx = " << approx
              << ", true = " << true_exp
              << ", error = " << std::abs(approx - true_exp) / true_exp * 100 << "%" << std::endl;
    return 0;
}