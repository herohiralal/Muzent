#define MZNT_IMPLEMENTATION
#include "RendererPrivate.h"
#include "VulkanRenderer.h"
#if MZNT_VULKAN

#define INLINED_FILE_INCLUSION_NAME k_TriangleShader
#include "Shaders/triangle_spv.c"

VKAPI_ATTR VkBool32 VKAPI_CALL MZNT_Internal_VkDebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT types,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData)
{
    PNSLR_LoggerLevel lvl = (PNSLR_LoggerLevel) {0};
    if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)        lvl = PNSLR_LoggerLevel_Error;
    else if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) lvl = PNSLR_LoggerLevel_Warn;
    else if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)    lvl = PNSLR_LoggerLevel_Info;
    else if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) lvl = PNSLR_LoggerLevel_Debug;
    else                                                                 lvl = PNSLR_LoggerLevel_Debug;

    utf8str general = (types & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT) ? PNSLR_StringLiteral("[GENERAL]") : PNSLR_StringLiteral("");
    utf8str validation = (types & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) ? PNSLR_StringLiteral("[VALIDATION]") : PNSLR_StringLiteral("");
    utf8str performance = (types & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) ? PNSLR_StringLiteral("[PERFORMANCE]") : PNSLR_StringLiteral("");

    PNSLR_Logf(lvl, PNSLR_StringLiteral("VK DEBUG UTILS: $. $ $ $"),
        PNSLR_FmtArgs(
            PNSLR_FmtCString((cstring) pCallbackData->pMessage),
            PNSLR_FmtString(general),
            PNSLR_FmtString(validation),
            PNSLR_FmtString(performance),
        ),
        PNSLR_GET_LOC()
    );

    return VK_FALSE;
}

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

    b8 foundComputeSupport = false;
    for (i64 i = 0; i < queueFamilies.count; i++)
    {
        VkQueueFlags flags = queueFamilies.data[i].queueFlags;

        if (flags & VK_QUEUE_GRAPHICS_BIT)
        {
            b8 currentQueueSupportsCompute = !!(flags & VK_QUEUE_COMPUTE_BIT);
            if (*gfxQueue == U32_MAX || (!foundComputeSupport && currentQueueSupportsCompute))
            {
                *gfxQueue = (u32) i;
                foundComputeSupport = currentQueueSupportsCompute;
            }
        }

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

    #if PNSLR_DBG
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo =
        {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .pNext = nil,
            .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
            .messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
            .pfnUserCallback = MZNT_Internal_VkDebugCallback,
            .pUserData       = output,
        };

        MZNT_INTERNAL_VK_CHECKED_CALL(vkCreateDebugUtilsMessengerEXT(output->instance, &debugCreateInfo, nil, &output->debugMessenger));
    #endif

    u32 deviceCount = 0;
    vkEnumeratePhysicalDevices(output->instance, &deviceCount, nil);
    PNSLR_ArraySlice(VkPhysicalDevice) devices = PNSLR_MakeSlice(VkPhysicalDevice, deviceCount, false, tempAllocator, PNSLR_GET_LOC(), nil);
    vkEnumeratePhysicalDevices(output->instance, &deviceCount, devices.data);
    devices.count = (i64) deviceCount;

    VkPhysicalDevice selectedDevice = VK_NULL_HANDLE;
    for (i64 i = 0; i < devices.count; i++)
    {
        VkPhysicalDeviceVulkan13Features deviceFeatures13 = {.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES};
        VkPhysicalDeviceVulkan12Features deviceFeatures12 = {.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES, .pNext = &deviceFeatures13};
        VkPhysicalDeviceVulkan11Features deviceFeatures11 = {.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES, .pNext = &deviceFeatures12};
        VkPhysicalDeviceFeatures2 deviceFeatures = {.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2, .pNext = &deviceFeatures11};
        vkGetPhysicalDeviceFeatures2(devices.data[i], &deviceFeatures);

        VkPhysicalDeviceProperties2 deviceProperties = {.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2};
        vkGetPhysicalDeviceProperties2(devices.data[i], &deviceProperties);

        if (deviceProperties.properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
            deviceFeatures12.bufferDeviceAddress &&
            deviceFeatures12.descriptorIndexing &&
            deviceFeatures13.dynamicRendering &&
            deviceFeatures13.synchronization2)
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
    #elif PNSLR_ANDROID
        ANativeWindow* window = ((struct android_app*) config.appHandle.handle)->window;
        VkAndroidSurfaceCreateInfoKHR surfaceCreateInfo =
        {
            .sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,
            .window = window,
        };

        MZNT_INTERNAL_VK_CHECKED_CALL(vkCreateAndroidSurfaceKHR(output->instance, &surfaceCreateInfo, nil, &tempSurfaceForQueueSelect));
    #else
        #error "unimplemented"
    #endif

    PNSLR_ArraySlice(VkDeviceQueueCreateInfo) qcis = MZNT_Internal_SelectVkQueueFamilies(selectedDevice, tempSurfaceForQueueSelect, &(output->gfxQueueFamilyIndex), &(output->presQueueFamilyIndex), tempAllocator);

    vkDestroySurfaceKHR(output->instance, tempSurfaceForQueueSelect, nil);
    #if PNSLR_WINDOWS
        DestroyWindow(tempWindow);
    #elif PNSLR_ANDROID
        // nothing to do
    #else
        #error "unimplemented"
    #endif

    const char* enabledDeviceExtensions[] =
    {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_KHR_SPIRV_1_4_EXTENSION_NAME,
        VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
    };

    u32 enabledDeviceExtensionCount = sizeof(enabledDeviceExtensions) / sizeof(char*);

    VkPhysicalDeviceSynchronization2FeaturesKHR sync2Features = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES_KHR,
        .synchronization2 = VK_TRUE,
    };

    VkPhysicalDeviceDynamicRenderingFeatures dynaRendFeatauures = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES,
        .pNext = &sync2Features,
        .dynamicRendering = VK_TRUE,
    };

    VkPhysicalDeviceShaderDrawParametersFeatures shaderDrawParamsFeatures = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DRAW_PARAMETERS_FEATURES,
        .pNext = &dynaRendFeatauures,
        .shaderDrawParameters = VK_TRUE,
    };

    VkDeviceCreateInfo deviceCreateInfo =
    {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = &shaderDrawParamsFeatures,
        .queueCreateInfoCount = (u32) qcis.count,
        .pQueueCreateInfos = qcis.data,
        .enabledExtensionCount = enabledDeviceExtensionCount,
        .ppEnabledExtensionNames = enabledDeviceExtensions,
        .pEnabledFeatures = nil,
    };

    MZNT_INTERNAL_VK_CHECKED_CALL(vkCreateDevice(selectedDevice, &deviceCreateInfo, nil, &output->device));

    volkLoadDevice(output->device);

    VkDeviceQueueInfo2 gfxQueueInfo  = {.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2, .queueFamilyIndex = output->gfxQueueFamilyIndex,  .queueIndex = 0};
    VkDeviceQueueInfo2 presQueueInfo = {.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2, .queueFamilyIndex = output->presQueueFamilyIndex, .queueIndex = 0};

    vkGetDeviceQueue2(output->device, &gfxQueueInfo,  &output->gfxQueue);
    vkGetDeviceQueue2(output->device, &presQueueInfo, &output->presQueue);

    VmaVulkanFunctions vmaFns = {
        .vkGetInstanceProcAddr = vkGetInstanceProcAddr,
        .vkGetDeviceProcAddr = vkGetDeviceProcAddr,
    };

    VmaAllocatorCreateInfo allocatorInfo = {
        .physicalDevice = output->physicalDevice,
        .device = output->device,
        .instance = output->instance,
        .flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT,
        .pVulkanFunctions = &vmaFns,
    };

    vmaCreateAllocator(&allocatorInfo, &(output->vmaAllocator));

    // triangle shader pipeline
    {
        VkShaderModuleCreateInfo triangleShMCi = {
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .codeSize = (size_t) k_TriangleShaderSize,
            .pCode = (u32*) k_TriangleShaderContents,
        };

        MZNT_INTERNAL_VK_CHECKED_CALL(vkCreateShaderModule(output->device, &triangleShMCi, nil, &output->triangleShader.module));

        VkPipelineLayoutCreateInfo pipelineLayoutInfo = {.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
        MZNT_INTERNAL_VK_CHECKED_CALL(vkCreatePipelineLayout(output->device, &pipelineLayoutInfo, nil, &output->triangleShader.layout));
    }

    return output;
}

