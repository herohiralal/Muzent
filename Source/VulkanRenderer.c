#define MZNT_IMPLEMENTATION
#include "VulkanRenderer.h"
#if MZNT_VULKAN

#define INLINED_FILE_INCLUSION_NAME k_MZNT_Internal_VkHelloTriangleVS
#include "Shaders/HelloTriangle/HelloTriangle.vert.spv.c"
#undef INLINED_FILE_INCLUSION_NAME

#define INLINED_FILE_INCLUSION_NAME k_MZNT_Internal_VkHelloTriangleFS
#include "Shaders/HelloTriangle/HelloTriangle.frag.spv.c"
#undef INLINED_FILE_INCLUSION_NAME

#define INLINED_FILE_INCLUSION_NAME k_MZNT_Internal_VkFullScreenBlitVS
#include "Shaders/FullScreenBlit/FullScreenBlit.vert.spv.c"
#undef INLINED_FILE_INCLUSION_NAME

#define INLINED_FILE_INCLUSION_NAME k_MZNT_Internal_VkFullScreenBlitFS
#include "Shaders/FullScreenBlit/FullScreenBlit.frag.spv.c"

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

static const VkFormat k_MZNT_Internal_PreferredVkColourAttchFormat = VK_FORMAT_R16G16B16A16_SFLOAT;
static const VkFormat k_MZNT_Internal_PreferredVkDepthAttchFormat  = VK_FORMAT_D32_SFLOAT_S8_UINT;

