#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cstring>
#include "GJFParser.hpp"
#include "Constant.hpp"

/**
 * @brief 换行符
 * 
 */
const static std::string mem = "%mem=2gb";
const static std::string proc = "%nprocshared=10";
const static std::string a3= "a3";


/**
 * @brief .生成com文件的类
 * 
 */
class COMGenerator {
public:
    /**
     * @brief Construct a new COMGenerator object
     * 
     */
    COMGenerator() {}

    /**
     * @brief Construct a new COMGenerator object
     * 
     * @param gjf_filename 文件名
     */
    COMGenerator(std::string gjf_filename, int option):
        m_gjf_filename(gjf_filename),
        m_chk_filename("%chk=" + m_gjf_filename.substr(0, m_gjf_filename.find(".gjf"))),
        m_gjfparser(gjf_filename),
        m_option(option) 
    {
        if(m_option == OPT) {
            if(m_gjfparser.m_transition_element_set.empty()) {
                m_method = Constant::opt_method;
            }
            else {
                m_method = Constant::metal_opt_method;
            }
        }
        else if(m_option == TS) {
            if(m_gjfparser.m_transition_element_set.empty()) {
                m_method = Constant::ts_method;
            }
            else {
                m_method = Constant::metal_ts_method;
            }
        }
    }

    /**
     * @brief Destroy the COMGenerator object
     * 
     */
    ~COMGenerator() {}


    /**
     * @brief 构建.com文件
     * 
     * @return true 
     * @return false 
     */
    bool BuildFile() {
        m_file_content += (m_chk_filename + Constant::LF);
        m_file_content += (mem + Constant::LF);
        m_file_content += (proc+ Constant::LF);
        m_file_content += (m_method + Constant::LF);
        m_file_content += Constant::LF;
        m_file_content += (a3 + Constant::LF);
        m_file_content += Constant::LF;
        m_file_content += (m_gjfparser.GetChargeAndSpinMultiplicityLine());
        m_file_content += Constant::LF;
        m_file_content += (m_gjfparser.GetAtomAndCoordinate() + Constant::LF);
        m_file_content += Constant::LF;
        if(!m_gjfparser.m_transition_element_set.empty()) {
            m_file_content += (BuildMainGroupElementsLine() + Constant::LF);
            m_file_content += ("6-31G(d)" + Constant::LF);
            m_file_content += ("****" + Constant::LF);
            m_file_content += (BuildTransitionElementsLine() + Constant::LF);
            m_file_content += ("LANL2DZ" + Constant::LF);
            m_file_content += ("****" + Constant::LF);
            m_file_content += Constant::LF;
            m_file_content += (BuildTransitionElementsLine() + Constant::LF);
            m_file_content += ("LANL2DZ" + Constant::LF);
        }
        m_file_content += Constant::LF;
    }

    /**
     * @brief 导出.com文件
     * 
     * @return true 
     * @return false 
     */
    bool GenerateFile() {
        std::ofstream out((m_gjf_filename.substr(0, m_gjf_filename.find(".gjf")) + ".com"));
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
     * @brief 构建主族元素行
     * 
     * @return std::string 
     */
    std::string BuildMainGroupElementsLine() {
        std::string main_group_elems;
        for(auto elem: m_gjfparser.m_main_group_element_set) {
            main_group_elems += elem;
            main_group_elems += ' ';
        }
        main_group_elems += '0';
        return main_group_elems;
    }

    /**
     * @brief 构建过渡元素的行
     * 
     * @return std::string 
     */
    std::string BuildTransitionElementsLine() {
        std::string transition_elems;
        for(auto elem: m_gjfparser.m_transition_element_set) {
            transition_elems += elem;
            transition_elems += ' ';
        }
        transition_elems += '0';
        return transition_elems;
    }

protected:
    /**
     * @brief gjf文件名
     * 
     */
    std::string m_gjf_filename;
    /**
     * @brief .com文件内容 
     * 
     */
    std::string m_chk_filename;
    std::string m_file_content;
    /**
     * @brief 
     * 
     */
    GJFParser m_gjfparser;
    int m_option;
    std::string m_method;

};