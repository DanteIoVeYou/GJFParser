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

class Controller {
public:
    Controller(std::string gjf_filename):
        m_gjf_filename(gjf_filename),
        m_com_generator(m_gjf_filename),
        m_pbs_generator(m_gjf_filename)
    {}

    void Start() {
        m_com_generator.BuildFile();
        m_com_generator.GenerateFile();
        m_pbs_generator.BuildFile();
        m_pbs_generator.GenerateFile();
    }

    ~Controller() {}

protected:
    std::string m_gjf_filename;
    // GJFParser m_gjf_parser;
    COMGenerator m_com_generator;
    PBSGenerator m_pbs_generator;
};