b8 MZNT_DestroyRenderer_Vulkan(MZNT_VulkanRenderer* renderer, PNSLR_Allocator tempAllocator)
{
    if (!renderer) return false;

    MZNT_INTERNAL_VK_CHECKED_CALL(vkDeviceWaitIdle(renderer->device));

    vkDestroyPipelineLayout(renderer->device, renderer->triangleShader.layout, nil);
    vkDestroyShaderModule(renderer->device, renderer->triangleShader.module, nil);

    vmaDestroyAllocator(renderer->vmaAllocator);

    vkDestroyDevice(renderer->device, nil);

    #if PNSLR_DBG
        vkDestroyDebugUtilsMessengerEXT(renderer->instance, renderer->debugMessenger, nil);
    #endif

    vkDestroyInstance(renderer->instance, nil);

    PNSLR_Delete(renderer, renderer->parent.allocator, PNSLR_GET_LOC(), nil);

    volkFinalize();

    return true;
}

void MZNT_Internal_CreateVkSwapchain(MZNT_VulkanRendererSurface* surface, PNSLR_Allocator tempAllocator)
{
    VkSurfaceCapabilitiesKHR surfaceCaps;
    MZNT_INTERNAL_VK_CHECKED_CALL(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(surface->renderer->physicalDevice, surface->surface, &surfaceCaps));

    PNSLR_ArraySlice(VkSurfaceFormatKHR) formats;
    {
        u32 fmtCount = 0;
        MZNT_INTERNAL_VK_CHECKED_CALL(vkGetPhysicalDeviceSurfaceFormatsKHR(surface->renderer->physicalDevice, surface->surface, &fmtCount, nil));
        formats = PNSLR_MakeSlice(VkSurfaceFormatKHR, fmtCount, false, tempAllocator, PNSLR_GET_LOC(), nil);
        MZNT_INTERNAL_VK_CHECKED_CALL(vkGetPhysicalDeviceSurfaceFormatsKHR(surface->renderer->physicalDevice, surface->surface, &fmtCount, formats.data));
        formats.count = (i64) fmtCount;
    }

    u32 imageCount = surfaceCaps.minImageCount + 1;
    if (surfaceCaps.maxImageCount > 0 && imageCount > surfaceCaps.maxImageCount)
        imageCount = surfaceCaps.maxImageCount;

    u32 presentModesCount = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(surface->renderer->physicalDevice, surface->surface, &presentModesCount, nil);
    PNSLR_ArraySlice(VkPresentModeKHR) presentModes = PNSLR_MakeSlice(VkPresentModeKHR, presentModesCount, false, tempAllocator, PNSLR_GET_LOC(), nil);
    vkGetPhysicalDeviceSurfacePresentModesKHR(surface->renderer->physicalDevice, surface->surface, &presentModesCount, presentModes.data);

    VkPresentModeKHR selectedPresentMode = VK_PRESENT_MODE_FIFO_KHR; // always available
    for (i64 i = 0; i < presentModes.count; i++)
    {
        if (presentModes.data[i] == VK_PRESENT_MODE_MAILBOX_KHR) // best quality
        {
            selectedPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
            break;
        }
    }

    u32 formatCount = 0;
    MZNT_INTERNAL_VK_CHECKED_CALL(vkGetPhysicalDeviceSurfaceFormatsKHR(surface->renderer->physicalDevice, surface->surface, &formatCount, nil));
    PNSLR_ArraySlice(VkSurfaceFormatKHR) surfaceFormats = PNSLR_MakeSlice(VkSurfaceFormatKHR, formatCount, false, tempAllocator, PNSLR_GET_LOC(), nil);
    MZNT_INTERNAL_VK_CHECKED_CALL(vkGetPhysicalDeviceSurfaceFormatsKHR(surface->renderer->physicalDevice, surface->surface, &formatCount, surfaceFormats.data));

    if (formatCount == 0)
    {
        PNSLR_LogE(PNSLR_StringLiteral("Failed to get any surface formats for swapchain"), PNSLR_GET_LOC());
        FORCE_DBG_TRAP;
    }

    surface->swapchainImageFormat = surfaceFormats.data[0];
    for (i64 i = 0; i < surfaceFormats.count; i++)
    {
        if (surfaceFormats.data[i].format == VK_FORMAT_B8G8R8A8_UNORM &&
            surfaceFormats.data[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            surface->swapchainImageFormat = surfaceFormats.data[i];
            break;
        }
    }

    surface->swapchainExtent = surfaceCaps.currentExtent;
    VkSwapchainCreateInfoKHR swapchainCI = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = surface->surface,
        .minImageCount = imageCount,
        .imageFormat = surface->swapchainImageFormat.format,
        .imageColorSpace = surface->swapchainImageFormat.colorSpace,
        .imageExtent = surface->swapchainExtent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_TRANSFER_DST_BIT|VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .preTransform = surfaceCaps.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = selectedPresentMode,
        .clipped = VK_TRUE,
        .oldSwapchain = surface->swapchain,
    };

    u32 queueFamilyIndices[] = {surface->renderer->gfxQueueFamilyIndex, surface->renderer->presQueueFamilyIndex};
    if (surface->renderer->gfxQueueFamilyIndex != surface->renderer->presQueueFamilyIndex)
    {
        swapchainCI.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchainCI.queueFamilyIndexCount = 2;
        swapchainCI.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        swapchainCI.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapchainCI.queueFamilyIndexCount = 0;
        swapchainCI.pQueueFamilyIndices = nil;
    }

    MZNT_INTERNAL_VK_CHECKED_CALL(vkCreateSwapchainKHR(surface->renderer->device, &swapchainCI, nil, &(surface->swapchain)));

    if (swapchainCI.oldSwapchain != VK_NULL_HANDLE)
    {
        vkDestroySwapchainKHR(surface->renderer->device, swapchainCI.oldSwapchain, nil);
    }
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

MZNT_VulkanRendererSurface* MZNT_CreateRendererSurfaceFromWindow_Vulkan(MZNT_VulkanRenderer* renderer, MZNT_WindowHandle windowHandle, PNSLR_Allocator tempAllocator)
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
    #elif PNSLR_ANDROID
        VkAndroidSurfaceCreateInfoKHR createInfo =
        {
            .sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,
            .window = (ANativeWindow*) (windowHandle.handle),
        };

        MZNT_INTERNAL_VK_CHECKED_CALL(vkCreateAndroidSurfaceKHR(renderer->instance, &createInfo, nil, &output->surface));
    #else
        #error "unimplemented"
    #endif

    MZNT_Internal_CreateVkSwapchain(output, tempAllocator);

    MZNT_Internal_CreateVkSwapchainImagesAndViews(output, tempAllocator);

    output->semIdx = 0;
    output->curFrame = 0;

    VkCommandPoolCreateInfo cmdPoolInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = renderer->gfxQueueFamilyIndex,
    };

    MZNT_INTERNAL_VK_CHECKED_CALL(vkCreateCommandPool(renderer->device, &cmdPoolInfo, nil, &(output->cmdPool)));

    VkCommandBufferAllocateInfo cmdBufAI = {
        .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool        = output->cmdPool,
        .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = MZNT_NUM_FRAMES_IN_FLIGHT,
    };

    VkCommandBuffer cmdBuffers[MZNT_NUM_FRAMES_IN_FLIGHT];
    MZNT_INTERNAL_VK_CHECKED_CALL(vkAllocateCommandBuffers(renderer->device, &cmdBufAI, cmdBuffers));
    for (i32 i = 0; i < MZNT_NUM_FRAMES_IN_FLIGHT; i++)
    {
        output->commandBuffers[i].parent.type = MZNT_RendererType_Vulkan;
        output->commandBuffers[i].renderer = renderer;
        output->commandBuffers[i].cmdBuffer = cmdBuffers[i];
    }

    i64 imgCount = output->swapchainImages.count;
    output->presentCompleteSemaphores = PNSLR_MakeSlice(VkSemaphore, imgCount, false, renderer->parent.allocator, PNSLR_GET_LOC(), nil);
    output->renderFinishedSemaphores = PNSLR_MakeSlice(VkSemaphore, imgCount, false, renderer->parent.allocator, PNSLR_GET_LOC(), nil);
    output->inFlightFences = PNSLR_MakeSlice(VkFence, imgCount, false, renderer->parent.allocator, PNSLR_GET_LOC(), nil);

    for (i64 i = 0; i < imgCount; i++)
    {
        VkSemaphoreCreateInfo semaphoreCI = {.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
        MZNT_INTERNAL_VK_CHECKED_CALL(vkCreateSemaphore(renderer->device, &semaphoreCI, nil, &(output->presentCompleteSemaphores.data[i])));
        MZNT_INTERNAL_VK_CHECKED_CALL(vkCreateSemaphore(renderer->device, &semaphoreCI, nil, &(output->renderFinishedSemaphores.data[i])));

        VkFenceCreateInfo fenceCI = {.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO, .flags = VK_FENCE_CREATE_SIGNALED_BIT};
        MZNT_INTERNAL_VK_CHECKED_CALL(vkCreateFence(renderer->device, &fenceCI, nil, &(output->inFlightFences.data[i])));
    }

    {
        VkPipelineShaderStageCreateInfo triangleShaderStages[] = {
            {
                .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                .stage  = VK_SHADER_STAGE_VERTEX_BIT,
                .module = renderer->triangleShader.module,
                .pName  = "vertMain",
            },
            {
                .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                .stage  = VK_SHADER_STAGE_FRAGMENT_BIT,
                .module = renderer->triangleShader.module,
                .pName  = "fragMain",
            },
        };

        VkPipelineVertexInputStateCreateInfo vertexInputInfo = {.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};
        VkPipelineInputAssemblyStateCreateInfo inputAssembly = {.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO, .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST};
        VkPipelineViewportStateCreateInfo viewportState = {.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO, .viewportCount = 1, .scissorCount = 1};

        VkPipelineRasterizationStateCreateInfo rasterizer = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .depthClampEnable = VK_FALSE,
            .rasterizerDiscardEnable = VK_FALSE,
            .polygonMode = VK_POLYGON_MODE_FILL,
            .cullMode = VK_CULL_MODE_BACK_BIT,
            .frontFace = VK_FRONT_FACE_CLOCKWISE,
            .depthBiasEnable = VK_FALSE,
            .depthBiasSlopeFactor = 1.0f,
            .lineWidth = 1.0f,
        };

        VkPipelineMultisampleStateCreateInfo multisampling = {.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO, .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT};
        VkPipelineColorBlendAttachmentState colorBlendAttachment = {.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT};
        VkPipelineColorBlendStateCreateInfo colorBlending = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .logicOpEnable = VK_FALSE,
            .logicOp = VK_LOGIC_OP_COPY,
            .attachmentCount = 1,
            .pAttachments = &colorBlendAttachment,
        };

        VkDynamicState dynamicStates[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
        VkPipelineDynamicStateCreateInfo dynamicState = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
            .dynamicStateCount = sizeof(dynamicStates) / sizeof(VkDynamicState),
            .pDynamicStates = dynamicStates,
        };

        VkPipelineRenderingCreateInfo renderingCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
            .colorAttachmentCount = 1,
            .pColorAttachmentFormats = &output->swapchainImageFormat.format,
        };

        VkGraphicsPipelineCreateInfo pipelineInfo = {
            .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .pNext = &renderingCreateInfo,
            .stageCount = sizeof(triangleShaderStages) / sizeof(VkPipelineShaderStageCreateInfo),
            .pStages = triangleShaderStages,
            .pVertexInputState = &vertexInputInfo,
            .pInputAssemblyState = &inputAssembly,
            .pViewportState = &viewportState,
            .pRasterizationState = &rasterizer,
            .pMultisampleState = &multisampling,
            .pColorBlendState = &colorBlending,
            .pDynamicState = &dynamicState,
            .layout = renderer->triangleShader.layout,
            .renderPass = VK_NULL_HANDLE,
        };

        MZNT_INTERNAL_VK_CHECKED_CALL(vkCreateGraphicsPipelines(renderer->device, VK_NULL_HANDLE, 1, &pipelineInfo, nil, &output->trianglePipeline));
    }

    return output;
}

