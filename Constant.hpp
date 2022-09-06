#pragma once
#include <string>

enum OPTION
{
    OPT,
    TS
};

/**
 * @brief 常量类
 * 
 */
class Constant {
public:
    Constant() {}
    ~Constant() {}

    /**
     * @brief 换行符
     * 
     */
    const static std::string LF;

    /**
     * @brief 配置文件注释标记 
     * 
     */
    const static std::string COMMENT_MARK;

    /**
     * @brief 默认的配置文件名
     * 
     */
    const static std::string CONFIG_FILENAME;

};
const std::string Constant::LF = "\n";
const std::string Constant::COMMENT_MARK = "#";
const std::string Constant::CONFIG_FILENAME = "com.config";