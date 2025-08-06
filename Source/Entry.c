#include <Waterlily.h>

int main(int argc, const char *const *const argv)
{
    waterlily_arguments_t arguments = {0};
    waterlily_engine_digest(argc, argv, &arguments);

    waterlily_engine_setup(&arguments);

    waterlily_window_create("Rogue");
    while(waterlily_window_process())
    {
        __asm("");
    }
    waterlily_window_destroy();

    return 0;
}
