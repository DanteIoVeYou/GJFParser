#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cstring>
#include "Constant.hpp"


/**
 * @brief 生成.pbs的类
 * 
 */
class PBSGenerator {

public:

    /**
     * @brief Construct a new PBSGenerator object
     * 
     * @param gjf_filename 
     */
    PBSGenerator(std::string gjf_filename)
        :m_gjf_filename(gjf_filename)
    {}

    /**
     * @brief Destroy the PBSGenerator object
     * 
     */
    ~PBSGenerator() {}

    /**
     * @brief 构建.pbs文件
     * 
     */
    void BuildFile() {
        m_file_content += ("#!/bin/sh" + Constant::LF);
        m_file_content += ("#PBS -N " + m_gjf_filename.substr(0, m_gjf_filename.find(".gjf")) + Constant::LF);
        m_file_content += ("#PBS -j oe" + Constant::LF);
        m_file_content += ("#PBS -l nodes=1:ppn=12" + Constant::LF);
        m_file_content += ("#PBS -l walltime=999:00:00" + Constant::LF);
        m_file_content += ("#PBS -q batch" + Constant::LF);
        m_file_content += ("#PBS -V" + Constant::LF);
        m_file_content += ("export GAUSS_EXEDIR=/opt/g16" + Constant::LF);
        m_file_content += ("export g16root=/opt" + Constant::LF);
        m_file_content += ("export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$g16root/g16" + Constant::LF);
        m_file_content += ("export GAUSS_SCRDIR=/tmp" + Constant::LF);
        m_file_content += ("source $GAUSS_EXEDIR/bsd/g16.profile" + Constant::LF);
        m_file_content += ("FILENAME=" + m_gjf_filename.substr(0, m_gjf_filename.find(".gjf")) + ".com" + Constant::LF);
        m_file_content += (Constant::LF);
        m_file_content += ("#executable programe" + Constant::LF);
        m_file_content += ("cd $PBS_O_WORKDIR" + Constant::LF);
    }

    /**
     * @brief 导出.pbs文件
     * 
     * @return true 
     * @return false 
     */
    bool GenerateFile() {
        std::ofstream out((m_gjf_filename.substr(0, m_gjf_filename.find(".gjf")) + ".pbs"));
        if(out.is_open()) {
            out << m_file_content;
            out.close();
            return true;
        }
        else {
            out.close();
            return false;
        }
    }

protected:

    /**
     * @brief gjf文件名
     * 
     */
    std::string m_gjf_filename;
    /**
     * @brief .pbs文件内容
     * 
     */
    std::string m_file_content;
};
