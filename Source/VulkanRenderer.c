#define MZNT_IMPLEMENTATION
#include "RendererPrivate.h"
#include "VulkanRenderer.h"
#if MZNT_VULKAN

static inline void MZNT_Internal_LogVkResultOnFailure(VkResult result, utf8str fnCall, PNSLR_SourceCodeLocation loc)
{
    utf8str message = {0};
    // chatgpt generated
    switch (result)
    {
        case VK_SUCCESS: message = PNSLR_StringLiteral("Command successfully completed"); break;
        case VK_NOT_READY: message = PNSLR_StringLiteral("A fence or query has not yet completed"); break;
        case VK_TIMEOUT: message = PNSLR_StringLiteral("A wait operation has not completed in the specified time"); break;
        case VK_EVENT_SET: message = PNSLR_StringLiteral("An event is signaled"); break;
        case VK_EVENT_RESET: message = PNSLR_StringLiteral("An event is unsignaled"); break;
        case VK_INCOMPLETE: message = PNSLR_StringLiteral("A return array was too small for the result"); break;
        case VK_ERROR_OUT_OF_HOST_MEMORY: message = PNSLR_StringLiteral("Host memory allocation has failed"); break;
        case VK_ERROR_OUT_OF_DEVICE_MEMORY: message = PNSLR_StringLiteral("Device memory allocation has failed"); break;
        case VK_ERROR_INITIALIZATION_FAILED: message = PNSLR_StringLiteral("Initialization of an object could not be completed"); break;
        case VK_ERROR_DEVICE_LOST: message = PNSLR_StringLiteral("The logical or physical device has been lost"); break;
        case VK_ERROR_MEMORY_MAP_FAILED: message = PNSLR_StringLiteral("Mapping of a memory object has failed"); break;
        case VK_ERROR_LAYER_NOT_PRESENT: message = PNSLR_StringLiteral("A requested layer is not present or could not be loaded"); break;
        case VK_ERROR_EXTENSION_NOT_PRESENT: message = PNSLR_StringLiteral("A requested extension is not supported"); break;
        case VK_ERROR_FEATURE_NOT_PRESENT: message = PNSLR_StringLiteral("A requested feature is not supported"); break;
        case VK_ERROR_INCOMPATIBLE_DRIVER: message = PNSLR_StringLiteral("The requested version of Vulkan is not supported by the driver"); break;
        case VK_ERROR_TOO_MANY_OBJECTS: message = PNSLR_StringLiteral("Too many objects of the type have already been created"); break;
        case VK_ERROR_FORMAT_NOT_SUPPORTED: message = PNSLR_StringLiteral("A requested format is not supported on this device"); break;
        case VK_ERROR_FRAGMENTED_POOL: message = PNSLR_StringLiteral("A pool allocation has failed due to fragmentation"); break;
        case VK_ERROR_UNKNOWN: message = PNSLR_StringLiteral("An unknown error has occurred"); break;
        case VK_ERROR_OUT_OF_POOL_MEMORY: message = PNSLR_StringLiteral("A pool memory allocation has failed"); break;
        case VK_ERROR_INVALID_EXTERNAL_HANDLE: message = PNSLR_StringLiteral("An external handle is not valid"); break;
        case VK_ERROR_FRAGMENTATION: message = PNSLR_StringLiteral("A descriptor pool creation has failed due to fragmentation"); break;
        case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS: message = PNSLR_StringLiteral("A buffer creation or memory allocation failed due to invalid opaque capture address"); break;
        case VK_PIPELINE_COMPILE_REQUIRED: message = PNSLR_StringLiteral("Pipeline compilation required but not performed"); break;
        case VK_ERROR_NOT_PERMITTED: message = PNSLR_StringLiteral("Operation not permitted"); break;
        case VK_ERROR_SURFACE_LOST_KHR: message = PNSLR_StringLiteral("A surface is no longer available"); break;
        case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: message = PNSLR_StringLiteral("The requested window is already connected to another instance"); break;
        case VK_SUBOPTIMAL_KHR: message = PNSLR_StringLiteral("Swapchain no longer matches surface properties exactly, but is still usable"); break;
        case VK_ERROR_OUT_OF_DATE_KHR: message = PNSLR_StringLiteral("Swapchain is no longer compatible with the surface"); break;
        case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: message = PNSLR_StringLiteral("Display is incompatible with the requested mode"); break;
        case VK_ERROR_VALIDATION_FAILED_EXT: message = PNSLR_StringLiteral("Validation layer found an error"); break;
        case VK_ERROR_INVALID_SHADER_NV: message = PNSLR_StringLiteral("Invalid shader was provided to NV extension"); break;
        case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR: message = PNSLR_StringLiteral("The requested image usage is not supported"); break;
        case VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR: message = PNSLR_StringLiteral("Video picture layout not supported"); break;
        case VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR: message = PNSLR_StringLiteral("Video profile operation not supported"); break;
        case VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR: message = PNSLR_StringLiteral("Video profile format not supported"); break;
        case VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR: message = PNSLR_StringLiteral("Video profile codec not supported"); break;
        case VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR: message = PNSLR_StringLiteral("Video Std version not supported"); break;
        case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT: message = PNSLR_StringLiteral("DRM format modifier plane layout is invalid"); break;
        case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT: message = PNSLR_StringLiteral("Full screen exclusive mode lost"); break;
        case VK_THREAD_IDLE_KHR: message = PNSLR_StringLiteral("A deferred operation is not complete but there is currently no work for this thread"); break;
        case VK_THREAD_DONE_KHR: message = PNSLR_StringLiteral("A deferred operation is not complete but there is no work remaining for this thread"); break;
        case VK_OPERATION_DEFERRED_KHR: message = PNSLR_StringLiteral("A deferred operation was requested and will be completed later"); break;
        case VK_OPERATION_NOT_DEFERRED_KHR: message = PNSLR_StringLiteral("A deferred operation was not deferred and has been completed"); break;
        case VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR: message = PNSLR_StringLiteral("Parameters for a video Std are invalid"); break;
        case VK_ERROR_COMPRESSION_EXHAUSTED_EXT: message = PNSLR_StringLiteral("Compression resources are exhausted"); break;
        case VK_INCOMPATIBLE_SHADER_BINARY_EXT: message = PNSLR_StringLiteral("Shader binary is incompatible"); break;
        case VK_PIPELINE_BINARY_MISSING_KHR: message = PNSLR_StringLiteral("Pipeline binary is missing"); break;
        case VK_ERROR_NOT_ENOUGH_SPACE_KHR: message = PNSLR_StringLiteral("Not enough space for pipeline binary cache"); break;
        case VK_RESULT_MAX_ENUM: message = PNSLR_StringLiteral("Max enum value, do not use"); break;
        default: message = PNSLR_StringLiteral("Unknown VkResult value"); break;
    }

    if (result != VK_SUCCESS)
    {
        PNSLR_LogEf(PNSLR_StringLiteral("Vk error: $ from $"),
                    PNSLR_FmtArgs(
                        PNSLR_FmtString(message),
                        PNSLR_FmtString(fnCall)
                    ),
                    loc);

        FORCE_DBG_TRAP;
    }
}

