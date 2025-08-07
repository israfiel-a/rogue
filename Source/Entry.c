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

    VkSwapchainKHR swapchain = nullptr;
    uint32_t imageCount = 0;
    if (!waterlily_vulkan_createSwapchain(logical, &imageCount, &swapchain,
                                          &surface, &indices))
        return -1;

    VkImageView images[imageCount];
    if (!waterlily_vulkan_partitionSwapchain(logical, &surface, swapchain,
                                             imageCount, images))
        return -1;

    waterlily_vulkan_graphics_pipeline_t pipeline = {0};

    VkPipelineShaderStageCreateInfo stages[2];
    const char *const stageNames[2] = {"default.vert", "default.frag"};
    if (!waterlily_vulkan_setupShadersPipeline(logical, stageNames, 2, stages))
        return -1;

    waterlily_vulkan_pipeline_info_t info = {0};
    waterlily_vulkan_fillInfoPipeline(&surface, &info);

    if (!waterlily_vulkan_createLayoutPipeline(logical, &pipeline) ||
        !waterlily_vulkan_createRenderpassPipeline(logical, &pipeline,
                                                   &surface) ||
        !waterlily_vulkan_createPipeline(logical, &pipeline, stages, 2, &info))
        return -1;

    VkFramebuffer framebuffers[imageCount];
    if (!waterlily_vulkan_createFramebuffersSwapchain(
            logical, &surface, pipeline.renderpass, imageCount, images,
            framebuffers))
        return -1;

    VkCommandPool pool;
    VkCommandBuffer buffers[WATERLILY_CONCURRENT_FRAMES];
    VkFence fences[WATERLILY_CONCURRENT_FRAMES];
    VkSemaphore imageAvailableSemaphores[WATERLILY_CONCURRENT_FRAMES];
    VkSemaphore renderFinishedSemaphores[WATERLILY_CONCURRENT_FRAMES];
    if (!waterlily_vulkan_createBuffersCommand(logical, &indices, &pool,
                                               buffers) ||
        !waterlily_vulkan_createSyncsCommand(logical, fences,
                                             imageAvailableSemaphores,
                                             renderFinishedSemaphores))
        return -1;

    uint32_t currentFrame = 0;
    while (waterlily_window_process())
    {
        if (!waterlily_vulkan_render(
                logical, &surface, &indices, &queues, &pipeline,
                buffers[currentFrame], fences[currentFrame],
                imageAvailableSemaphores[currentFrame],
                renderFinishedSemaphores[currentFrame], &swapchain, &imageCount,
                framebuffers, images))
            return -1;
        if (!waterlily_vulkan_sync(logical))
            return -1;

        currentFrame = (currentFrame + 1) % WATERLILY_CONCURRENT_FRAMES;
    }

    waterlily_vulkan_destroyBuffers(logical, pool);
    waterlily_vulkan_destroySyncs(logical, imageAvailableSemaphores,
                                  renderFinishedSemaphores, fences);
    waterlily_vulkan_destroySwapchain(logical, imageCount, framebuffers, images,
                                      swapchain);
    waterlily_vulkan_destroyPipeline(logical, &pipeline);
    waterlily_vulkan_destroyGPU(logical);
    waterlily_vulkan_destroySurface(instance, &surface);
    waterlily_vulkan_destroy(instance);
    waterlily_window_destroy();

    return 0;
}

