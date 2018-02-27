#include "HeightField.h"                                                                                                        //      the HeightField class
//#include "IsosurfaceWindow.h"
#include <string.h>
#include <stdio.h>

int main(int argc, char **argv)
{
//    IsosurfaceWindow * window = new IsosurfaceWindow(argc, argv, "test");

    HeightField *field;
    field = new HeightField(argc, argv);

    char *str = strdup(argv[2]);
    field->PrintValidValues(str);
//    field->PrintValidValues("test");
    return 0;
}
