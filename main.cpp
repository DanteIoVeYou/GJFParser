#include "Controller.hpp"
void Usage() {
    printf("manual:\n\t\tgen [filename]\n");
}
int main(int argc, char* argv[])
{
    if(argc != 2) {
        Usage();
    }
    Controller *controller = new Controller(argv[1]);
    controller->Start();
    delete (controller);
}