#define MZNT_INTERNAL_VK_CHECKED_CALL(call) \
    MZNT_Internal_LogVkResultOnFailure((call), PNSLR_StringLiteral(#call), PNSLR_GET_LOC())

// returns number of unique queues
PNSLR_ArraySlice(VkDeviceQueueCreateInfo) MZNT_Internal_SelectVkQueueFamilies(VkPhysicalDevice physDev, VkSurfaceKHR surfaceToPresent, u32* gfxQueue, u32* presQueue, PNSLR_Allocator tempAllocator)
{
    if (!gfxQueue || !presQueue) FORCE_DBG_TRAP;

    *gfxQueue = U32_MAX; *presQueue = U32_MAX;

    // get all queue families
    PNSLR_ArraySlice(VkQueueFamilyProperties) queueFamilies;
    {
        u32 queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physDev, &queueFamilyCount, nil);
        queueFamilies = PNSLR_MakeSlice(VkQueueFamilyProperties, queueFamilyCount, false, tempAllocator, PNSLR_GET_LOC(), nil);
        vkGetPhysicalDeviceQueueFamilyProperties(physDev, &queueFamilyCount, queueFamilies.data);
        queueFamilies.count = (i64) queueFamilyCount;
    }

    for (i64 i = 0; i < queueFamilies.count; i++)
    {
        VkQueueFlags flags = queueFamilies.data[i].queueFlags;

        if ((flags & VK_QUEUE_GRAPHICS_BIT) && *gfxQueue == U32_MAX)
            *gfxQueue = (u32) i;

        VkBool32 supportsPresent = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(physDev, (u32) i, surfaceToPresent, &supportsPresent);
        if (supportsPresent && *presQueue == U32_MAX)
            *presQueue = (u32) i;

        if (*gfxQueue != U32_MAX && *presQueue != U32_MAX)
            break;
    }

    if (*gfxQueue == U32_MAX || *presQueue == U32_MAX)
    {
        PNSLR_LogE(PNSLR_StringLiteral("Failed to find required queue families on physical device"), PNSLR_GET_LOC());
        FORCE_DBG_TRAP;
    }

    // create queue create infos
    u32 queueCount = (*gfxQueue == *presQueue) ? 1 : 2;
    PNSLR_ArraySlice(VkDeviceQueueCreateInfo) queueCreateInfos = PNSLR_MakeSlice(VkDeviceQueueCreateInfo, queueCount, false, tempAllocator, PNSLR_GET_LOC(), nil);

    float queuePriority = 1.0f;
    queueCreateInfos.data[0] = (VkDeviceQueueCreateInfo)
    {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = *gfxQueue,
        .queueCount = 1,
        .pQueuePriorities = &queuePriority,
    };

    if (queueCount == 2)
    {
        queueCreateInfos.data[1] = (VkDeviceQueueCreateInfo)
        {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = *presQueue,
            .queueCount = 1,
            .pQueuePriorities = &queuePriority,
        };
    }

    return queueCreateInfos;
}

MZNT_VulkanRenderer* MZNT_CreateRenderer_Vulkan(MZNT_RendererConfiguration config, PNSLR_Allocator tempAllocator)
{
    MZNT_INTERNAL_VK_CHECKED_CALL(volkInitialize());

    MZNT_VulkanRenderer* output = PNSLR_New(MZNT_VulkanRenderer, config.allocator, PNSLR_GET_LOC(), nil);
    if (!output) FORCE_DBG_TRAP;

    output->parent.type      = MZNT_RendererType_Vulkan;
    output->parent.allocator = config.allocator;
    output->parent.appHandle = config.appHandle;

    #if PNSLR_DBG
        const char* enabledLayers[] = {"VK_LAYER_KHRONOS_validation"};
        u32 layerCount = 1;
    #endif

    const char* enabledExtensions[] =
    {
        VK_KHR_SURFACE_EXTENSION_NAME,
        #if PNSLR_WINDOWS
            VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
        #elif PNSLR_LINUX
            VK_KHR_XCB_SURFACE_EXTENSION_NAME,
        #elif PNSLR_ANDROID
            VK_KHR_ANDROID_SURFACE_EXTENSION_NAME,
        #elif PNSLR_APPLE
            VK_EXT_METAL_SURFACE_EXTENSION_NAME,
        #endif
        #if PNSLR_DBG
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
        #endif
    };

    u32 extensionCount = sizeof(enabledExtensions) / sizeof(char*);

    VkApplicationInfo appInfo =
    {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = nil,
        .pApplicationName = PNSLR_CStringFromString(config.appName, tempAllocator),
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "M_U_Z_E_N_T",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = VK_API_VERSION_1_3, // fuck it, we dyna-rendering
    };

    VkInstanceCreateInfo createInfo =
    {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = nil,
        .flags = 0,
        .pApplicationInfo = &appInfo,
        #if PNSLR_DBG
            .enabledLayerCount = layerCount,
            .ppEnabledLayerNames = enabledLayers,
        #endif
        .enabledExtensionCount = extensionCount,
        .ppEnabledExtensionNames = enabledExtensions,
    };

    MZNT_INTERNAL_VK_CHECKED_CALL(vkCreateInstance(&createInfo, nil, &output->instance));

    volkLoadInstanceOnly(output->instance);

    u32 deviceCount = 0;
    vkEnumeratePhysicalDevices(output->instance, &deviceCount, nil);
    PNSLR_ArraySlice(VkPhysicalDevice) devices = PNSLR_MakeSlice(VkPhysicalDevice, deviceCount, false, tempAllocator, PNSLR_GET_LOC(), nil);
    vkEnumeratePhysicalDevices(output->instance, &deviceCount, devices.data);
    devices.count = (i64) deviceCount;

    VkPhysicalDevice selectedDevice = VK_NULL_HANDLE;
    for (i64 i = 0; i < devices.count; i++)
    {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(devices.data[i], &deviceProperties);

        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            selectedDevice = devices.data[i];
            break;
        }

        if (selectedDevice == VK_NULL_HANDLE)
            selectedDevice = devices.data[i];
    }

    output->physicalDevice = selectedDevice;

    VkSurfaceKHR tempSurfaceForQueueSelect = VK_NULL_HANDLE;
    #if PNSLR_WINDOWS
        HWND tempWindow = CreateWindowA("STATIC", "temp", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1, 1, nil, nil, (HINSTANCE) (uintptr_t) config.appHandle.handle, nil);
        VkWin32SurfaceCreateInfoKHR surfaceCreateInfo =
        {
            .sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
            .hinstance = (HINSTANCE) (uintptr_t) config.appHandle.handle,
            .hwnd      = tempWindow,
        };

        MZNT_INTERNAL_VK_CHECKED_CALL(vkCreateWin32SurfaceKHR(output->instance, &surfaceCreateInfo, nil, &tempSurfaceForQueueSelect));
    #else
        #error "unimplemented"
    #endif

    u32 gfxQueueFamIdx = U32_MAX, presQueueFamIdx = U32_MAX;
    PNSLR_ArraySlice(VkDeviceQueueCreateInfo) qcis = MZNT_Internal_SelectVkQueueFamilies(selectedDevice, tempSurfaceForQueueSelect, &gfxQueueFamIdx, &presQueueFamIdx, tempAllocator);

    vkDestroySurfaceKHR(output->instance, tempSurfaceForQueueSelect, nil);
    #if PNSLR_WINDOWS
        DestroyWindow(tempWindow);
    #else
        #error "unimplemented"
    #endif

    const char* enabledDeviceExtensions[] =
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
    };

    u32 enabledDeviceExtensionCount = sizeof(enabledDeviceExtensions) / sizeof(char*);

    VkDeviceCreateInfo deviceCreateInfo =
    {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount = (u32) qcis.count,
        .pQueueCreateInfos = qcis.data,
        .enabledExtensionCount = enabledDeviceExtensionCount,
        .ppEnabledExtensionNames = enabledDeviceExtensions,
        .pEnabledFeatures = nil,
    };

    MZNT_INTERNAL_VK_CHECKED_CALL(vkCreateDevice(selectedDevice, &deviceCreateInfo, nil, &output->device));

    volkLoadDevice(output->device);

    VkDeviceQueueInfo2 gfxQueueInfo  = {.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2, .queueFamilyIndex = gfxQueueFamIdx,  .queueIndex = 0};
    VkDeviceQueueInfo2 presQueueInfo = {.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2, .queueFamilyIndex = presQueueFamIdx, .queueIndex = 0};

    vkGetDeviceQueue2(output->device, &gfxQueueInfo,  &output->gfxQueue);
    vkGetDeviceQueue2(output->device, &presQueueInfo, &output->presQueue);

    return output;
}