MZNT_VulkanRenderer* MZNT_CreateRenderer_Vulkan(MZNT_RendererConfiguration config, PNSLR_Allocator tempAllocator)
{
    MZNT_INTERNAL_VK_CHECKED_CALL(volkInitialize());

    MZNT_VulkanRenderer* output = PNSLR_New(MZNT_VulkanRenderer, config.allocator, PNSLR_GET_LOC(), nil);
    if (!output) FORCE_DBG_TRAP;

    output->parent.type      = MZNT_RendererType_Vulkan;
    output->parent.allocator = config.allocator;
    output->parent.appHandle = config.appHandle;

    static const i32 k_MaxSupportedInstanceLayers = 16;
    cstring* enabledLayers = (cstring*) PNSLR_Allocate(tempAllocator, false, k_MaxSupportedInstanceLayers * sizeof(cstring), alignof(cstring), PNSLR_GET_LOC(), nil);
    u32 enabledLayersCount = 0;

    u32 availableLayerCount = 0;
    vkEnumerateInstanceLayerProperties(&availableLayerCount, nil);
    PNSLR_ArraySlice(VkLayerProperties) availableLayers = PNSLR_MakeSlice(VkLayerProperties, availableLayerCount, false, tempAllocator, PNSLR_GET_LOC(), nil);
    vkEnumerateInstanceLayerProperties(&availableLayerCount, availableLayers.data);
    availableLayers.count = (i64) availableLayerCount;

    b8 validationLayersEnabled = false;
    for (i64 i = 0; i < availableLayers.count; i++)
    {
        VkLayerProperties* layer = &availableLayers.data[i];

        if (PNSLR_DBG && PNSLR_AreCStringsEqual(layer->layerName, "VK_LAYER_KHRONOS_validation", 0))
        {
            validationLayersEnabled = true;
            enabledLayers[enabledLayersCount++] = &(layer->layerName[0]);
            PNSLR_LogI(PNSLR_StringLiteral("Found validation layers in vulkan. Enabling."), PNSLR_GET_LOC());
            continue;
        }

        PNSLR_LogIf(
            PNSLR_StringLiteral("Skipped, but available layer: $ ($)."),
            PNSLR_FmtArgs(
                PNSLR_FmtCString((cstring) &(layer->layerName[0])),
                PNSLR_FmtCString((cstring) &(layer->description[0]))
            ),
            PNSLR_GET_LOC()
        );
    }

    static const i32 k_MaxSupportedInstanceExtensions = 16;
    cstring* enabledExtensions = (cstring*) PNSLR_Allocate(tempAllocator, false, k_MaxSupportedInstanceExtensions * sizeof(cstring), alignof(cstring), PNSLR_GET_LOC(), nil);
    u32 enabledExtensionsCount = 0;

    u32 availableExtensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nil, &availableExtensionCount, nil);
    PNSLR_ArraySlice(VkExtensionProperties) availableExtensions = PNSLR_MakeSlice(VkExtensionProperties, availableExtensionCount, false, tempAllocator, PNSLR_GET_LOC(), nil);
    vkEnumerateInstanceExtensionProperties(nil, &availableExtensionCount, availableExtensions.data);
    availableExtensions.count = (i64) availableExtensionCount;

    for (i64 i = 0; i < availableExtensions.count; i++)
    {
        VkExtensionProperties* ext = &availableExtensions.data[i];

        if (PNSLR_AreCStringsEqual(ext->extensionName, VK_KHR_SURFACE_EXTENSION_NAME, 0))
        {
            enabledExtensions[enabledExtensionsCount++] = &(ext->extensionName[0]);
            continue;
        }

        #if PNSLR_WINDOWS
            if (PNSLR_AreCStringsEqual(ext->extensionName, VK_KHR_WIN32_SURFACE_EXTENSION_NAME, 0))
            {
                enabledExtensions[enabledExtensionsCount++] = &(ext->extensionName[0]);
                continue;
            }
        #endif

        #if PNSLR_LINUX
            if (PNSLR_AreCStringsEqual(ext->extensionName, VK_KHR_XCB_SURFACE_EXTENSION_NAME, 0))
            {
                enabledExtensions[enabledExtensionsCount++] = &(ext->extensionName[0]);
                continue;
            }
        #endif

        #if PNSLR_ANDROID
            if (PNSLR_AreCStringsEqual(ext->extensionName, VK_KHR_ANDROID_SURFACE_EXTENSION_NAME, 0))
            {
                enabledExtensions[enabledExtensionsCount++] = &(ext->extensionName[0]);
                continue;
            }
        #endif

        #if PNSLR_APPLE
            if (PNSLR_AreCStringsEqual(ext->extensionName, VK_EXT_METAL_SURFACE_EXTENSION_NAME, 0))
            {
                enabledExtensions[enabledExtensionsCount++] = &(ext->extensionName[0]);
                continue;
            }
        #endif

        if (validationLayersEnabled && PNSLR_AreCStringsEqual(ext->extensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME, 0))
        {
            enabledExtensions[enabledExtensionsCount++] = &(ext->extensionName[0]);
            continue;
        }
    }

    MZNT_INTERNAL_VK_CHECKED_CALL(vkCreateInstance(&(VkInstanceCreateInfo)
    {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = nil,
        .flags = 0,
        .pApplicationInfo = &(VkApplicationInfo)
        {
            .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext              = nil,
            .pApplicationName   = PNSLR_CStringFromString(config.appName, tempAllocator),
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .pEngineName        = "M_U_Z_E_N_T",
            .engineVersion      = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion         = VK_API_VERSION_1_3, // fuck it, we dyna-rendering fr this time
        },
        .enabledLayerCount = enabledLayersCount,
        .ppEnabledLayerNames = (const char* const*) enabledLayers,
        .enabledExtensionCount = enabledExtensionsCount,
        .ppEnabledExtensionNames = (const char* const*) enabledExtensions,
    }, nil, &output->instance));

    volkLoadInstanceOnly(output->instance);

    if (validationLayersEnabled)
    {
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
    }

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

        PNSLR_LogIf(
            PNSLR_StringLiteral(
                "Device: $. ty: $.\n"
                "\tsampler anisotropy:                              $.\n" // 1.0
                "\tshader draw parameters:                          $.\n" // 1.1
                "\tdescriptor indexing:                             $.\n" // 1.2
                "\tshader sampled image array non uniform indexing: $.\n"
                "\tdescriptor binding variable descriptor count:    $.\n"
                "\truntime descriptor array:                        $.\n"
                "\tbuffer device address:                           $.\n"
                "\tsynchronization 2:                               $.\n" // 1.3
                "\tdynamic rendering:                               $.\n"
            ),
            PNSLR_FmtArgs(
                PNSLR_FmtCString(deviceProperties.properties.deviceName),
                PNSLR_FmtI32((i32) deviceProperties.properties.deviceType, 0),

                PNSLR_FmtB8(!!deviceFeatures.features.samplerAnisotropy),
                PNSLR_FmtB8(!!deviceFeatures11.shaderDrawParameters),
                PNSLR_FmtB8(!!deviceFeatures12.descriptorIndexing),
                PNSLR_FmtB8(!!deviceFeatures12.shaderSampledImageArrayNonUniformIndexing),
                PNSLR_FmtB8(!!deviceFeatures12.descriptorBindingVariableDescriptorCount),
                PNSLR_FmtB8(!!deviceFeatures12.runtimeDescriptorArray),
                PNSLR_FmtB8(!!deviceFeatures12.bufferDeviceAddress),
                PNSLR_FmtB8(!!deviceFeatures13.synchronization2),
                PNSLR_FmtB8(!!deviceFeatures13.dynamicRendering)
            ),
            PNSLR_GET_LOC()
        );

        // prefer discrete gpu on desktop
        // want shader draw params
        if ((!PNSLR_DESKTOP || deviceProperties.properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) &&
            !!deviceFeatures.features.samplerAnisotropy &&
            !!deviceFeatures11.shaderDrawParameters &&
            !!deviceFeatures12.descriptorIndexing &&
            !!deviceFeatures12.shaderSampledImageArrayNonUniformIndexing &&
            !!deviceFeatures12.descriptorBindingVariableDescriptorCount &&
            !!deviceFeatures12.runtimeDescriptorArray &&
            !!deviceFeatures12.bufferDeviceAddress &&
            !!deviceFeatures13.synchronization2 &&
            !!deviceFeatures13.dynamicRendering)
        {
            selectedDevice = devices.data[i];
            break;
        }
    }

    if (selectedDevice == VK_NULL_HANDLE)
    {
        PNSLR_LogE(PNSLR_StringLiteral("Failed to find suitable physical device!"), PNSLR_GET_LOC());
        FORCE_DBG_TRAP;
    }

    output->physicalDevice = selectedDevice;

    {
        VkFormatProperties formatProps = {0};
        vkGetPhysicalDeviceFormatProperties(selectedDevice, k_MZNT_Internal_PreferredVkColourAttchFormat, &formatProps);
        if (!(formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT))
        {
            PNSLR_LogE(PNSLR_StringLiteral("Preferred colour format not supported as color attachment!"), PNSLR_GET_LOC());
            FORCE_DBG_TRAP;
        }

        formatProps = (VkFormatProperties) {0};
        vkGetPhysicalDeviceFormatProperties(selectedDevice, k_MZNT_Internal_PreferredVkDepthAttchFormat, &formatProps);
        if (!(formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT))
        {
            PNSLR_LogE(PNSLR_StringLiteral("Preferred depth format not supported as depth attachment!"), PNSLR_GET_LOC());
            FORCE_DBG_TRAP;
        }
    }

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
    };

    u32 enabledDeviceExtensionCount = sizeof(enabledDeviceExtensions) / sizeof(char*);

    MZNT_INTERNAL_VK_CHECKED_CALL(vkCreateDevice(selectedDevice, &(VkDeviceCreateInfo)
    {
        .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount    = (u32) qcis.count,
        .pQueueCreateInfos       = qcis.data,
        .enabledExtensionCount   = enabledDeviceExtensionCount,
        .ppEnabledExtensionNames = enabledDeviceExtensions,
        .pEnabledFeatures        = &(VkPhysicalDeviceFeatures)
        {
            .samplerAnisotropy   = VK_TRUE,
        },
        .pNext                   = &(VkPhysicalDeviceVulkan13Features)
        {
            .sType               = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
            .pNext               = &(VkPhysicalDeviceVulkan12Features)
            {
                .sType           = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
                .pNext           = &(VkPhysicalDeviceVulkan11Features)
                {
                    .sType       = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES,
                    .pNext       = nil,

                    .shaderDrawParameters                  = VK_TRUE,
                },
                .descriptorIndexing                        = VK_TRUE,
                .shaderSampledImageArrayNonUniformIndexing = VK_TRUE,
                .descriptorBindingVariableDescriptorCount  = VK_TRUE,
                .runtimeDescriptorArray                    = VK_TRUE,
                .bufferDeviceAddress                       = VK_TRUE,
            },
            .synchronization2                              = VK_TRUE,
            .dynamicRendering                              = VK_TRUE,
        },
    }, nil, &output->device));

    volkLoadDevice(output->device);

    vkGetDeviceQueue2(output->device, &(VkDeviceQueueInfo2)
    {
        .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2,
        .queueFamilyIndex = output->gfxQueueFamilyIndex,
        .queueIndex       = 0,
    },  &output->gfxQueue);

    vkGetDeviceQueue2(output->device, &(VkDeviceQueueInfo2)
    {
        .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2,
        .queueFamilyIndex = output->presQueueFamilyIndex,
        .queueIndex       = 0,
    }, &output->presQueue);

    MZNT_INTERNAL_VK_CHECKED_CALL(vmaCreateAllocator(&(VmaAllocatorCreateInfo)
    {
        .physicalDevice            = output->physicalDevice,
        .device                    = output->device,
        .instance                  = output->instance,
        .flags                     = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT,
        .pVulkanFunctions          = &(VmaVulkanFunctions)
        {
            .vkGetInstanceProcAddr = vkGetInstanceProcAddr,
            .vkGetDeviceProcAddr   = vkGetDeviceProcAddr,
        },
    }, &(output->vmaAllocator)));

    // triangle shader pipeline
    {
        MZNT_INTERNAL_VK_CHECKED_CALL(vkCreateShaderModule(output->device, &(VkShaderModuleCreateInfo)
        {
            .sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .codeSize = (size_t) k_MZNT_Internal_VkHelloTriangleVSSize,
            .pCode    = (u32*)   k_MZNT_Internal_VkHelloTriangleVSContents,
        }, nil, &output->helloTriangleShader.vsModule));

        MZNT_INTERNAL_VK_CHECKED_CALL(vkCreateShaderModule(output->device, &(VkShaderModuleCreateInfo)
        {
            .sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .codeSize = (size_t) k_MZNT_Internal_VkHelloTriangleFSSize,
            .pCode    = (u32*)   k_MZNT_Internal_VkHelloTriangleFSContents,
        }, nil, &output->helloTriangleShader.fsModule));

        MZNT_INTERNAL_VK_CHECKED_CALL(vkCreatePipelineLayout(output->device, &(VkPipelineLayoutCreateInfo)
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        }, nil, &output->helloTriangleShader.pipelineLayout));

        VkPipelineShaderStageCreateInfo helloTriangleShaderStages[] = {
            {
                .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                .stage  = VK_SHADER_STAGE_VERTEX_BIT,
                .module = output->helloTriangleShader.vsModule,
                .pName  = "main",
            },
            {
                .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                .stage  = VK_SHADER_STAGE_FRAGMENT_BIT,
                .module = output->helloTriangleShader.fsModule,
                .pName  = "main",
            },
        };

        VkDynamicState dynamicStates[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};

        MZNT_INTERNAL_VK_CHECKED_CALL(vkCreateGraphicsPipelines(output->device, VK_NULL_HANDLE, 1, &(VkGraphicsPipelineCreateInfo)
        {
            .sType                       = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .stageCount                  = sizeof(helloTriangleShaderStages) / sizeof(VkPipelineShaderStageCreateInfo),
            .pStages                     = helloTriangleShaderStages,
            .pVertexInputState           = &(VkPipelineVertexInputStateCreateInfo)
            {
                .sType                   = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            },
            .pInputAssemblyState         = &(VkPipelineInputAssemblyStateCreateInfo)
            {
                .sType                   = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
                .topology                = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            },
            .pViewportState              = &(VkPipelineViewportStateCreateInfo)
            {
                .sType                   = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
                .viewportCount           = 1,
                .scissorCount            = 1,
            },
            .pRasterizationState         = &(VkPipelineRasterizationStateCreateInfo)
            {
                .sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
                .depthClampEnable        = VK_FALSE,
                .rasterizerDiscardEnable = VK_FALSE,
                .polygonMode             = VK_POLYGON_MODE_FILL,
                .cullMode                = VK_CULL_MODE_BACK_BIT,
                .frontFace               = VK_FRONT_FACE_CLOCKWISE,
                .depthBiasEnable         = VK_FALSE,
                .depthBiasSlopeFactor    = 1.0f,
                .lineWidth               = 1.0f,
            },
            .pMultisampleState           = &(VkPipelineMultisampleStateCreateInfo)
            {
                .sType                   = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
                .rasterizationSamples    = VK_SAMPLE_COUNT_1_BIT,
            },
            .pColorBlendState            = &(VkPipelineColorBlendStateCreateInfo)
            {
                .sType                   = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
                .logicOpEnable           = VK_FALSE,
                .logicOp                 = VK_LOGIC_OP_COPY,
                .attachmentCount         = 1,
                .pAttachments            = &(VkPipelineColorBlendAttachmentState)
                {
                    .colorWriteMask      = VK_COLOR_COMPONENT_R_BIT |
                                           VK_COLOR_COMPONENT_G_BIT |
                                           VK_COLOR_COMPONENT_B_BIT |
                                           VK_COLOR_COMPONENT_A_BIT,
                },
            },
            .pDynamicState               = &(VkPipelineDynamicStateCreateInfo)
            {
                .sType                   = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
                .dynamicStateCount       = sizeof(dynamicStates) / sizeof(VkDynamicState),
                .pDynamicStates          = dynamicStates,
            },
            .layout                      = output->helloTriangleShader.pipelineLayout,
            .renderPass                  = VK_NULL_HANDLE,
            .pNext                       = &(VkPipelineRenderingCreateInfo)
            {
                .sType                   = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
                .colorAttachmentCount    = 1,
                .pColorAttachmentFormats = &k_MZNT_Internal_PreferredVkColourAttchFormat,
                .depthAttachmentFormat   = VK_FORMAT_UNDEFINED,
                .stencilAttachmentFormat = VK_FORMAT_UNDEFINED,
            },
        }, nil, &output->helloTriangleShader.pipeline));
    }

    return output;
}

