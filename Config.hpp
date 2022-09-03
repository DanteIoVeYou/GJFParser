#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cstring>
#include "Constant.hpp"

/**
 * @brief 配置文件类 
 * 
 */
class Config {

public:
    /**
     * @brief 读取配置文件，将键值对存入哈希表
     *
     * @param filename 配置文件名
     * @param map
     * @return true
     * @return false
     */
    bool ReadConfigFile(const std::string &filename, std::unordered_map<std::string, std::string> &map) {
        std::ifstream in(filename);
        if(in.is_open()) {
            std::string line;
            while(std::getline(in, line)) {
                // 判断该行是否是注释
                bool is_comment = IsComment(line, Constant::COMMENT_MARK);
                // 判断该行是否为空行
                bool is_blackline = Utils::IsBlankLine(line);
                if(is_comment || is_blackline) {
                    continue;
                }
                else {
                    const std::string sep = "=";
                    std::string key;
                    std::string value;
                    int ret = GetKeyValue(line, sep, &key, &value);
                    if(!ret) {
                        // error
                    }
                    else {
                        if(key.size() == 0 || value.size() == 0) {
                            continue;
                        }
                        else {
                            if(map.find(key) != map.end()) {
                                // 找到对应的键
                                map[key] = value;
                            }
                            else {
                                // 非法的键
                                continue;
                            }
                        }
                    }
                }
            }
        }
        else {
            // error
        }
    }

protected:
    /**
     * @brief 判断行文本是否为被mark标注的注释行 
     * 
     * @param line 
     * @param mark 
     * @return true 
     * @return false 
     */
    bool IsComment(const std::string &line, const std::string &mark) {
        for (int i = 0; i < line.size(); i++) {
            if(line[i] == ' ') {
                continue;
            }
            else {
                bool flag = true;
                for (int j = 0; j < mark.size(); j++) {
                    if(line[i+j] != mark[j] || i+j >= line.size()) {
                        flag = false;
                        break;
                    }
                }
                return flag;
            }
        }
        return false;
    }

    /**
     * @brief 以sep为分隔符读取key和value
     * 
     * @param line 
     * @param sep 
     * @param key 
     * @param value 
     * @return true 
     * @return false 
     */
    bool GetKeyValue(const std::string &line, const std::string &sep, std::string *key, std::string *value) {
        // 跳过空格
        int pos_start = 0;
        for (; pos_start < line.size(); pos_start++) {
            if(line[pos_start] == ' ') {
                continue;
            }
            else {
                break;
            }
        }
        std::string new_line = line.substr(pos_start);
        int pos_sep = new_line.find(sep);
        if(pos_sep == std::string::npos) {
            // 没有找到分隔符
            return false;
        }
        *key = new_line.substr(0, pos_sep);
        *value = new_line.substr(pos_sep + 1);
        return true;
    }
};
