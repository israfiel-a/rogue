#include <waterlily/waterlily.h>

bool waterlily_application(waterlily_context_t *context)
{
    if (!waterlily_run(context))
        return false;
    return true;
}