b8 MZNT_DestroyRendererSurface_Vulkan(MZNT_VulkanRendererSurface* surface, PNSLR_Allocator tempAllocator)
{
    if (!surface) return false;
    if (!surface->renderer) FORCE_DBG_TRAP;

    MZNT_INTERNAL_VK_CHECKED_CALL(vkDeviceWaitIdle(surface->renderer->device));

    vkDestroyPipeline(surface->renderer->device, surface->trianglePipeline, nil);

    i64 imgCount = surface->swapchainImages.count;
    for (i32 i = 0; i < imgCount; i++)
    {
        vkDestroyFence(surface->renderer->device, surface->inFlightFences.data[i], nil);
        vkDestroySemaphore(surface->renderer->device, surface->renderFinishedSemaphores.data[i], nil);
        vkDestroySemaphore(surface->renderer->device, surface->presentCompleteSemaphores.data[i], nil);
    }

    PNSLR_FreeSlice(&(surface->renderFinishedSemaphores), surface->renderer->parent.allocator, PNSLR_GET_LOC(), nil);
    PNSLR_FreeSlice(&(surface->presentCompleteSemaphores), surface->renderer->parent.allocator, PNSLR_GET_LOC(), nil);
    PNSLR_FreeSlice(&(surface->inFlightFences), surface->renderer->parent.allocator, PNSLR_GET_LOC(), nil);

    VkCommandBuffer cmdBufs[MZNT_NUM_FRAMES_IN_FLIGHT];
    for (i32 i = 0; i < MZNT_NUM_FRAMES_IN_FLIGHT; i++)
        cmdBufs[i] = surface->commandBuffers[i].cmdBuffer;

    vkFreeCommandBuffers(surface->renderer->device, surface->cmdPool, MZNT_NUM_FRAMES_IN_FLIGHT, cmdBufs);
    vkDestroyCommandPool(surface->renderer->device, surface->cmdPool, nil);

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

    MZNT_INTERNAL_VK_CHECKED_CALL(vkQueueWaitIdle(surface->renderer->gfxQueue));

    for (i64 i = 0; i < surface->swapchainImageViews.count; i++)
        vkDestroyImageView(surface->renderer->device, surface->swapchainImageViews.data[i], nil);

    PNSLR_FreeSlice(&(surface->swapchainImageViews), surface->renderer->parent.allocator, PNSLR_GET_LOC(), nil);
    PNSLR_FreeSlice(&(surface->swapchainImages), surface->renderer->parent.allocator, PNSLR_GET_LOC(), nil);

    MZNT_Internal_CreateVkSwapchain(surface, tempAllocator);

    MZNT_Internal_CreateVkSwapchainImagesAndViews(surface, tempAllocator);
    return true;
}