b8 MZNT_DestroyRenderer_Vulkan(MZNT_VulkanRenderer* renderer, PNSLR_Allocator tempAllocator)
{
    if (!renderer) return false;

    vkDestroyDevice(renderer->device, nil);

    vkDestroyInstance(renderer->instance, nil);

    PNSLR_Delete(renderer, renderer->parent.allocator, PNSLR_GET_LOC(), nil);

    volkFinalize();

    return true;
}

void MZNT_Internal_CreateVkSwapchainImagesAndViews(MZNT_VulkanRendererSurface* surface, PNSLR_Allocator tempAllocator)
{
    // get swapchain images
    {
        u32 swapchainImageCount = 0;
        MZNT_INTERNAL_VK_CHECKED_CALL(vkGetSwapchainImagesKHR(surface->renderer->device, surface->swapchain, &swapchainImageCount, nil));
        surface->swapchainImages = PNSLR_MakeSlice(VkImage, swapchainImageCount, false, surface->renderer->parent.allocator, PNSLR_GET_LOC(), nil);
        MZNT_INTERNAL_VK_CHECKED_CALL(vkGetSwapchainImagesKHR(surface->renderer->device, surface->swapchain, &swapchainImageCount, surface->swapchainImages.data));
        surface->swapchainImages.count = (i64) swapchainImageCount;
    }

    {
        surface->swapchainImageViews = PNSLR_MakeSlice(VkImageView, surface->swapchainImages.count, false, surface->renderer->parent.allocator, PNSLR_GET_LOC(), nil);
        for (i64 i = 0; i < surface->swapchainImages.count; i++)
        {
            VkImageViewCreateInfo ivCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .image = surface->swapchainImages.data[i],
                .viewType = VK_IMAGE_VIEW_TYPE_2D,
                .format = VK_FORMAT_B8G8R8A8_UNORM,
                .components = {
                    .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                    .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                    .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                    .a = VK_COMPONENT_SWIZZLE_IDENTITY,
                },
                .subresourceRange = {
                    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                    .baseMipLevel = 0,
                    .levelCount = 1,
                    .baseArrayLayer = 0,
                    .layerCount = 1,
                },
            };

            MZNT_INTERNAL_VK_CHECKED_CALL(vkCreateImageView(surface->renderer->device, &ivCreateInfo, nil, &(surface->swapchainImageViews.data[i])));
        }
    }
}

