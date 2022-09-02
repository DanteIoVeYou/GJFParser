#include "Controller.hpp"

/**
 * @brief 使用手册
 * 
 */
void Usage() {
    printf("manual:\n\t\tgen [filename]\n");
}

/**
 * @brief 
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char* argv[])
{
    if(argc != 2) {
        Usage();
        exit(1);
    }
    Controller *controller = new Controller(argv[1]);
    controller->Start();
    delete (controller);
    return 0;
}