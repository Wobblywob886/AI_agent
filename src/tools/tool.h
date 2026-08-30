#ifndef TOOL_H
#define TOOL_H

#include "common/Header.h"

class Tool {
public:
    virtual ~Tool() = default;

    // Trả về tên định danh của Tool (Ví dụ: "calculator")
    virtual std::string getName() const = 0;
    
    // Trả về mô tả tính năng để chỉ dẫn cho LLM biết khi nào nên dùng
    virtual std::string getDescription() const = 0;

    // Thực thi công cụ với tham số đầu vào, trả về std::optional (C++17) phòng khi tool lỗi
    virtual std::optional<std::string> execute(const std::string& args) = 0;
};

#endif // TOOL_H