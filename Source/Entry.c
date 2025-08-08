#include <Waterlily.h>

#define FILENAME "Entry.c"

int main(int argc, const char *const *const argv)
{
    waterlily_context_t context = {0};

    if (!waterlily_engine_digest(argc, argv, &context))
        return -1;

    if (!waterlily_engine_setup(&context))
        return -1;

    if (!waterlily_window_create("Rogue", &context))
        return -1;

    if (!waterlily_vulkan_create(&context))
        return -1;

    if (!waterlily_vulkan_createSurface(&context))
        return -1;

    if (!waterlily_vulkan_getPhysicalGPU(&context))
        return -1;

    if (!waterlily_vulkan_getFormatSurface(&context) ||
        !waterlily_vulkan_getModeSurface(&context) ||
        !waterlily_vulkan_getCapabilitiesSurface(&context))
        return -1;
    waterlily_vulkan_getExtentSurface(&context);

    if (!waterlily_vulkan_createLogicalGPU(&context))
        return -1;

    if (!waterlily_vulkan_createSwapchain(&context))
        return -1;

    if (!waterlily_vulkan_partitionSwapchain(&context))
        return -1;

    VkPipelineShaderStageCreateInfo stages[2];
    const char *const stageNames[2] = {"default.vert", "default.frag"};
    if (!waterlily_vulkan_setupShadersPipeline(&context, stageNames, 2, stages))
        return -1;

    waterlily_vulkan_pipeline_info_t info = {0};
    waterlily_vulkan_fillInfoPipeline(&info);

    if (!waterlily_vulkan_createLayoutPipeline(&context) ||
        !waterlily_vulkan_createRenderpassPipeline(&context) ||
        !waterlily_vulkan_createPipeline(&context, stages, 2, &info))
        return -1;

    if (!waterlily_vulkan_createFramebuffersSwapchain(&context))
        return -1;

    if (!waterlily_vulkan_createBuffersCommand(&context) ||
        !waterlily_vulkan_createSyncsCommand(&context))
        return -1;

    while (waterlily_window_process(&context))
    {
        if (context.window.resized &&
            !waterlily_vulkan_recreateSwapchain(&context))
            return -1;

        if (!waterlily_vulkan_render(&context))
            return -1;
    }

    waterlily_vulkan_sync(&context);
    waterlily_vulkan_destroyBuffers(&context);
    waterlily_vulkan_destroySyncs(&context);
    waterlily_vulkan_destroySwapchain(&context);
    waterlily_vulkan_destroyPipeline(&context);
    waterlily_vulkan_destroyGPU(&context);
    waterlily_vulkan_destroySurface(&context);
    waterlily_vulkan_destroy(&context);
    waterlily_window_destroy(&context);

    return 0;
}

