#pragma once
#include <string>

/**
 * @brief 常量类
 * 
 */
class Constant {
public:
    /**
     * @brief 换行符
     * 
     */
    static const std::string LF;

    static const std::string COMMENT_MARK;
};

const std::string Constant::LF = "\n";
const std::string Constant::COMMENT_MARK = "#";