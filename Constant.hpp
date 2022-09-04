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

    const static std::string opt_method;

    const static std::string ts_method;

    const static std::string metal_opt_method;

    const static std::string metal_ts_method;
};
const std::string Constant::LF = "\n";
const std::string Constant::COMMENT_MARK = "#";
const std::string Constant::opt_method = "#p b3lyp/6-31g(d) opt freq=noraman";
const std::string Constant::metal_opt_method = "#p b3lyp/gen Pseudo=Read opt freq=noraman";
const std::string Constant::ts_method = "#p b3lyp/6-31G(d) opt(ts,calcfc,noeigen) freq=noraman";
const std::string Constant::metal_ts_method = "#p b3lyp/gen Pseudo=Read opt(ts,calcfc,noeigen) freq=noraman";
