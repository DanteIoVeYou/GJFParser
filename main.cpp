#include "COMGenerator.hpp"
int main()
{
    COMGenerator comgen("B.gjf");
    comgen.GenerateFile();
    comgen.BuildFile();
}