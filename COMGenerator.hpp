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

    /**
     * @brief Construct a new COMGenerator object
     * 
     * @param gjf_filename 文件名
     */
    COMGenerator(Config config, GJFParser gjfparser, std::string gjf_filename, int option):
        m_gjf_filename(gjf_filename),
        m_chk_filename("%chk=" + m_gjf_filename.substr(0, m_gjf_filename.find(".gjf"))),
        m_config(config),
        m_gjfparser(gjfparser),
        m_option(option) 
    {
        SetMethod();
        SetConfig();
    }

    COMGenerator(std::string gjf_filename, int option, bool charge_flag, int charge, bool spin_flag, int spin):
        m_gjf_filename(gjf_filename),
        m_chk_filename("%chk=" + m_gjf_filename.substr(0, m_gjf_filename.find(".gjf"))),
        m_gjfparser(gjf_filename),
        m_option(option) 
    {
        SetMethod();
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
        BuildBlankLine();
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
    bool NeedPseudo() {
        return m_gjfparser.m_transition_element_set.empty();
    }
    bool BuildLink0() {
        if(m_link0.empty()) {
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

    bool BuildRoute() {
        if(m_route.empty()) {
            std::cerr << "route not found" << std::endl;
            return false;
        }
        else {
            m_file_content += (m_route + Constant::LF);
            return true;
        }
    }

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

    bool BuildChargeMultipl() {
        if(m_charge_multipl.empty()) {
            std::cerr << "charge and multiplicity not found" << std::endl;
            return false;
        }
        else {
            m_file_content += (m_charge_multipl + Constant::LF);
            return true;
        }
    }

    bool BuildMoleculeSpecification() {
        if(m_molecule_specification.empty()) {
            return false;
        }
        else {
            m_file_content += (m_molecule_specification + Constant::LF);
            return true;
        }
    }

    bool BuildFullElectronicBasicSet() {
        if(m_full_electronic_elements.empty() || m_full_electronic_basic_set.empty()) {
            return false;
        }
        else {
            m_file_content += (m_full_electronic_elements + Constant::LF);
            m_file_content += (m_full_electronic_basic_set + Constant::LF);
            m_file_content += ("****" + Constant::LF);
            return true;
        }
    }

    bool BuildPseudoBasicSet() {
        if(m_pseudo_elements.empty() || m_pseudo_basic_set.empty()) {
            return false;
        }
        else {
            m_file_content += (m_full_electronic_elements + Constant::LF);
            m_file_content += (m_full_electronic_basic_set + Constant::LF);
            m_file_content += ("****");
            return true;
        }
    }
    bool BuildPseudo() {
        if(m_pseudo_elements.empty() || m_pseudo_basic_set.empty()) {
            return false;
        }
        else {
            m_file_content += (m_full_electronic_elements + Constant::LF);
            m_file_content += (m_full_electronic_basic_set + Constant::LF);
            return true;
        }
    }


    bool BuildBlankLine() {
        m_file_content += Constant::LF;
        return true;
    }

    void SetMethod() {
        if(m_option == OPT) {
            if(m_gjfparser.m_transition_element_set.empty()) {
                m_route = Constant::opt_method;
            }
            else {
                m_route = Constant::metal_opt_method;
            }
        }
        else if(m_option == TS) {
            if(m_gjfparser.m_transition_element_set.empty()) {
                m_route = Constant::ts_method;
            }
            else {
                m_route = Constant::metal_ts_method;
            }
        }
    }


    void SetConfig() {
        std::unordered_map<std::string, std::string> config_map;
        m_config.ReadConfigFile("com.config", config_map);
        m_
    }

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
    std::string m_opt_route;
    std::string m_pseudo_opt_route;
    std::string m_ts_route;
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
     * @brief 标题
     * 
     */
    std::string m_title;
    std::string m_charge_multipl;
    std::string m_molecule_specification;
    std::string m_full_electronic_elements;
    std::string m_full_electronic_basic_set;
    std::string m_pseudo_elements;
    std::string m_pseudo_basic_set;
};