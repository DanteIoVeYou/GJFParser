#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cstring>
#include "GJFParser.hpp"
#include "Constant.hpp"
#include "Config.hpp"


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

    COMGenerator(Config config, GJFParser gjfparser, std::string gjf_filename, int option, bool charge_flag, int charge, bool spin_flag, int spin):
        m_gjf_filename(gjf_filename),
        m_chk_filename("%chk=" + m_gjf_filename.substr(0, m_gjf_filename.find(".gjf"))),
        m_config(config),
        m_gjfparser(gjfparser),
        m_option(option) 
    {
        SetConfig();
        if(charge_flag) {
            m_gjfparser.SetCharge(charge);
        }
        if(spin_flag) {
            m_gjfparser.SetSpin(spin);
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
        // m_file_content += (m_chk_filename + Constant::LF);
        // m_file_content += (mem + Constant::LF);
        // m_file_content += (proc+ Constant::LF);
        // m_file_content += (m_route + Constant::LF);
        // m_file_content += Constant::LF;
        // m_file_content += (a3 + Constant::LF);
        // m_file_content += Constant::LF;
        // m_file_content += (m_gjfparser.GetChargeAndSpinMultiplicityLine());
        // m_file_content += Constant::LF;
        // m_file_content += (m_gjfparser.GetAtomAndCoordinate() + Constant::LF);
        // m_file_content += Constant::LF;
        // if(!m_gjfparser.m_transition_element_set.empty()) {
        //     m_file_content += (BuildMainGroupElementsLine() + Constant::LF);
        //     m_file_content += ("6-31G(d)" + Constant::LF);
        //     m_file_content += ("****" + Constant::LF);
        //     m_file_content += (BuildTransitionElementsLine() + Constant::LF);
        //     m_file_content += ("LANL2DZ" + Constant::LF);
        //     m_file_content += ("****" + Constant::LF);
        //     m_file_content += Constant::LF;
        //     m_file_content += (BuildTransitionElementsLine() + Constant::LF);
        //     m_file_content += ("LANL2DZ" + Constant::LF);
        // }
        // m_file_content += Constant::LF;
        BuildLink0();
        BuildRoute();
        BuildBlankLine();
        BuildTitle();
        BuildBlankLine();
        BuildChargeMultipl();
        BuildMoleculeSpecification();
        BuildBlankLine();
        if(NeedPseudo()) {
            BuildFullElectronicBasicSet();
            BuildPseudoBasicSet();
            BuildBlankLine();
            BuildPseudo();
        }
        BuildBlankLine();
        return true;
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
     * @brief 判断是否需要赝势
     * 
     * @return true 
     * @return false 
     */
    bool NeedPseudo() {
        return !m_gjfparser.m_transition_element_set.empty();
    }

    /**
     * @brief 构建link0行，包含chk文件名，内存，共享核心数
     * 
     * @return true 
     * @return false 
     */
    bool BuildLink0() {
        m_file_content += (m_chk_filename + Constant::LF);
        if (m_link0.empty()) {
            m_file_content += ("%mem=2gb" + Constant::LF);
            m_file_content += ("%nprocshared=10" + Constant::LF);
            return true;
        }
        else {
            std::vector<std::string> link0_items;
            int pos_start = 0;
            int pos = 0;
            while(pos < m_link0.size() && (pos = m_link0.find('%', pos_start)) != std::string::npos) {
                std::string item;
                while(m_link0[pos] != ' ' && pos < m_link0.size()) {
                    item += m_link0[pos];
                    pos++;
                }
                pos_start = pos;
                link0_items.push_back(item);
            }
            for (const auto &item: link0_items) {
                m_file_content += (item + Constant::LF);
            }
            return true;
        }
    }

    /**
     * @brief 构建route行
     * 
     * @return true 
     * @return false 
     */
    bool BuildRoute() {
        if(m_option == OPT) {
            if(m_gjfparser.m_transition_element_set.empty()) {
                m_route = m_opt_route;
            }
            else {
                m_route = m_pseudo_opt_route;
            }
        }
        else if(m_option == TS) {
            if(m_gjfparser.m_transition_element_set.empty()) {
                m_route = m_ts_route;
            }
            else {
                m_route = m_pseudo_ts_route;
            }
        }
        if(m_route.empty()) {
            std::cerr << "route not found" << std::endl;
            return false;
        }
        else {
            m_file_content += (m_route + Constant::LF);
            return true;
        }
    }

    /**
     * @brief 构建标题行 
     * 
     * @return true 
     * @return false 
     */
    bool BuildTitle() {
        if(m_title.empty()) {
            std::cerr << "title is set defaultly with a3" << std::endl;
            m_file_content += ("a3" + Constant::LF);
            return true;
        }
        else {
            m_file_content += (m_title + Constant::LF);
            return true;
        }
    }

    /**
     * @brief 构建电荷数、自旋多重度行 
     * 
     * @return true 
     * @return false 
     */
    bool BuildChargeMultipl() {
        m_charge_multipl = m_gjfparser.GetChargeAndSpinMultiplicityLine();
        if (m_charge_multipl.empty())
        {
            std::cerr << "charge and multiplicity not found" << std::endl;
            return false;
        }
        else {
            m_file_content += (m_charge_multipl + Constant::LF);
            return true;
        }
    }

    /**
     * @brief 构建原子、原子坐标部分
     * 
     * @return true 
     * @return false 
     */
    bool BuildMoleculeSpecification() {
        m_molecule_specification = m_gjfparser.GetAtomAndCoordinate();
        if (m_molecule_specification.empty())
        {
            return false;
        }
        else {
            m_file_content += (m_molecule_specification + Constant::LF);
            return true;
        }
    }

    /**
     * @brief 全电子基组
     * 
     * @return true 
     * @return false 
     */
    bool BuildFullElectronicBasicSet() {
        for (auto elem : m_gjfparser.m_main_group_element_set)
        {
            m_full_electronic_elements += elem;
            m_full_electronic_elements += ' ';
        }
        m_full_electronic_elements += '0';
        if(m_full_electronic_elements.empty() || m_full_electronic_basic_set_method.empty()) {
            return false;
        }
        else {
            m_file_content += (m_full_electronic_elements + Constant::LF);
            m_file_content += (m_full_electronic_basic_set_method + Constant::LF);
            m_file_content += ("****" + Constant::LF);
            return true;
        }
    }

    /**
     * @brief 赝势基组 
     * 
     * @return true 
     * @return false 
     */
    bool BuildPseudoBasicSet() {
        for(auto elem: m_gjfparser.m_transition_element_set) {
            m_pseudo_elements += elem;
            m_pseudo_elements += ' ';
        }
        m_pseudo_elements += '0';
        if(m_pseudo_elements.empty() || m_pseudo_basic_set.empty()) {
            return false;
        }
        else {
            m_file_content += (m_pseudo_elements + Constant::LF);
            m_file_content += (m_pseudo_basic_set + Constant::LF);
            m_file_content += ("****" + Constant::LF);
            return true;
        }
    }

    /**
     * @brief 赝势 
     * 
     * @return true 
     * @return false 
     */
    bool BuildPseudo() {
        if(m_pseudo_elements.empty() || m_pseudo_basic_set.empty()) {
            return false;
        }
        else {
            m_file_content += (m_pseudo_elements + Constant::LF);
            m_file_content += (m_pseudo + Constant::LF);
            return true;
        }
    }

    /**
     * @brief 构建空行
     * 
     * @return true 
     * @return false 
     */
    bool BuildBlankLine() {
        m_file_content += Constant::LF;
        return true;
    }

    /**
     * @brief 从m_config类中读取配置项 
     * 
     */
    void SetConfig() {
        m_link0 = m_config.GetConfigMap()["LINK0"];
        m_opt_route = m_config.GetConfigMap()["OPT_ROUTE"];
        m_pseudo_opt_route = m_config.GetConfigMap()["PSEUDO_OPT_ROUTE"];
        m_ts_route = m_config.GetConfigMap()["TS_ROUTE"];
        m_pseudo_ts_route = m_config.GetConfigMap()["PSEUDO_TS_ROUTE"];
        m_title = m_config.GetConfigMap()["TITLE"];
        m_full_electronic_basic_set_method = m_config.GetConfigMap()["FULL_ELECTRONIC_BASIC_SET_METHOD"];
        m_pseudo_basic_set = m_config.GetConfigMap()["PSEUDOPOTENTIAL_BASIC_SET"];
        m_pseudo = m_config.GetConfigMap()["PSEUDOPOTENTIAL"];
    }


protected:
    /**
     * @brief 配置文件类 
     * 
     */
    Config m_config;
    /**
     * @brief gjf处理器类
     * 
     */
    GJFParser m_gjfparser;
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
    /**
     * @brief .com文件内容
     * 
     */
    std::string m_file_content;
    /**
     * @brief 第一个参数，用来选择优化/过渡态搜索
     * 
     */
    int m_option;
    /**
     * @brief 配置文件中读取的opt的route行
     * 
     */
    std::string m_opt_route;
    /**
     * @brief 配置文件中读取的pseudo opt的route行
     * 
     */
    std::string m_pseudo_opt_route;
    /**
     * @brief 配置文件中读取的ts的route行
     * 
     */
    std::string m_ts_route;
    /**
     * @brief 配置文件中读取的pseudo ts的route行
     * 
     */
    std::string m_pseudo_ts_route;
    /**
     * @brief com文件的link0行，以%开头，如%chk、%mem、%nprocshared
     *
     */
    std::string m_link0;
    /**
     * @brief route行
     * 
     */
    std::string m_route;
    /**
     * @brief 标题行
     * 
     */
    std::string m_title;
    /**
     * @brief 电荷+自旋多重度行
     * 
     */
    std::string m_charge_multipl;
    /**
     * @brief 原子、原子坐标部分
     * 
     */
    std::string m_molecule_specification;
    /**
     * @brief 全电子原子行
     * 
     */
    std::string m_full_electronic_elements;
    /**
     * @brief 全电子基组的方法 
     * 
     */
    std::string m_full_electronic_basic_set_method;
    /**
     * @brief 需要使用赝势的元素
     * 
     */
    std::string m_pseudo_elements;
    /**
     * @brief 赝势基组
     * 
     */
    std::string m_pseudo_basic_set;
    /**
     * @brief 赝势
     * 
     */
    std::string m_pseudo;
};