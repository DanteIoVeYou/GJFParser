#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cstring>
#include "GJFParser.hpp"

/**
 * @brief 换行符
 * 
 */
const static std::string LF = "\n";
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
        m_file_content += chk;
        m_file_content += LF;
        m_file_content += mem;
        m_file_content += LF;
        m_file_content += proc;
        m_file_content += LF;
        m_file_content += method;
        m_file_content += LF;
        m_file_content += LF;
        m_file_content += a3;
        m_file_content += LF;
        m_file_content += LF;
        m_file_content += m_gjfparser.GetChargeAndSpinMultiplicityLine();
        m_file_content += LF;
        m_file_content += m_gjfparser.GetAtomAndCoordinate();
        if(!m_gjfparser.m_transition_element_set.empty()) {
            std::string main_group_elems;
            for(auto elem: m_gjfparser.m_main_group_element_set) {
                main_group_elems += elem;
                main_group_elems += ' ';
            }
            main_group_elems += '0';
            m_file_content += main_group_elems;
            m_file_content += LF;
            m_file_content += "6-31G(d)";
            m_file_content += LF;
            m_file_content += "****";
            m_file_content += LF;

            std::string transition_elems;
            for(auto elem: m_gjfparser.m_transition_element_set) {
                transition_elems += elem;
                transition_elems += ' ';
            }
            transition_elems += '0';
            m_file_content += transition_elems;
            m_file_content += LF;
            m_file_content += "LANL2DZ";
            m_file_content += LF;
            m_file_content += "****";
            m_file_content += LF;
            m_file_content += LF;
            m_file_content += transition_elems;
            m_file_content += LF;
            m_file_content += "LANL2DZ";
            m_file_content += LF;
        }
        m_file_content += LF;
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
    GJFParser m_gjfparser;
    std::string m_file_content;
};