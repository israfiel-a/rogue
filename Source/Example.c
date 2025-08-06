#include <Waterlily.h>

int main(int argc, char **argv)
{
    char *argvExpanded[argc + 2];
    for(size_t i = 0; i < (size_t)argc; ++i)
        argvExpanded[i] = argv[i];
    argvExpanded[argc] = "Example";
    argvExpanded[argc + 1] = "0";

    bool success = waterlily_initialize(argc + 2, argvExpanded);
    if (!success) return -1;

    waterlily_run();

    waterlily_cleanup();
    return 0;
}
