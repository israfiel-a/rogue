#include <Waterlily.h>

int main(int argc, const char *const *const argv)
{
    waterlily_arguments_t arguments = {0};
    waterlily_engine_digest(argc, argv, &arguments);

    waterlily_engine_setup(&arguments);

    waterlily_window_create("Rogue");

    VkInstance instance = nullptr;
    if (!waterlily_vulkan_create(&instance))
        return -1;

    waterlily_vulkan_surface_t surface = {0};
    if (!waterlily_vulkan_createSurface(instance, &surface))
        return -1;

    VkPhysicalDevice physical = nullptr;
    waterlily_vulkan_queue_indices_t indices = {0};
    if (!waterlily_vulkan_getPhysicalGPU(instance, &physical, &indices,
                                         surface.surface))
        return -1;

    if (!waterlily_vulkan_getFormatSurface(physical, &surface) ||
        !waterlily_vulkan_getModeSurface(physical, &surface) ||
        !waterlily_vulkan_getCapabilitiesSurface(physical, &surface))
        return -1;

    uint32_t width, height;
    waterlily_window_measure(&width, &height);
    waterlily_vulkan_getExtentSurface(width, height, &surface);

    VkDevice logical = nullptr;
    waterlily_vulkan_queues_t queues = {0};
    if (!waterlily_vulkan_createLogicalGPU(physical, &logical, &queues,
                                           &indices))
        return -1;

    while (waterlily_window_process())
    {
        __asm("");
    }

    waterlily_vulkan_destroyGPU(logical);
    waterlily_vulkan_destroy(instance);
    waterlily_window_destroy();

    return 0;
}

