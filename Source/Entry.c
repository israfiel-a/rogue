#include <Waterlily.h>

int main(int argc, const char *const *const argv) {
    waterlily_arguments_t arguments = {0};
    waterlily_engine_digest(argc, argv, &arguments);

    waterlily_engine_setup(&arguments);

    waterlily_window_create("Rogue");

    VkInstance instance = nullptr;
    if (!waterlily_vulkan_create(&instance)) return -1;
    VkSurfaceKHR surface = nullptr;
    if (!waterlily_vulkan_link(instance, &surface)) return -1;

    while (waterlily_window_process()) {
        __asm("");
    }

    waterlily_vulkan_destroy(instance);
    waterlily_window_destroy();

    return 0;
}
