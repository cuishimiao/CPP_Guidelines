
---

### **手写 STL 容器与数学库 —— 深入底层，掌握数据结构的灵魂**

---

#### **1. 实现 `vector`：动态数组的终极奥义**
**核心设计目标**：
- 动态扩容策略（时间复杂度均摊 O(1)）
- 异常安全性（强异常保证）
- 支持迭代器（兼容 STL 算法）

**类模板定义**：
```cpp
template <typename T, typename Allocator = std::allocator<T>>
class Vector {
public:
    // 类型别名（兼容 STL 风格）
    using value_type = T;
    using iterator = T*;
    using const_iterator = const T*;
    using size_type = size_t;

    // 构造函数
    Vector() = default;
    explicit Vector(size_type count, const T& value = T());
    Vector(const Vector& other);
    Vector(Vector&& other) noexcept;

    // 析构函数
    ~Vector();

    // 容量操作
    size_type size() const { return size_; }
    size_type capacity() const { return capacity_; }
    void reserve(size_type new_cap);

    // 元素访问
    T& operator[](size_type pos) { return data_[pos]; }
    const T& operator[](size_type pos) const { return data_[pos]; }

    // 迭代器
    iterator begin() { return data_; }
    iterator end() { return data_ + size_; }

    // 插入/删除
    void push_back(const T& value);
    void push_back(T&& value);

private:
    T* data_ = nullptr;
    size_type size_ = 0;
    size_type capacity_ = 0;
    Allocator alloc_;

    void reallocate(size_type new_cap);
};
```

**关键实现细节**：
1. **倍增扩容策略**：
```cpp
void Vector<T>::push_back(const T& value) {
    if (size_ >= capacity_) {
        // 扩容策略：初始为1，之后倍增
        size_type new_cap = (capacity_ == 0) ? 1 : capacity_ * 2;
        reallocate(new_cap);
    }
    alloc_.construct(data_ + size_, value);
    ++size_;
}
```

2. **内存管理（使用分配器）**：
```cpp
void Vector<T>::reallocate(size_type new_cap) {
    T* new_data = alloc_.allocate(new_cap);
    for (size_type i = 0; i < size_; ++i) {
        alloc_.construct(new_data + i, std::move(data_[i]));
        alloc_.destroy(data_ + i);
    }
    alloc_.deallocate(data_, capacity_);
    data_ = new_data;
    capacity_ = new_cap;
}
```

3. **异常安全性**：
```cpp
// 使用 RAII 保证构造失败时资源释放
try {
    alloc_.construct(new_data + i, data_[i]);
} catch (...) {
    for (size_type j = 0; j < i; ++j) {
        alloc_.destroy(new_data + j);
    }
    alloc_.deallocate(new_data, new_cap);
    throw;
}
```

**优化技巧**：
- **移动语义支持**：
  ```cpp
  void push_back(T&& value) {
      if (size_ >= capacity_) {
          reallocate(capacity_ ? capacity_ * 2 : 1);
      }
      alloc_.construct(data_ + size_, std::move(value));
      ++size_;
  }
  ```
- **SSO（Small String Optimization）思想**：对小容量使用栈存储

---

#### **2. 实现 `unordered_map`：哈希表的艺术**
**核心设计目标**：
- 基于开链法解决哈希冲突
- 动态 rehash 控制负载因子
- 支持 C++11 的桶接口

**类模板定义**：
```cpp
template <
    typename Key,
    typename Value,
    typename Hash = std::hash<Key>,
    typename KeyEqual = std::equal_to<Key>
>
class UnorderedMap {
public:
    using Node = std::pair<const Key, Value>;
    using size_type = size_t;

    // 构造函数
    UnorderedMap() = default;
    explicit UnorderedMap(size_type bucket_count);

    // 容量
    size_type size() const { return size_; }
    bool empty() const { return size_ == 0; }

    // 元素访问
    Value& operator[](const Key& key);

    // 修改器
    std::pair<iterator, bool> insert(const Node& node);
    size_type erase(const Key& key);

private:
    struct Bucket {
        std::list<Node> chain;
    };

    std::vector<Bucket> buckets_;
    size_type size_ = 0;
    float max_load_factor_ = 1.0f;
    Hash hasher_;
    KeyEqual key_eq_;

    void rehash(size_type new_bucket_count);
    size_type bucket_index(const Key& key) const {
        return hasher_(key) % buckets_.size();
    }
};
```

**关键实现细节**：
1. **插入操作**：
```cpp
std::pair<iterator, bool> UnorderedMap::insert(const Node& node) {
    auto& bucket = buckets_[bucket_index(node.first)];
    for (auto& entry : bucket.chain) {
        if (key_eq_(entry.first, node.first)) {
            return {&entry, false}; // 已存在
        }
    }
    // 检查负载因子
    if (load_factor() > max_load_factor_) {
        rehash(buckets_.size() * 2);
    }
    bucket.chain.push_back(node);
    ++size_;
    return {&bucket.chain.back(), true};
}
```

