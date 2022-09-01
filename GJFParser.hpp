#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <cstring>
#include "utils.hpp"

/**
 * @brief 换行符
 * 
 */
const static std::string LF = "\r\n";

/**
 * @brief 读取、提取、处理gjf文件信息的类 
 * 
 */
class GJFParser {
public:
    /**
     * @brief Construct a new GJFParser object
     * 
     */
    GJFParser() {}
    /**
     * @brief Construct a new GJFParser object
     * 
     * @param gjf_filename 
     */
    GJFParser(std::string gjf_filename) 
        :m_read_atom_flag(false)
    {
        m_gjf_file = std::ifstream(gjf_filename);
        if(m_gjf_file.is_open()) {
            ReadGJFFile();
        }
        else {
            // open file error

        }
    }
    ~GJFParser() {
        m_gjf_file.close();
    }
public:
    /**
     * @brief Get the Charge And Spin Multiplicity Line object
     * 
     * @return std::string 
     */
    std::string GetChargeAndSpinMultiplicityLine() {
        return m_charge_and_spin_multiplicity;
    }

    /**
     * @brief Get the Atom And Coordinate object
     * 
     * @return std::string 
     */
    std::string GetAtomAndCoordinate() {
        return m_atoms_line;
    }

    std::string GetMainGroupElements() {
        std::string str;
        for(auto elem: m_main_group_element_set) {
            str += (elem + ' ');
        }
        str += LF;
        return str;
    }

private:
    bool ReadGJFFile() {
        std::string line;
        while(getline(m_gjf_file, line)) {
            // 读取原子和原子坐标
            if(m_read_atom_flag) {
                if(Utils::IsBlankLine(line)) {
                    break;
                }
                int start = 0;
                for (; start < line.size(); start++)
                {
                    if(line[start] == ' ') {
                        continue;
                    }
                    else {
                        std::string element;
                        while(line[start] != ' ') {
                            element.push_back(line[start]);
                            start++;
                        }
                        m_element_set.insert(element);
                        break;
                    }
                }
                m_atoms_table.push_back(line);
            }
            // 读取含有电荷与自旋多重度的行
            if(JudgeChargeAndMultiplicityLine(line)) {
                std::string charge;
                std::string spin_multiplicity;
                if(ReadChargeandSpinMultiplicity(line, &charge, &spin_multiplicity, " ")) {
                    line.back() = '\n';
                    m_charge_and_spin_multiplicity = line;
                    m_charge = atoi(charge.c_str());
                    m_spin_multiplicity = atoi(charge.c_str());
                    m_read_atom_flag = true;
                }
                else {
                    std::cerr << "电荷，自旋多重度读取失败" << std::endl;
                    break;
                    // error
                }
            }
        }
        for(const auto &line: m_atoms_table) {
            m_atoms_line.back() = '\n';
            m_atoms_line += line;
        }
        DistinguishMainGroupAndTransition();
    }
    /**
     * @brief 遍历所有元素的集合，将其分为主族元素和过渡金属元素并放入对应的集合中 
     * 
     * @return true 
     * @return false 
     */
    bool DistinguishMainGroupAndTransition() {
        if(!m_element_set.empty()) {
            for(auto &element: m_element_set) {
                if(Utils::IsMainGroupElement(element)) {
                    m_main_group_element_set.insert(element);
                }
                else if(Utils::IsTransitionGroupElement(element)) {
                    m_transition_element_set.insert(element);
                }
            }
        }
    }
    /**
     * @brief 判断该行是否为含有电荷和自选多重度的行
     * 
     * @param line 
     * @return true 
     * @return false 
     */
    bool JudgeChargeAndMultiplicityLine(const std::string &line) {
            if( line.size() > 0 && ( (line[0] >= '0' && line[0] <= '9') || (line[0] == '-') ) ) {
                return true;
            }
            else {
                return false;
            }
    }
    /**
     * @brief 读取电荷和自选多重度
     * 
     * @return true 
     * @return false 
     */
    bool ReadChargeandSpinMultiplicity(const std::string &line, std::string *charge, std::string *spin_multiplicity, const std::string &sep) {
        if(Utils::CutStringBySep(line, charge, spin_multiplicity, sep)) {
            return true;
        }
        else {
            return false;
        }
    }

protected:
    /**
     * @brief gjf文件流
     * 
     */
    std::ifstream m_gjf_file;
    /**
     * @brief 总元素种类的集合
     * 
     */
    std::unordered_set<std::string> m_element_set;
public:
    /**
     * @brief 主族元素种类的集合
     * 
     */
    std::unordered_set<std::string> m_main_group_element_set;
    /**
     * @brief 过渡金属元素种类的集合
     * 
     */
    std::unordered_set<std::string> m_transition_element_set;
protected:
    /**
     * @brief 包含电荷以及自旋多重度的行 
     * 
     */
    std::string m_charge_and_spin_multiplicity;
    /**
     * @brief 体系所带电荷数
     * 
     */
    int m_charge;
    /**
     * @brief 体系自选多重度
     * 
     */
    int m_spin_multiplicity;
    /**
     * @brief 开始读取原子坐标的标志
     * 
     */
    bool m_read_atom_flag;
    /**
     * @brief 每个成员为一行 元素+坐标 的数组
     * 
     */
    std::vector<std::string> m_atoms_table;
    /**
     * @brief 元素+坐标的行文本
     * 
     */
    std::string m_atoms_line;
};