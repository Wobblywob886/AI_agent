#ifndef HEADER_H
#define HEADER_H

// Thư viện C++ hiện đại cơ bản
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <functional>
#include <optional>
#include <expected> // Tính năng C++23 dùng cho xử lý lỗi mạng
#include <fstream>
#include <filesystem>

// Nhúng thư viện nlohmann/json (Bạn chỉ cần tải file json.hpp bỏ vào cùng thư mục)
#include "json.hpp" 
// using json = nlohmann::json;
using json = nlohmann::json;  // defined in json.hpp

#endif // HEADER_H