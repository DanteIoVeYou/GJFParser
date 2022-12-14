#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cstring>
#include "GJFParser.hpp"
#include "COMGenerator.hpp"
#include "PBSGenterator.hpp"

/**
 * @brief 控制器类
 * 
 */
class Controller {

public:

    /**
     * @brief Construct a new Controller object
     * 
     * @param gjf_filename 
     */
    Controller(std::string gjf_filename, int option, bool charge_flag, int charge, bool spin_flag, int spin):
        m_gjf_filename(gjf_filename),
        m_option(option),
        m_config(Constant::CONFIG_FILENAME),
        m_gjfparser(gjf_filename),
        m_com_generator(m_config, m_gjfparser, m_gjf_filename, m_option, charge_flag, charge, spin_flag, spin),
        m_pbs_generator(m_gjf_filename)
    {}

    /**
     * @brief Destroy the Controller object
     * 
     */
    ~Controller() {}

    /**
     * @brief 调用com generator和pbs generator的构建、导出文件的方法
     * 
     */
    void Start() {
        m_com_generator.BuildFile();
        m_com_generator.GenerateFile();
        m_pbs_generator.BuildFile();
        m_pbs_generator.GenerateFile();
    }


protected:
    /**
     * @brief gjf文件名
     * 
     */
    std::string m_gjf_filename;
    /**
     * @brief -opt/-ts
     * 
     */
    int m_option;
    /**
     * @brief 配置文件类
     * 
     */
    Config m_config;
    /**
     * @brief gjfparser类
     * 
     */
    GJFParser m_gjfparser;
    /**
     * @brief com generator成员
     *
     */
    COMGenerator m_com_generator;
    /**
     * @brief pbs generator成员
     * 
     */
    PBSGenerator m_pbs_generator;
};