2. **rehash 策略**：
```cpp
void UnorderedMap::rehash(size_type new_bucket_count) {
    std::vector<Bucket> new_buckets(new_bucket_count);
    for (auto& bucket : buckets_) {
        for (auto& node : bucket.chain) {
            size_type idx = hasher_(node.first) % new_bucket_count;
            new_buckets[idx].chain.splice(
                new_buckets[idx].chain.end(),
                bucket.chain,
                bucket.chain.iterator_to(node)
            );
        }
    }
    buckets_.swap(new_buckets);
}
```

3. **operator[] 实现**：
```cpp
Value& UnorderedMap::operator[](const Key& key) {
    auto it = find(key);
    if (it != end()) {
        return it->second;
    }
    return insert({key, Value{}}).first->second;
}
```

**性能优化**：
- **开放寻址法替代链表**：使用 `std::vector` 存储节点，减少内存碎片
- **缓存友好布局**：将键值对存储在连续内存中
- **SIMD 加速哈希**：使用 SSE/AVX 指令优化哈希计算

---

#### **3. 实现数学库：高性能矩阵运算**
**核心功能**：
- 矩阵加减乘
- 转置、行列式、逆矩阵
- SIMD 加速（SSE/AVX）

**类定义**：
```cpp
class Matrix {
public:
    // 构造函数
    Matrix(size_t rows, size_t cols);
    Matrix(std::initializer_list<std::initializer_list<double>> init);

    // 基本运算
    Matrix operator+(const Matrix& other) const;
    Matrix operator*(const Matrix& other) const;
    Matrix transpose() const;

    // 高级运算
    double determinant() const;
    Matrix inverse() const;

    // 访问元素
    double& operator()(size_t row, size_t col);
    const double& operator()(size_t row, size_t col) const;

private:
    std::vector<double> data_;
    size_t rows_;
    size_t cols_;

    void validate_dimensions(const Matrix& other, const std::string& op) const;
};
```

**关键实现细节**：
1. **矩阵乘法（朴素算法）**：
```cpp
Matrix Matrix::operator*(const Matrix& other) const {
    validate_dimensions(other, "multiply");
    Matrix result(rows_, other.cols_);
    for (size_t i = 0; i < rows_; ++i) {
        for (size_t k = 0; k < cols_; ++k) {
            double tmp = (*this)(i, k);
            for (size_t j = 0; j < other.cols_; ++j) {
                result(i, j) += tmp * other(k, j);
            }
        }
    }
    return result;
}
```

2. **SIMD 优化乘法（AVX2）**：
```cpp
#include <immintrin.h>

void matrix_multiply_avx2(const double* a, const double* b, double* c,
                          size_t m, size_t n, size_t p) {
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < p; j += 4) {
            __m256d sum = _mm256_setzero_pd();
            for (size_t k = 0; k < n; ++k) {
                __m256d a_vec = _mm256_broadcast_sd(a + i*n + k);
                __m256d b_vec = _mm256_loadu_pd(b + k*p + j);
                sum = _mm256_fmadd_pd(a_vec, b_vec, sum);
            }
            _mm256_storeu_pd(c + i*p + j, sum);
        }
    }
}
```

3. **行列式计算（递归法）**：
```cpp
double Matrix::determinant() const {
    if (rows_ != cols_) throw std::logic_error("Not a square matrix");
    if (rows_ == 1) return data_[0];
    double det = 0;
    for (size_t j = 0; j < cols_; ++j) {
        Matrix minor(rows_-1, cols_-1);
        // 构造余子式
        // ...
        det += ((j % 2 == 0) ? 1 : -1) * data_[j] * minor.determinant();
    }
    return det;
}
```

**高级优化技术**：
- **分块算法（Blocking）**：优化缓存利用率
- **多线程并行**：使用 OpenMP 或线程池加速
- **Strassen 算法**：将矩阵乘法复杂度降至 O(n^2.81)

---

#### **4. 测试与调试技巧**
1. **单元测试框架**：
```cpp
#define ASSERT_EQ(a, b) if ((a) != (b)) { std::cerr << "Test failed!\n"; }

void test_vector_push_back() {
    Vector<int> vec;
    vec.push_back(42);
    ASSERT_EQ(vec.size(), 1);
    ASSERT_EQ(vec[0], 42);
}
```

2. **Valgrind 检测内存泄漏**：
```bash
valgrind --leak-check=full ./test_program
```

3. **Benchmark 性能分析**：
```cpp
#include <chrono>

auto start = std::chrono::high_resolution_clock::now();
// 待测试代码
auto end = std::chrono::high_resolution_clock::now();
std::cout << "Time: "
          << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
          << "ms\n";
```

---

### **总结：从轮子制造到性能调优**
| **组件**          | **核心难点**                | **关键优化点**              |
|-------------------|----------------------------|----------------------------|
| **`vector`**      | 异常安全的扩容逻辑          | 移动语义、SSO 优化          |
| **`unordered_map`** | 哈希冲突与 rehash 策略      | 开放寻址法、缓存友好布局     |
| **矩阵库**        | SIMD 指令并行化            | 分块算法、多线程加速        |

**设计哲学**：
- **资源管理**：RAII 是 C++ 的基石
- **零开销抽象**：高性能代码不应为封装付出代价
- **测试驱动**：每个功能点对应精确的单元测试

掌握这些底层实现，你将在系统编程、游戏引擎、高频交易等领域游刃有余！ 🛠️