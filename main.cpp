#include "Controller.hpp"
#include <algorithm>

/**
 * @brief 使用手册
 *
 */
void Usage() {
    printf("manual:\n\t\tgen [option] [filename]\noption:\n\t\t-opt: optimization\n\t\t-ts: transition\n");
}

/**
 * @brief 
 * gen -opt filename.gjf
 * gen -ts filename.gjf
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char* argv[])
{
    if(argc != 3 || argc != 5 || argc != 7) {
        Usage();
        exit(1);
    }
    std::vector<std::string> cmds;
    int option = 0;
    int charge = 0;
    int spin = 0;
    bool charge_flag = false;
    bool spin_flag = false;
    for (int i = 0; i < argc; i++)
    {
        cmds.push_back(argv[i]);
    }
    if(cmds[0] != "gen") {
        Usage();
        exit(2);
    }
    else {
        if(cmds[1] == "-opt") {
            option = OPT;
        }
        else if(cmds[1] == "-ts") {
            option = TS;
        }
        else {
            Usage();
            exit(3);
        }
    }
    for (int i = 3; i < cmds.size(); i += 2) {
        if(cmds[i] == "-charge") {
            if(i + 1 >= cmds.size() || !Utils::IsInteger(cmds[i+1])) {
                Usage();
                exit(4);
            }
            else {
                charge_flag = true;
                charge = atoi(cmds[i + 1].c_str());
            }
        }
        else if(cmds[i] == "-spin") {
            if(i + 1 >= cmds.size() || !Utils::IsInteger(cmds[i+1])) {
                Usage();
                exit(5);
            }
            else {
                spin_flag = true;
                spin = atoi(cmds[i + 1].c_str());
            }
        }
        else {
            Usage();
            exit(3);
        }
    }
    Controller *controller = new Controller(cmds[2], option, charge_flag, charge, spin_flag, spin);
    controller->Start();
    delete (controller);
    return 0;
}