void MZNT_Internal_TransitionVkImage(
    VkCommandBuffer cmd,
    VkImage image,
    VkImageLayout srcLayout,
    VkImageLayout dstLayout,
    VkAccessFlags2 srcAccess,
    VkAccessFlags2 dstAccess,
    VkPipelineStageFlags2 srcStage,
    VkPipelineStageFlags2 dstStage
)
{
    VkImageMemoryBarrier2 imageBarrier = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
        .pNext = nil,
        .srcStageMask = srcStage,
        .srcAccessMask = srcAccess,
        .dstStageMask = dstStage,
        .dstAccessMask = dstAccess,
        .oldLayout = srcLayout,
        .newLayout = dstLayout,
        .subresourceRange = {
            .aspectMask = (dstLayout == VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = VK_REMAINING_MIP_LEVELS,
            .baseArrayLayer = 0,
            .layerCount = VK_REMAINING_ARRAY_LAYERS,
        },
        .image = image,
    };

    VkDependencyInfo depInfo = {
        .sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .pNext = nil,
        .imageMemoryBarrierCount = 1,
        .pImageMemoryBarriers = &imageBarrier,
    };

    vkCmdPipelineBarrier2(cmd, &depInfo);
}

MZNT_VulkanRendererCommandBuffer* MZNT_BeginFrame_Vulkan(MZNT_VulkanRendererSurface* surface, f32 r, f32 g, f32 b, f32 a, PNSLR_Allocator tempAllocator)
{
    MZNT_INTERNAL_VK_CHECKED_CALL(vkWaitForFences(surface->renderer->device, 1, &(surface->inFlightFences.data[surface->curFrame]), VK_TRUE, U64_MAX));
    MZNT_INTERNAL_VK_CHECKED_CALL(vkAcquireNextImageKHR(surface->renderer->device, surface->swapchain, U64_MAX, surface->presentCompleteSemaphores.data[surface->semIdx], VK_NULL_HANDLE, &(surface->curSwpchImgIdx)));
    MZNT_INTERNAL_VK_CHECKED_CALL(vkResetFences(surface->renderer->device, 1, &(surface->inFlightFences.data[surface->curFrame])));

    MZNT_VulkanRendererCommandBuffer* cmdBuf = &(surface->commandBuffers[surface->curFrame]);
    MZNT_INTERNAL_VK_CHECKED_CALL(vkResetCommandBuffer(cmdBuf->cmdBuffer, 0));

    VkCommandBufferBeginInfo cmdBufBI = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
    };

    MZNT_INTERNAL_VK_CHECKED_CALL(vkBeginCommandBuffer(cmdBuf->cmdBuffer, &cmdBufBI));

    MZNT_Internal_TransitionVkImage(
        cmdBuf->cmdBuffer,
        surface->swapchainImages.data[surface->curSwpchImgIdx],
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_GENERAL,
        VK_ACCESS_2_MEMORY_WRITE_BIT,
        VK_ACCESS_2_MEMORY_WRITE_BIT | VK_ACCESS_2_MEMORY_READ_BIT,
        VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
        VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT
    );

    // clear frame buffer
    // VkClearColorValue clearColour = {.float32 = {r, g, b, a}};
    // VkImageSubresourceRange clearRange = {
    //     .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
    //     .baseMipLevel = 0,
    //     .levelCount = VK_REMAINING_MIP_LEVELS,
    //     .baseArrayLayer = 0,
    //     .layerCount = VK_REMAINING_ARRAY_LAYERS,
    // };
    // vkCmdClearColorImage(cmdBuf->cmdBuffer, surface->swapchainImages.data[surface->curSwpchImgIdx], VK_IMAGE_LAYOUT_GENERAL, &clearColour, 1, &clearRange);

    VkRenderingInfo renderingInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
        .renderArea = {
            .offset = {0, 0},
            .extent = surface->swapchainExtent,
        },
        .layerCount = 1,
        .colorAttachmentCount = 1,
        .pColorAttachments = (VkRenderingAttachmentInfo[])
        {
            {
                .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
                .imageView = surface->swapchainImageViews.data[surface->curSwpchImgIdx],
                .imageLayout = VK_IMAGE_LAYOUT_GENERAL,
                .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                .clearValue = {.color = {.float32 = {r, g, b, a}}},
            },
        },
    };

    vkCmdBeginRendering(cmdBuf->cmdBuffer, &renderingInfo);

    vkCmdBindPipeline(cmdBuf->cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, surface->trianglePipeline);

    vkCmdSetViewport(cmdBuf->cmdBuffer, 0, 1, &(VkViewport)
    {
        .x = 0.0f,
        .y = 0.0f,
        .width = (f32) surface->swapchainExtent.width,
        .height = (f32) surface->swapchainExtent.height,
        .minDepth = 0.0f,
        .maxDepth = 1.0f,
    });

    vkCmdSetScissor(cmdBuf->cmdBuffer, 0, 1, &(VkRect2D){.offset = {0, 0}, .extent = surface->swapchainExtent});

    vkCmdDraw(cmdBuf->cmdBuffer, 3, 1, 0, 0);

    return cmdBuf;
}

