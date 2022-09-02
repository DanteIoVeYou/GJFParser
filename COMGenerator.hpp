#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cstring>
#include "GJFParser.hpp"
#include "constant.hpp"

/**
 * @brief 换行符
 * 
 */
const static std::string chk = "%chk=M-opt";
const static std::string mem = "%mem=2gb";
const static std::string proc = "%nprocshared=10";
const static std::string method = "#p b3lyp/gen Pseudo=Read opt freq=noraman";
const static std::string a3= "a3";


class COMGenerator {
public:
    COMGenerator() {}

    COMGenerator(std::string gjf_filename)
        :m_gjfparser(gjf_filename) 
    {
        
    }

    ~COMGenerator() {

    }


    bool BuildFile() {
        m_file_content += (chk + Constant::LF);
        m_file_content += (mem + Constant::LF);
        m_file_content += (proc+ Constant::LF);
        m_file_content += (method + Constant::LF);
        m_file_content += Constant::LF;
        m_file_content += (a3 + Constant::LF);
        m_file_content += Constant::LF;
        m_file_content += (m_gjfparser.GetChargeAndSpinMultiplicityLine() + Constant::LF);
        m_file_content += m_gjfparser.GetAtomAndCoordinate();
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

    bool GenerateFile() {
        std::ofstream out("test.com");
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
    std::string BuildMainGroupElementsLine() {
        std::string main_group_elems;
        for(auto elem: m_gjfparser.m_main_group_element_set) {
            main_group_elems += elem;
            main_group_elems += ' ';
        }
        main_group_elems += '0';
        return main_group_elems;
    }

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
    GJFParser m_gjfparser;
    std::string m_file_content;
};