MZNT_VulkanRendererSurface* MZNT_CreateRendererSurface_Vulkan(MZNT_VulkanRenderer* renderer, MZNT_WindowHandle windowHandle, PNSLR_Allocator tempAllocator)
{
    if (!renderer) return nil;

    MZNT_VulkanRendererSurface* output = PNSLR_New(MZNT_VulkanRendererSurface, renderer->parent.allocator, PNSLR_GET_LOC(), nil);
    if (!output) FORCE_DBG_TRAP;

    output->parent.type = MZNT_RendererType_Vulkan;
    output->renderer    = renderer;

    #if PNSLR_WINDOWS
        VkWin32SurfaceCreateInfoKHR createInfo =
        {
            .sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
            .hinstance = (HINSTANCE)(uintptr_t) (renderer->parent.appHandle.handle),
            .hwnd      = (HWND)(uintptr_t) (windowHandle.handle),
        };

        MZNT_INTERNAL_VK_CHECKED_CALL(vkCreateWin32SurfaceKHR(renderer->instance, &createInfo, nil, &output->surface));
    #else
        #error "unimplemented"
    #endif

    VkSurfaceCapabilitiesKHR surfaceCaps;
    MZNT_INTERNAL_VK_CHECKED_CALL(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(renderer->physicalDevice, output->surface, &surfaceCaps));

    PNSLR_ArraySlice(VkSurfaceFormatKHR) formats;
    {
        u32 fmtCount = 0;
        MZNT_INTERNAL_VK_CHECKED_CALL(vkGetPhysicalDeviceSurfaceFormatsKHR(renderer->physicalDevice, output->surface, &fmtCount, nil));
        formats = PNSLR_MakeSlice(VkSurfaceFormatKHR, fmtCount, false, tempAllocator, PNSLR_GET_LOC(), nil);
        MZNT_INTERNAL_VK_CHECKED_CALL(vkGetPhysicalDeviceSurfaceFormatsKHR(renderer->physicalDevice, output->surface, &fmtCount, formats.data));
        formats.count = (i64) fmtCount;
    }

    u32 imageCount = surfaceCaps.minImageCount + 1;
    if (surfaceCaps.maxImageCount > 0 && imageCount > surfaceCaps.maxImageCount)
        imageCount = surfaceCaps.maxImageCount;

    VkSwapchainCreateInfoKHR swapchainCI = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = output->surface,
        .minImageCount = imageCount,
        .imageFormat = VK_FORMAT_B8G8R8A8_UNORM,
        .imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
        .imageExtent = surfaceCaps.currentExtent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .preTransform = surfaceCaps.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = VK_PRESENT_MODE_FIFO_KHR, // always available
        .clipped = VK_TRUE,
    };

    MZNT_INTERNAL_VK_CHECKED_CALL(vkCreateSwapchainKHR(renderer->device, &swapchainCI, nil, &(output->swapchain)));

    MZNT_Internal_CreateVkSwapchainImagesAndViews(output, tempAllocator);

    return output;
}

