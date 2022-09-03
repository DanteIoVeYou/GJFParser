#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <cstring>

/**
 * @brief 主族元素集合 
 * 
 */
const static std::unordered_set<std::string> main_group_element_table = {
    "C",
    "H",
    "O",
    "N",
    "P",
    "S"
};

/**
 * @brief 过渡金属集合
 * 
 */
const static std::unordered_set<std::string> transition_element_table = {
    "Au"
};

/**
 * @brief 通用工具类
 * 
 */
class Utils {

public:

    static bool IsInteger(const std::string &num) {
        int start = 0;
        if (num[0] == '-') {
            start++;
        }
        for (int i = start; i < num.size(); i++)
        {
            if(num[i] >= '0' || num[i] <= '9') {
                continue;
            }
            else {
                return false;
            }
        }
        return true;
    }

    /**
     * @brief 根据sep将字符串切分为两部分 
     * 
     * @param str 
     * @param first 
     * @param last 
     * @param sep 
     * @return true 
     * @return false 
     */
    static bool CutStringBySep(const std::string &str, std::string *first, std::string *last, const std::string &sep) {
        int pos_sep = str.find(sep);
        if(pos_sep != std::string::npos) {
            std::string first_str = str.substr(0, pos_sep);
            std::string last_str = str.substr(pos_sep + sep.size());
            if(first_str.size() != 0 && last_str.size() != 0) {
                *first = first_str;
                *last = last_str;
                return true;
            }
            else {
                return false;
            }
        }
        else {
            return false;
        }
    }

    /**
     * @brief 判断元素是否为主族元素
     * 
     * @param elememt 
     * @return true 
     * @return false 
     */
    static bool IsMainGroupElement(const std::string &elememt) {
        return main_group_element_table.find(elememt) != main_group_element_table.end();
    }

    /**
     * @brief 判断元素是否为过渡金属
     * 
     * @param elememt 
     * @return true 
     * @return false 
     */
    static bool IsTransitionGroupElement(const std::string &elememt) {
        return transition_element_table.find(elememt) != transition_element_table.end();
    }

    /**
     * @brief 判断空行
     * 
     * @param line 
     * @return true 
     * @return false 
     */
    static bool IsBlankLine(const std::string &line) {
        if(line.size() == 0) {
            return true;
        }
        else if(line == "\r\n") {
            std::cerr << "is blank" << std::endl;
            return true;
        }
        else if(line == "\n") {
            return true;
        }
        else if(line == "\r") {
            return true;
        }
        else {
            for (int i = 0; i < line.size(); i++) {
                if (line[i] == ' ') {
                    continue;
                }
                else {
                    return false;
                }
            }
            return true;
        }
    }
};