#include <iostream>

#include <torch/torch.h>

int main() {
    std::cout << "CUDA available: " << (torch::cuda::is_available() ? "yes" : "no") << std::endl;

    auto tensor_a = torch::ones({2, 3});
    std::cout << "ones(2,3):\n" << tensor_a << std::endl;

    auto tensor_b = torch::randn({2, 3});
    std::cout << "randn(2,3):\n" << tensor_b << std::endl;

    auto tensor_sum = tensor_a + tensor_b;
    std::cout << "sum:\n" << tensor_sum << std::endl;

    // Matrix multiply: (2x3) @ (3x2) -> (2x2)
    auto tensor_mm = torch::mm(tensor_a, tensor_b.t());
    std::cout << "matmul:\n" << tensor_mm << std::endl;
    return 0;
}