b8 MZNT_EndFrame_Vulkan(MZNT_VulkanRendererSurface* surface, PNSLR_Allocator tempAllocator)
{
    MZNT_VulkanRendererCommandBuffer* cmdBuf = &(surface->commandBuffers[surface->curFrame]);

    vkCmdEndRendering(cmdBuf->cmdBuffer);

    MZNT_Internal_TransitionVkImage(
        cmdBuf->cmdBuffer,
        surface->swapchainImages.data[surface->curSwpchImgIdx],
        VK_IMAGE_LAYOUT_GENERAL,
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
        VK_ACCESS_2_MEMORY_WRITE_BIT,
        VK_ACCESS_2_MEMORY_WRITE_BIT | VK_ACCESS_2_MEMORY_READ_BIT,
        VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT,
        VK_PIPELINE_STAGE_2_ALL_COMMANDS_BIT
    );

    MZNT_INTERNAL_VK_CHECKED_CALL(vkEndCommandBuffer(cmdBuf->cmdBuffer));

    VkCommandBufferSubmitInfo cmdSubmitInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
        .commandBuffer = cmdBuf->cmdBuffer,
    };

    VkSemaphoreSubmitInfo waitInfo = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
        .semaphore = surface->presentCompleteSemaphores.data[surface->semIdx],
        .value = 1,
        .stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
    };

    VkSemaphoreSubmitInfo signalInfo = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
        .semaphore = surface->renderFinishedSemaphores.data[surface->curSwpchImgIdx],
        .value = 1,
        .stageMask = VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT,
    };

    VkSubmitInfo2 submitInfo = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
        .pNext = nil,
        .flags = 0,
        .waitSemaphoreInfoCount = 1,
        .pWaitSemaphoreInfos = &waitInfo,
        .commandBufferInfoCount = 1,
        .pCommandBufferInfos = &cmdSubmitInfo,
        .signalSemaphoreInfoCount = 1,
        .pSignalSemaphoreInfos = &signalInfo,
    };

    MZNT_INTERNAL_VK_CHECKED_CALL(vkQueueSubmit2(surface->renderer->gfxQueue, 1, &submitInfo, surface->inFlightFences.data[surface->curFrame]));

    VkPresentInfoKHR presentInfo = {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .pNext = nil,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &(surface->renderFinishedSemaphores.data[surface->curSwpchImgIdx]),
        .swapchainCount = 1,
        .pSwapchains = &(surface->swapchain),
        .pImageIndices = &(surface->curSwpchImgIdx),
        .pResults = nil,
    };

    MZNT_INTERNAL_VK_CHECKED_CALL(vkQueuePresentKHR(surface->renderer->gfxQueue, &presentInfo));

    surface->semIdx = (surface->semIdx + 1) % (u32) surface->presentCompleteSemaphores.count;
    surface->curFrame = (surface->curFrame + 1) % MZNT_NUM_FRAMES_IN_FLIGHT;
    surface->curSwpchImgIdx = U32_MAX; // invalidate
    return true;
}

#endif