b8 MZNT_DestroyRenderer_Vulkan(MZNT_VulkanRenderer* renderer, PNSLR_Allocator tempAllocator)
{
    if (!renderer) return false;

    MZNT_INTERNAL_VK_CHECKED_CALL(vkDeviceWaitIdle(renderer->device));

    vkDestroyPipeline(renderer->device, renderer->helloTriangleShader.pipeline, nil);
    vkDestroyPipelineLayout(renderer->device, renderer->helloTriangleShader.pipelineLayout, nil);
    vkDestroyShaderModule(renderer->device, renderer->helloTriangleShader.fsModule, nil);
    vkDestroyShaderModule(renderer->device, renderer->helloTriangleShader.vsModule, nil);

    vmaDestroyAllocator(renderer->vmaAllocator);

    vkDestroyDevice(renderer->device, nil);

    if (renderer->debugMessenger != VK_NULL_HANDLE)
        vkDestroyDebugUtilsMessengerEXT(renderer->instance, renderer->debugMessenger, nil);

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

    surface->swapchainExtent = surfaceCaps.currentExtent;
    PNSLR_LogDf(PNSLR_StringLiteral("Swapchain extent: $x$"), PNSLR_FmtArgs(PNSLR_FmtU32(surface->swapchainExtent.width, 0), PNSLR_FmtU32(surface->swapchainExtent.height, 0)), PNSLR_GET_LOC());
    VkSwapchainCreateInfoKHR swapchainCI = {
        .sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface          = surface->surface,
        .minImageCount    = imageCount,
        .imageFormat      = surface->swapchainImageFormat.format,
        .imageColorSpace  = surface->swapchainImageFormat.colorSpace,
        .imageExtent      = surface->swapchainExtent,
        .imageArrayLayers = 1,
        .imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .preTransform     = surfaceCaps.currentTransform,
        .compositeAlpha   = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode      = selectedPresentMode,
        .clipped          = VK_TRUE,
        .oldSwapchain     = surface->swapchain,
    };

    u32 queueFamilyIndices[] = {surface->renderer->gfxQueueFamilyIndex, surface->renderer->presQueueFamilyIndex};
    if (surface->renderer->gfxQueueFamilyIndex != surface->renderer->presQueueFamilyIndex)
    {
        swapchainCI.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
        swapchainCI.queueFamilyIndexCount = 2;
        swapchainCI.pQueueFamilyIndices   = queueFamilyIndices;
    }
    else
    {
        swapchainCI.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
        swapchainCI.queueFamilyIndexCount = 0;
        swapchainCI.pQueueFamilyIndices   = nil;
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
            MZNT_INTERNAL_VK_CHECKED_CALL(vkCreateImageView(surface->renderer->device, &(VkImageViewCreateInfo)
            {
                .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .image = surface->swapchainImages.data[i],
                .viewType = VK_IMAGE_VIEW_TYPE_2D,
                .format = surface->swapchainImageFormat.format,
                .components = {
                    .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                    .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                    .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                    .a = VK_COMPONENT_SWIZZLE_IDENTITY,
                },
                .subresourceRange = {
                    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                    .baseMipLevel = 0,
                    .levelCount = VK_REMAINING_MIP_LEVELS,
                    .baseArrayLayer = 0,
                    .layerCount = VK_REMAINING_ARRAY_LAYERS,
                },
            }, nil, &(surface->swapchainImageViews.data[i])));
        }
    }

    // screen buffer
    {
        surface->screenImages = PNSLR_MakeSlice(VkImage, surface->swapchainImages.count, false, surface->renderer->parent.allocator, PNSLR_GET_LOC(), nil);
        surface->screenImageAllocations = PNSLR_MakeSlice(VmaAllocation, surface->swapchainImages.count, false, surface->renderer->parent.allocator, PNSLR_GET_LOC(), nil);
        surface->screenImageViews = PNSLR_MakeSlice(VkImageView, surface->swapchainImages.count, false, surface->renderer->parent.allocator, PNSLR_GET_LOC(), nil);

        for (i64 i = 0; i < surface->swapchainImages.count; i++)
        {
            MZNT_INTERNAL_VK_CHECKED_CALL(vmaCreateImage(surface->renderer->vmaAllocator, &(VkImageCreateInfo)
            {
                .sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
                .imageType     = VK_IMAGE_TYPE_2D,
                .format        = k_MZNT_Internal_PreferredVkColourAttchFormat,
                .extent        =
                {
                    .width     = surface->swapchainExtent.width,
                    .height    = surface->swapchainExtent.height,
                    .depth     = 1,
                },
                .mipLevels     = 1,
                .arrayLayers   = 1,
                .samples       = VK_SAMPLE_COUNT_1_BIT,
                .tiling        = VK_IMAGE_TILING_OPTIMAL,
                .usage         = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            },
            &(VmaAllocationCreateInfo)
            {
                .flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
                .usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,
            }, &(surface->screenImages.data[i]), &(surface->screenImageAllocations.data[i]), nil));

            MZNT_INTERNAL_VK_CHECKED_CALL(vkCreateImageView(surface->renderer->device, &(VkImageViewCreateInfo)
            {
                .sType      = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .image      = surface->screenImages.data[i],
                .viewType   = VK_IMAGE_VIEW_TYPE_2D,
                .format     = k_MZNT_Internal_PreferredVkColourAttchFormat,
                .components =
                {
                    .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                    .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                    .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                    .a = VK_COMPONENT_SWIZZLE_IDENTITY,
                },
                .subresourceRange =
                {
                    .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
                    .baseMipLevel   = 0,
                    .levelCount     = VK_REMAINING_MIP_LEVELS,
                    .baseArrayLayer = 0,
                    .layerCount     = VK_REMAINING_ARRAY_LAYERS,
                },
            }, nil, &surface->screenImageViews.data[i]));
        }
    }

    // depth image
    {
        surface->depthImages = PNSLR_MakeSlice(VkImage, surface->swapchainImages.count, false, surface->renderer->parent.allocator, PNSLR_GET_LOC(), nil);
        surface->depthImageAllocations = PNSLR_MakeSlice(VmaAllocation, surface->swapchainImages.count, false, surface->renderer->parent.allocator, PNSLR_GET_LOC(), nil);
        surface->depthImageViews = PNSLR_MakeSlice(VkImageView, surface->swapchainImages.count, false, surface->renderer->parent.allocator, PNSLR_GET_LOC(), nil);

        for (i64 i = 0; i < surface->swapchainImages.count; i++)
        {
            MZNT_INTERNAL_VK_CHECKED_CALL(vmaCreateImage(surface->renderer->vmaAllocator, &(VkImageCreateInfo)
            {
                .sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
                .imageType     = VK_IMAGE_TYPE_2D,
                .format        = k_MZNT_Internal_PreferredVkDepthAttchFormat,
                .extent        =
                {
                    .width     = surface->swapchainExtent.width,
                    .height    = surface->swapchainExtent.height,
                    .depth     = 1,
                },
                .mipLevels     = 1,
                .arrayLayers   = 1,
                .samples       = VK_SAMPLE_COUNT_1_BIT,
                .tiling        = VK_IMAGE_TILING_OPTIMAL,
                .usage         = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            },
            &(VmaAllocationCreateInfo)
            {
                .flags = VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
                .usage = VMA_MEMORY_USAGE_AUTO,
            }, &(surface->depthImages.data[i]), &(surface->depthImageAllocations.data[i]), nil));

            MZNT_INTERNAL_VK_CHECKED_CALL(vkCreateImageView(surface->renderer->device, &(VkImageViewCreateInfo)
            {
                .sType      = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .image      = surface->depthImages.data[i],
                .viewType   = VK_IMAGE_VIEW_TYPE_2D,
                .format     = k_MZNT_Internal_PreferredVkDepthAttchFormat,
                .components =
                {
                    .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                    .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                    .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                    .a = VK_COMPONENT_SWIZZLE_IDENTITY,
                },
                .subresourceRange =
                {
                    .aspectMask     = VK_IMAGE_ASPECT_DEPTH_BIT,
                    .baseMipLevel   = 0,
                    .levelCount     = VK_REMAINING_MIP_LEVELS,
                    .baseArrayLayer = 0,
                    .layerCount     = VK_REMAINING_ARRAY_LAYERS,
                },
            }, nil, &surface->depthImageViews.data[i]));
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

        MZNT_INTERNAL_VK_CHECKED_CALL(vkCreateWin32SurfaceKHR(renderer->instance, &(VkWin32SurfaceCreateInfoKHR)
        {
            .sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
            .hinstance = (HINSTANCE)(uintptr_t) (renderer->parent.appHandle.handle),
            .hwnd      = (HWND)(uintptr_t) (windowHandle.handle),
        }, nil, &output->surface));

    #elif PNSLR_ANDROID

        MZNT_INTERNAL_VK_CHECKED_CALL(vkCreateAndroidSurfaceKHR(renderer->instance, &(VkAndroidSurfaceCreateInfoKHR)
        {
            .sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,
            .window = (ANativeWindow*) (windowHandle.handle),
        }, nil, &output->surface));

    #else
        #error "unimplemented"
    #endif

    // select surface format type
    {
        u32 formatCount = 0;
        MZNT_INTERNAL_VK_CHECKED_CALL(vkGetPhysicalDeviceSurfaceFormatsKHR(output->renderer->physicalDevice, output->surface, &formatCount, nil));
        PNSLR_ArraySlice(VkSurfaceFormatKHR) surfaceFormats = PNSLR_MakeSlice(VkSurfaceFormatKHR, formatCount, false, tempAllocator, PNSLR_GET_LOC(), nil);
        MZNT_INTERNAL_VK_CHECKED_CALL(vkGetPhysicalDeviceSurfaceFormatsKHR(output->renderer->physicalDevice, output->surface, &formatCount, surfaceFormats.data));

        if (formatCount == 0)
        {
            PNSLR_LogE(PNSLR_StringLiteral("Failed to get any surface formats for swapchain"), PNSLR_GET_LOC());
            FORCE_DBG_TRAP;
        }

        output->swapchainImageFormat = surfaceFormats.data[0];
        for (i64 i = 0; i < surfaceFormats.count; i++)
        {
            #if PNSLR_DESKTOP
                if (surfaceFormats.data[i].format == VK_FORMAT_B8G8R8A8_UNORM &&
                    surfaceFormats.data[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            #elif PNSLR_ANDROID
                if (surfaceFormats.data[i].format == VK_FORMAT_R8G8B8A8_UNORM)
            #else
                #error "unimplemented"
            #endif
                {
                    output->swapchainImageFormat = surfaceFormats.data[i];
                    break;
                }
        }
    }

    MZNT_Internal_CreateVkSwapchain(output, tempAllocator);

    MZNT_Internal_CreateVkSwapchainImagesAndViews(output, tempAllocator);

    output->semIdx = 0;
    output->curFrame = 0;

    MZNT_INTERNAL_VK_CHECKED_CALL(vkCreateCommandPool(renderer->device, &(VkCommandPoolCreateInfo)
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = renderer->gfxQueueFamilyIndex,
    }, nil, &(output->cmdPool)));

    VkCommandBuffer cmdBuffers[MZNT_NUM_FRAMES_IN_FLIGHT];
    MZNT_INTERNAL_VK_CHECKED_CALL(vkAllocateCommandBuffers(renderer->device, &(VkCommandBufferAllocateInfo)
    {
        .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool        = output->cmdPool,
        .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = MZNT_NUM_FRAMES_IN_FLIGHT,
    }, cmdBuffers));

    for (i32 i = 0; i < MZNT_NUM_FRAMES_IN_FLIGHT; i++)
    {
        output->commandBuffers[i].parent.type = MZNT_RendererType_Vulkan;
        output->commandBuffers[i].renderer    = renderer;
        output->commandBuffers[i].cmdBuffer   = cmdBuffers[i];
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

    // final blit shader pipeline
    {
        // TODO: replace with immutable sampler thing
        MZNT_INTERNAL_VK_CHECKED_CALL(vkCreateSampler(renderer->device, &(VkSamplerCreateInfo)
        {
            .sType        = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
            .magFilter    = VK_FILTER_LINEAR,
            .minFilter    = VK_FILTER_LINEAR,
            .mipmapMode   = VK_SAMPLER_MIPMAP_MODE_LINEAR,
            .addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
            .addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
            .addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
            .maxAnisotropy = 1.0f,
            .maxLod       = VK_LOD_CLAMP_NONE,
        }, nil, &(output->finalBlitSampler)));

        MZNT_INTERNAL_VK_CHECKED_CALL(vkCreateShaderModule(renderer->device, &(VkShaderModuleCreateInfo)
        {
            .sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .codeSize = (size_t) k_MZNT_Internal_VkFullScreenBlitVSSize,
            .pCode    = (u32*)   k_MZNT_Internal_VkFullScreenBlitVSContents,
        }, nil, &output->finalBlitShader.vsModule));

        MZNT_INTERNAL_VK_CHECKED_CALL(vkCreateShaderModule(renderer->device, &(VkShaderModuleCreateInfo)
        {
            .sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .codeSize = (size_t) k_MZNT_Internal_VkFullScreenBlitFSSize,
            .pCode    = (u32*)   k_MZNT_Internal_VkFullScreenBlitFSContents,
        }, nil, &output->finalBlitShader.fsModule));

        MZNT_INTERNAL_VK_CHECKED_CALL(vkCreateDescriptorSetLayout(renderer->device, &(VkDescriptorSetLayoutCreateInfo)
        {
            .sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
            .bindingCount = 2,
            .pBindings    = (VkDescriptorSetLayoutBinding[])
            {
                {
                    .binding            = 0,
                    .descriptorType     = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
                    .descriptorCount    = 1,
                    .stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT,
                },
                {
                    .binding            = 1,
                    .descriptorType     = VK_DESCRIPTOR_TYPE_SAMPLER,
                    .descriptorCount    = 1,
                    .stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT,
                }
            }
        }, nil, &output->finalBlitShader.descriptorSetLayout));

        MZNT_INTERNAL_VK_CHECKED_CALL(vkCreatePipelineLayout(renderer->device, &(VkPipelineLayoutCreateInfo)
        {
            .sType          = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .setLayoutCount = 1,
            .pSetLayouts    = &output->finalBlitShader.descriptorSetLayout,
        }, nil, &output->finalBlitShader.pipelineLayout));

        VkPipelineShaderStageCreateInfo finalBlitShaderStages[] = {
            {
                .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                .stage  = VK_SHADER_STAGE_VERTEX_BIT,
                .module = output->finalBlitShader.vsModule,
                .pName  = "main",
            },
            {
                .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                .stage  = VK_SHADER_STAGE_FRAGMENT_BIT,
                .module = output->finalBlitShader.fsModule,
                .pName  = "main",
            },
        };

        VkDynamicState dynamicStates[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};

        MZNT_INTERNAL_VK_CHECKED_CALL(vkCreateGraphicsPipelines(renderer->device, VK_NULL_HANDLE, 1, &(VkGraphicsPipelineCreateInfo)
        {
            .sType                       = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .stageCount                  = sizeof(finalBlitShaderStages) / sizeof(VkPipelineShaderStageCreateInfo),
            .pStages                     = finalBlitShaderStages,
            .pVertexInputState           = &(VkPipelineVertexInputStateCreateInfo)
            {
                .sType                   = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            },
            .pInputAssemblyState         = &(VkPipelineInputAssemblyStateCreateInfo)
            {
                .sType                   = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
                .topology                = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            },
            .pViewportState              = &(VkPipelineViewportStateCreateInfo)
            {
                .sType                   = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
                .viewportCount           = 1,
                .scissorCount            = 1,
            },
            .pRasterizationState         = &(VkPipelineRasterizationStateCreateInfo)
            {
                .sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
                .depthClampEnable        = VK_FALSE,
                .rasterizerDiscardEnable = VK_FALSE,
                .polygonMode             = VK_POLYGON_MODE_FILL,
                .cullMode                = VK_CULL_MODE_BACK_BIT,
                .frontFace               = VK_FRONT_FACE_CLOCKWISE,
                .depthBiasEnable         = VK_FALSE,
                .depthBiasSlopeFactor    = 1.0f,
                .lineWidth               = 1.0f,
            },
            .pMultisampleState           = &(VkPipelineMultisampleStateCreateInfo)
            {
                .sType                   = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
                .rasterizationSamples    = VK_SAMPLE_COUNT_1_BIT,
            },
            .pColorBlendState            = &(VkPipelineColorBlendStateCreateInfo)
            {
                .sType                   = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
                .logicOpEnable           = VK_FALSE,
                .logicOp                 = VK_LOGIC_OP_COPY,
                .attachmentCount         = 1,
                .pAttachments            = &(VkPipelineColorBlendAttachmentState)
                {
                    .colorWriteMask      = VK_COLOR_COMPONENT_R_BIT |
                                           VK_COLOR_COMPONENT_G_BIT |
                                           VK_COLOR_COMPONENT_B_BIT |
                                           VK_COLOR_COMPONENT_A_BIT,
                },
            },
            .pDepthStencilState          = &(VkPipelineDepthStencilStateCreateInfo)
            {
                .sType                   = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
                .depthTestEnable         = VK_FALSE,
                .depthWriteEnable        = VK_FALSE,
            },
            .pDynamicState               = &(VkPipelineDynamicStateCreateInfo)
            {
                .sType                   = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
                .dynamicStateCount       = sizeof(dynamicStates) / sizeof(VkDynamicState),
                .pDynamicStates          = dynamicStates,
            },
            .layout                      = output->finalBlitShader.pipelineLayout,
            .renderPass                  = VK_NULL_HANDLE,
            .pNext                       = &(VkPipelineRenderingCreateInfo)
            {
                .sType                   = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
                .colorAttachmentCount    = 1,
                .pColorAttachmentFormats = &output->swapchainImageFormat.format,
                .depthAttachmentFormat   = VK_FORMAT_UNDEFINED,
                .stencilAttachmentFormat = VK_FORMAT_UNDEFINED,
            },
        }, nil, &output->finalBlitShader.pipeline));

        MZNT_INTERNAL_VK_CHECKED_CALL(vkCreateDescriptorPool(renderer->device, &(VkDescriptorPoolCreateInfo)
        {
            .sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .maxSets       = MZNT_NUM_FRAMES_IN_FLIGHT,
            .flags         = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
            .poolSizeCount = 2,
            .pPoolSizes    = (VkDescriptorPoolSize[])
            {
                {
                    .type            = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
                    .descriptorCount = MZNT_NUM_FRAMES_IN_FLIGHT,
                },
                {
                    .type            = VK_DESCRIPTOR_TYPE_SAMPLER,
                    .descriptorCount = MZNT_NUM_FRAMES_IN_FLIGHT,
                }
            }
        }, nil, &output->finalBlitDescriptorPool));

        MZNT_INTERNAL_VK_CHECKED_CALL(vkAllocateDescriptorSets(renderer->device, &(VkDescriptorSetAllocateInfo)
        {
            .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
            .descriptorPool     = output->finalBlitDescriptorPool,
            .descriptorSetCount = MZNT_NUM_FRAMES_IN_FLIGHT,
            .pSetLayouts        = (VkDescriptorSetLayout[])
            {
                output->finalBlitShader.descriptorSetLayout,
                output->finalBlitShader.descriptorSetLayout,
                output->finalBlitShader.descriptorSetLayout,
            },
        }, output->finalBlitDescriptorSets));
    }

    return output;
}

b8 MZNT_DestroyRendererSurface_Vulkan(MZNT_VulkanRendererSurface* surface, PNSLR_Allocator tempAllocator)
{
    if (!surface) return false;
    if (!surface->renderer) FORCE_DBG_TRAP;

    MZNT_INTERNAL_VK_CHECKED_CALL(vkDeviceWaitIdle(surface->renderer->device));

    vkFreeDescriptorSets(surface->renderer->device, surface->finalBlitDescriptorPool, MZNT_NUM_FRAMES_IN_FLIGHT, surface->finalBlitDescriptorSets);
    vkDestroyDescriptorPool(surface->renderer->device, surface->finalBlitDescriptorPool, nil);
    vkDestroyPipeline(surface->renderer->device, surface->finalBlitShader.pipeline, nil);
    vkDestroyPipelineLayout(surface->renderer->device, surface->finalBlitShader.pipelineLayout, nil);
    vkDestroyDescriptorSetLayout(surface->renderer->device, surface->finalBlitShader.descriptorSetLayout, nil);
    vkDestroyShaderModule(surface->renderer->device, surface->finalBlitShader.fsModule, nil);
    vkDestroyShaderModule(surface->renderer->device, surface->finalBlitShader.vsModule, nil);
    vkDestroySampler(surface->renderer->device, surface->finalBlitSampler, nil);

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
        vkDestroyImageView(surface->renderer->device, surface->depthImageViews.data[i], nil);
        vmaDestroyImage(surface->renderer->vmaAllocator, surface->depthImages.data[i], surface->depthImageAllocations.data[i]);

        vkDestroyImageView(surface->renderer->device, surface->screenImageViews.data[i], nil);
        vmaDestroyImage(surface->renderer->vmaAllocator, surface->screenImages.data[i], surface->screenImageAllocations.data[i]);
    }

    PNSLR_FreeSlice(&(surface->depthImageViews), surface->renderer->parent.allocator, PNSLR_GET_LOC(), nil);
    PNSLR_FreeSlice(&(surface->depthImageAllocations), surface->renderer->parent.allocator, PNSLR_GET_LOC(), nil);
    PNSLR_FreeSlice(&(surface->depthImages), surface->renderer->parent.allocator, PNSLR_GET_LOC(), nil);

    PNSLR_FreeSlice(&(surface->screenImageViews), surface->renderer->parent.allocator, PNSLR_GET_LOC(), nil);
    PNSLR_FreeSlice(&(surface->screenImageAllocations), surface->renderer->parent.allocator, PNSLR_GET_LOC(), nil);
    PNSLR_FreeSlice(&(surface->screenImages), surface->renderer->parent.allocator, PNSLR_GET_LOC(), nil);

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
    {
        vkDestroyImageView(surface->renderer->device, surface->depthImageViews.data[i], nil);
        vmaDestroyImage(surface->renderer->vmaAllocator, surface->depthImages.data[i], surface->depthImageAllocations.data[i]);

        vkDestroyImageView(surface->renderer->device, surface->screenImageViews.data[i], nil);
        vmaDestroyImage(surface->renderer->vmaAllocator, surface->screenImages.data[i], surface->screenImageAllocations.data[i]);
    }

    PNSLR_FreeSlice(&(surface->depthImageViews), surface->renderer->parent.allocator, PNSLR_GET_LOC(), nil);
    PNSLR_FreeSlice(&(surface->depthImageAllocations), surface->renderer->parent.allocator, PNSLR_GET_LOC(), nil);
    PNSLR_FreeSlice(&(surface->depthImages), surface->renderer->parent.allocator, PNSLR_GET_LOC(), nil);

    PNSLR_FreeSlice(&(surface->screenImageViews), surface->renderer->parent.allocator, PNSLR_GET_LOC(), nil);
    PNSLR_FreeSlice(&(surface->screenImageAllocations), surface->renderer->parent.allocator, PNSLR_GET_LOC(), nil);
    PNSLR_FreeSlice(&(surface->screenImages), surface->renderer->parent.allocator, PNSLR_GET_LOC(), nil);

    for (i64 i = 0; i < surface->swapchainImageViews.count; i++)
    {
        vkDestroyImageView(surface->renderer->device, surface->swapchainImageViews.data[i], nil);
    }

    PNSLR_FreeSlice(&(surface->swapchainImageViews), surface->renderer->parent.allocator, PNSLR_GET_LOC(), nil);
    PNSLR_FreeSlice(&(surface->swapchainImages), surface->renderer->parent.allocator, PNSLR_GET_LOC(), nil);

    MZNT_Internal_CreateVkSwapchain(surface, tempAllocator);

    MZNT_Internal_CreateVkSwapchainImagesAndViews(surface, tempAllocator);

    return true;
}

MZNT_VulkanRendererCommandBuffer* MZNT_BeginFrame_Vulkan(MZNT_VulkanRendererSurface* surface, f32 r, f32 g, f32 b, f32 a, PNSLR_Allocator tempAllocator)
{
    MZNT_INTERNAL_VK_CHECKED_CALL(vkWaitForFences(surface->renderer->device, 1, &(surface->inFlightFences.data[surface->curFrame]), VK_TRUE, U64_MAX));
    MZNT_INTERNAL_VK_CHECKED_CALL(vkAcquireNextImageKHR(surface->renderer->device, surface->swapchain, U64_MAX, surface->presentCompleteSemaphores.data[surface->semIdx], VK_NULL_HANDLE, &(surface->curSwpchImgIdx)));
    MZNT_INTERNAL_VK_CHECKED_CALL(vkResetFences(surface->renderer->device, 1, &(surface->inFlightFences.data[surface->curFrame])));

    MZNT_VulkanRendererCommandBuffer* cmdBuf = &(surface->commandBuffers[surface->curFrame]);
    MZNT_INTERNAL_VK_CHECKED_CALL(vkResetCommandBuffer(cmdBuf->cmdBuffer, 0));

    // command buffer start
    MZNT_INTERNAL_VK_CHECKED_CALL(vkBeginCommandBuffer(cmdBuf->cmdBuffer, &(VkCommandBufferBeginInfo)
    {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
    }));

    // screenbuffer: undefined -> rt, swapchain: undefined -> present
    vkCmdPipelineBarrier2(cmdBuf->cmdBuffer, &(VkDependencyInfo)
    {
        .sType                   = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .imageMemoryBarrierCount = 2,
        .pImageMemoryBarriers    = (VkImageMemoryBarrier2[])
        {
            {
                .sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
                .srcStageMask        = VK_PIPELINE_STAGE_2_NONE,
                .srcAccessMask       = VK_ACCESS_2_NONE,
                .dstStageMask        = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
                .dstAccessMask       = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
                .oldLayout           = VK_IMAGE_LAYOUT_UNDEFINED,
                .newLayout           = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                .image               = surface->screenImages.data[surface->curSwpchImgIdx],
                .subresourceRange    =
                {
                    .aspectMask      = VK_IMAGE_ASPECT_COLOR_BIT,
                    .baseMipLevel    = 0,
                    .levelCount      = 1,
                    .baseArrayLayer  = 0,
                    .layerCount      = 1,
                },
            },
            {
                .sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
                .srcStageMask        = VK_PIPELINE_STAGE_2_NONE,
                .srcAccessMask       = VK_ACCESS_2_NONE,
                .dstStageMask        = VK_PIPELINE_STAGE_2_NONE,
                .dstAccessMask       = VK_ACCESS_2_NONE,
                .oldLayout           = VK_IMAGE_LAYOUT_UNDEFINED,
                .newLayout           = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                .image               = surface->swapchainImages.data[surface->curSwpchImgIdx],
                .subresourceRange    =
                {
                    .aspectMask      = VK_IMAGE_ASPECT_COLOR_BIT,
                    .baseMipLevel    = 0,
                    .levelCount      = 1,
                    .baseArrayLayer  = 0,
                    .layerCount      = 1,
                },
            },
        }
    });

    // viewport and scissor
    {
        vkCmdSetViewport(cmdBuf->cmdBuffer, 0, 1, &(VkViewport)
        {
            .width    = (f32) surface->swapchainExtent.width,
            .height   = (f32) surface->swapchainExtent.height,
            .maxDepth = 1.0f,
        });

        vkCmdSetScissor(cmdBuf->cmdBuffer, 0, 1, &(VkRect2D) {.extent = surface->swapchainExtent});
    }

    // bind screen buffer and depth buffer
    vkCmdBeginRendering(cmdBuf->cmdBuffer, &(VkRenderingInfo)
    {
        .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
        .renderArea = {.offset = {0, 0}, .extent = surface->swapchainExtent},
        .layerCount = 1,
        .colorAttachmentCount = 1,
        .pColorAttachments = &(VkRenderingAttachmentInfo)
        {
            .sType       = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
            .imageView   = surface->screenImageViews.data[surface->curSwpchImgIdx],
            .imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            .loadOp      = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp     = VK_ATTACHMENT_STORE_OP_STORE,
            .clearValue  = {.color = {.float32 = {r, g, b, a}}},
        },
        // TODO: bind
        .pDepthAttachment = nil,
        .pStencilAttachment = nil,
    });

    // draw triangle
    {
        vkCmdBindPipeline(cmdBuf->cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, surface->renderer->helloTriangleShader.pipeline);
        vkCmdDraw(cmdBuf->cmdBuffer, 3, 1, 0, 0);
    }

    return cmdBuf;
}

b8 MZNT_EndFrame_Vulkan(MZNT_VulkanRendererSurface* surface, PNSLR_Allocator tempAllocator)
{
    MZNT_VulkanRendererCommandBuffer* cmdBuf = &(surface->commandBuffers[surface->curFrame]);

    vkCmdEndRendering(cmdBuf->cmdBuffer);

    // screenbuffer: rt -> srv, swapchain: present -> rt
    vkCmdPipelineBarrier2(cmdBuf->cmdBuffer, &(VkDependencyInfo)
    {
        .sType                   = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .imageMemoryBarrierCount = 2,
        .pImageMemoryBarriers    = (VkImageMemoryBarrier2[])
        {
            {
                .sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
                .srcStageMask        = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
                .srcAccessMask       = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
                .dstStageMask        = VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT,
                .dstAccessMask       = VK_ACCESS_2_SHADER_SAMPLED_READ_BIT,
                .oldLayout           = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                .newLayout           = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                .image               = surface->screenImages.data[surface->curSwpchImgIdx],
                .subresourceRange    =
                {
                    .aspectMask      = VK_IMAGE_ASPECT_COLOR_BIT,
                    .baseMipLevel    = 0,
                    .levelCount      = 1,
                    .baseArrayLayer  = 0,
                    .layerCount      = 1,
                },
            },
            {
                .sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
                .srcStageMask        = VK_PIPELINE_STAGE_2_NONE,
                .srcAccessMask       = VK_ACCESS_2_NONE,
                .dstStageMask        = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
                .dstAccessMask       = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
                .oldLayout           = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                .newLayout           = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                .image               = surface->swapchainImages.data[surface->curSwpchImgIdx],
                .subresourceRange    =
                {
                    .aspectMask      = VK_IMAGE_ASPECT_COLOR_BIT,
                    .baseMipLevel    = 0,
                    .levelCount      = 1,
                    .baseArrayLayer  = 0,
                    .layerCount      = 1,
                },
            },
        }
    });

    // bind swapchain to output
    vkCmdBeginRendering(cmdBuf->cmdBuffer, &(VkRenderingInfo)
    {
        .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
        .renderArea = {.offset = {0, 0}, .extent = surface->swapchainExtent},
        .layerCount = 1,
        .colorAttachmentCount = 1,
        .pColorAttachments = &(VkRenderingAttachmentInfo)
        {
            .sType       = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
            .imageView   = surface->swapchainImageViews.data[surface->curSwpchImgIdx],
            .imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            .loadOp      = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .storeOp     = VK_ATTACHMENT_STORE_OP_STORE,
        },
        // TODO: bind
        .pDepthAttachment = nil,
        .pStencilAttachment = nil,
    });

    // copy screen buffer to swapchain buffer
    {
        vkCmdBindPipeline(cmdBuf->cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, surface->finalBlitShader.pipeline);

        vkUpdateDescriptorSets(surface->renderer->device, 2, (VkWriteDescriptorSet[])
        {
            {
                .sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                .dstSet          = surface->finalBlitDescriptorSets[surface->curFrame],
                .dstBinding      = 0,
                .descriptorCount = 1,
                .descriptorType  = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
                .pImageInfo      = &(VkDescriptorImageInfo)
                {
                    .imageView   = surface->screenImageViews.data[surface->curSwpchImgIdx],
                    .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                },
            },
            {
                .sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                .dstSet          = surface->finalBlitDescriptorSets[surface->curFrame],
                .dstBinding      = 1,
                .descriptorCount = 1,
                .descriptorType  = VK_DESCRIPTOR_TYPE_SAMPLER,
                .pImageInfo      = &(VkDescriptorImageInfo)
                {
                    .sampler = surface->finalBlitSampler,
                },
            }
        }, 0, nil);

        vkCmdBindDescriptorSets(cmdBuf->cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, surface->finalBlitShader.pipelineLayout,
            0, 1, &(surface->finalBlitDescriptorSets[surface->curFrame]), 0, nil);

        vkCmdDraw(cmdBuf->cmdBuffer, 3, 1, 0, 0);
    }

    vkCmdEndRendering(cmdBuf->cmdBuffer);

    // screenbuffer: srv -> rt, swapchain: rt -> present
    vkCmdPipelineBarrier2(cmdBuf->cmdBuffer, &(VkDependencyInfo)
    {
        .sType                   = VK_STRUCTURE_TYPE_DEPENDENCY_INFO,
        .imageMemoryBarrierCount = 2,
        .pImageMemoryBarriers    = (VkImageMemoryBarrier2[])
        {
            {
                .sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
                .srcStageMask        = VK_PIPELINE_STAGE_2_FRAGMENT_SHADER_BIT,
                .srcAccessMask       = VK_ACCESS_2_SHADER_SAMPLED_READ_BIT,
                .dstStageMask        = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
                .dstAccessMask       = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
                .oldLayout           = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                .newLayout           = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                .image               = surface->screenImages.data[surface->curSwpchImgIdx],
                .subresourceRange    =
                {
                    .aspectMask      = VK_IMAGE_ASPECT_COLOR_BIT,
                    .baseMipLevel    = 0,
                    .levelCount      = 1,
                    .baseArrayLayer  = 0,
                    .layerCount      = 1,
                },
            },
            {
                .sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2,
                .srcStageMask        = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
                .srcAccessMask       = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT,
                .dstStageMask        = VK_PIPELINE_STAGE_2_NONE,
                .dstAccessMask       = VK_ACCESS_2_NONE,
                .oldLayout           = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                .newLayout           = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                .image               = surface->swapchainImages.data[surface->curSwpchImgIdx],
                .subresourceRange    =
                {
                    .aspectMask      = VK_IMAGE_ASPECT_COLOR_BIT,
                    .baseMipLevel    = 0,
                    .levelCount      = 1,
                    .baseArrayLayer  = 0,
                    .layerCount      = 1,
                },
            },
        }
    });

    // command buffer over
    MZNT_INTERNAL_VK_CHECKED_CALL(vkEndCommandBuffer(cmdBuf->cmdBuffer));

    // submit command buffer
    MZNT_INTERNAL_VK_CHECKED_CALL(vkQueueSubmit2(surface->renderer->gfxQueue, 1, &(VkSubmitInfo2)
    {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2,
        .pNext = nil,
        .flags = 0,
        .waitSemaphoreInfoCount = 1,
        .pWaitSemaphoreInfos    = (VkSemaphoreSubmitInfo[])
        {
            {
                .sType     = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
                .semaphore = surface->presentCompleteSemaphores.data[surface->semIdx],
                .value     = 1,
                .stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
            },
        },
        .commandBufferInfoCount = 1,
        .pCommandBufferInfos    = (VkCommandBufferSubmitInfo[])
        {
            {
                .sType         = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
                .commandBuffer = cmdBuf->cmdBuffer,
            },
        },
        .signalSemaphoreInfoCount = 1,
        .pSignalSemaphoreInfos    = (VkSemaphoreSubmitInfo[])
        {
            {
                .sType     = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
                .semaphore = surface->renderFinishedSemaphores.data[surface->curSwpchImgIdx],
                .value     = 1,
                .stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
            },
        },
    }, surface->inFlightFences.data[surface->curFrame]));

    // present
    MZNT_INTERNAL_VK_CHECKED_CALL(vkQueuePresentKHR(surface->renderer->gfxQueue, &(VkPresentInfoKHR)
    {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .pNext = nil,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &(surface->renderFinishedSemaphores.data[surface->curSwpchImgIdx]),
        .swapchainCount = 1,
        .pSwapchains = &(surface->swapchain),
        .pImageIndices = &(surface->curSwpchImgIdx),
        .pResults = nil,
    }));

    // update swapchain indexing
    surface->semIdx = (surface->semIdx + 1) % (u32) surface->presentCompleteSemaphores.count;
    surface->curFrame = (surface->curFrame + 1) % MZNT_NUM_FRAMES_IN_FLIGHT;
    surface->curSwpchImgIdx = U32_MAX; // invalidate

    return true;
}

#endif
