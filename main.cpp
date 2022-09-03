#include "Controller.hpp"

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
    if(argc != 3) {
        Usage();
        exit(1);
    }
    std::vector<std::string> cmds;
    int option;
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
    Controller *controller = new Controller(argv[2], option);
    controller->Start();
    delete (controller);
    return 0;
}