b8 MZNT_DestroyRendererSurface_Vulkan(MZNT_VulkanRendererSurface* surface, PNSLR_Allocator tempAllocator)
{
    if (!surface) return false;
    if (!surface->renderer) FORCE_DBG_TRAP;

    for (i64 i = 0; i < surface->swapchainImageViews.count; i++)
    {
        vkDestroyImageView(surface->renderer->device, surface->swapchainImageViews.data[i], nil);
    }

    PNSLR_FreeSlice(&(surface->swapchainImageViews), surface->renderer->parent.allocator, PNSLR_GET_LOC(), nil);

    PNSLR_FreeSlice(&(surface->swapchainImages), surface->renderer->parent.allocator, PNSLR_GET_LOC(), nil);

    vkDestroySwapchainKHR(surface->renderer->device, surface->swapchain, nil);
    vkDestroySurfaceKHR(surface->renderer->instance, surface->surface, nil);

    PNSLR_Delete(surface, surface->renderer->parent.allocator, PNSLR_GET_LOC(), nil);

    return true;
}

b8 MZNT_ResizeRendererSurface_Vulkan(MZNT_VulkanRendererSurface* surface, u16 width, u16 height, PNSLR_Allocator tempAllocator)
{
    if (!surface) return false;
    if (!surface->renderer) FORCE_DBG_TRAP;

    vkDeviceWaitIdle(surface->renderer->device);

    for (i64 i = 0; i < surface->swapchainImageViews.count; i++)
        vkDestroyImageView(surface->renderer->device, surface->swapchainImageViews.data[i], nil);

    PNSLR_FreeSlice(&(surface->swapchainImageViews), surface->renderer->parent.allocator, PNSLR_GET_LOC(), nil);
    PNSLR_FreeSlice(&(surface->swapchainImages), surface->renderer->parent.allocator, PNSLR_GET_LOC(), nil);

    VkSurfaceCapabilitiesKHR surfaceCaps;
    MZNT_INTERNAL_VK_CHECKED_CALL(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(surface->renderer->physicalDevice, surface->surface, &surfaceCaps));

    u32 imageCount = surfaceCaps.minImageCount + 1;
    if (surfaceCaps.maxImageCount > 0 && imageCount > surfaceCaps.maxImageCount)
        imageCount = surfaceCaps.maxImageCount;

    // ignore input width/height, use current extent from surface capabilities
    VkSwapchainCreateInfoKHR swapchainCI = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = surface->surface,
        .minImageCount = imageCount,
        .imageFormat = VK_FORMAT_B8G8R8A8_UNORM,
        .imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
        .imageExtent = surfaceCaps.currentExtent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .preTransform = surfaceCaps.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = VK_PRESENT_MODE_FIFO_KHR,
        .clipped = VK_TRUE,
        .oldSwapchain = surface->swapchain,
    };

    MZNT_INTERNAL_VK_CHECKED_CALL(vkCreateSwapchainKHR(surface->renderer->device, &swapchainCI, nil, &(surface->swapchain)));
    vkDestroySwapchainKHR(surface->renderer->device, swapchainCI.oldSwapchain, nil);

    MZNT_Internal_CreateVkSwapchainImagesAndViews(surface, tempAllocator);
    return true;
}
#endif
