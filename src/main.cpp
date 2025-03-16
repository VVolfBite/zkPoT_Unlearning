#include <vector>
#include <math.h>
#include "MLP.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mimc.h"
#include "quantization.h"
#include "GKR.h"
#include <chrono>
#include "utils.hpp"
#include "pol_verifier.h"
#include "CNN.h"
#include "elliptic_curves.h"
#include "config_pc.hpp"
#include "poly_commit.h"
#include "lookups.h"
#include <unistd.h>

extern int partitions;

int PC_scheme, Commitment_hash;
int levels;
using namespace std;

double proving_time;
double total_time;
double range_proof_time = 0.0;
int threads = 1;
unsigned long int mul_counter = 0;
double Forward_propagation;
double dx_computation;
double gradients_computation;
vector<int> predicates_size;
vector<struct proof> Transcript;
vector<F> SHA;
vector<F> H;

vector<F> x_transcript, y_transcript;
F current_randomness;
double aggregation_time = 0.0;

F test_addition(F num1, F num2) {
    return num1 + num2;  // 定点数加法
}

F test_multiplication(F num1, F num2) {
    return num1 * num2;  // 定点数乘法
}

F test_division(F num1, F num2) {
    return divide(num1, num2);  // 定点数除法，使用 divide 函数
}

F test_mixed_operations(F num1, int int_num, float float_num) {
    // 混合运算：定点数和整数及浮点数
    F int_as_F = F(int_num);  // 普通整数转为 F 类型
    F float_as_F = quantize(float_num);  // 浮点数转为定点数

    // 先进行定点数除法，再与整数和浮点数相加
    F result = test_division(num1, int_as_F);
    result = result + float_as_F;

    return result;
}

F test_exp(F x) {
    x = divide(x * F(1 << Q), quantize(M_exp));

    F base = quantize(1) + x;
    for (int i = 0; i < (int)log2(M_exp); i++) {
        base = base * base;
    }
    return base;
}

F test_shift(F num1, int depth) {
    vector<F> result = shift(num1, depth);
    return result[0];  // 返回商
}

int main() {
    // 定点数表示的 num1 和 num2
    F num1 = quantize(12.34f);
    F num2 = quantize(5.67f);

    // 普通整数 num3 和浮点数 num4
    F num3 = F(10);
    float num4 = 7.89f;

    // 测试加法
    F addition_result = test_addition(num1, num2);

    // 测试乘法
    F multiplication_result = test_multiplication(num1, num2);

    // 测试除法
    F division_result = test_division(num1, num2);

    // 测试混合运算
    F mixed_result = test_mixed_operations(num1, 3, num4);

    // 测试指数运算
    F exp_result = test_exp(num1);

    // 测试移位操作
    F shift_result = test_shift(num1, 3);

    // 将结果存储到变量中，方便后续调试
    F addition_output = dequantize(addition_result, 0);
    F multiplication_output = dequantize(dequantize(multiplication_result, 0),0);
    F division_output = dequantize(division_result, 0);
    F mixed_output = dequantize(mixed_result, 0);
    F exp_output = dequantize(exp_result, 0);
    F shift_output = dequantize(shift_result, 0);

    // 返回或处理计算结果，例如返回结构体或存储在数组中
    // 返回的是变量，具体的操作可能根据需求来调整
    return 0;
}
