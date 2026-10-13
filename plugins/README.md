# URM Extensions / Plugins

URM Extensions provides **customizations and extensions**, which are applied on top of the core URM framework.

## Overview

URM provides a standard upstream framework for managing system resources through a well-defined set of resources and signals. However, different targets, segments, and use cases often require:

- **Custom resources** beyond the standard upstream set
- **Target-specific signals** tailored to particular usecase and hardware platforms
- **Specialized resource provisioning logic** for unique scenarios

**URM Extensions** enables developers to:

- Add new custom resources and signals without modifying the core URM codebase
- Override default resource handlers with custom implementations
- Provide target-specific configurations for different hardware platforms
- Extend URM functionality through a clean plugin architecture

## What's Included

As part of Extensions, the following items are included:

- **Extended Configurations**: Custom resource and signal configurations for specific targets
- **Extension Modules**: Plugin implementations that extend URM's core functionality
- **Target-Specific Configs**: Hardware-specific tuning parameters for platforms like qcm6490, qcs8300, qcs9100

## Architecture

```
┌─────────────────────────────────────────────────────────┐
│         Userspace Resource Manager (URM)                │
│  ┌──────────────────────────────────────────────────┐   │
│  │  Standard Upstream Resources & Signals           │   │
│  │  - CPU, GPU, Memory, Cgroups, etc.               │   │
│  │  - Generic app lifecycle signals                 │   │
│  └──────────────────────────────────────────────────┘   │
│                         ▲                               │
│                         │                               │
│                         │ Extension Interface           │
│                         │                               │
└─────────────────────────┼───────────────────────────────┘
                          │
┌─────────────────────────┼────────────────────────────────┐
│                         │                                │
│    URM Extensions (Plugins)                        │
│  ┌──────────────────────┴──────────────────────────┐     │
│  │  Custom Resources & Signals                     │     │
│  │  - GPU resources (power levels, devfreq)        │     │
│  │  - Multimedia signals (video decode, camera)    │     │
│  │  - Target-specific optimizations                │     │
│  └─────────────────────────────────────────────────┘     │
│  ┌──────────────────────────────────────────────────┐    │
│  │  Extensions                                      │    │
│  │  - Custom resource handlers                      │    │
│  │  - Post processing logic                         │    │
│  │  - custom developer/customer specific features   │    │
│  └──────────────────────────────────────────────────┘    │
└──────────────────────────────────────────────────────────┘

```

## When to Use Extensions

Use URM Extensions when you need to:

| Scenario                                                      |                   Solution                                       |
|---------------------------------------------------------------|------------------------------------------------------------------|
| Add hardware-specific resources (e.g., vendor GPU controls)   | Define custom resources in `Configs/ResourcesConfig.yaml`        |
| Implement target-specific signals                             | Add signal configurations in `Configs/SignalsConfig.yaml`        |
| Override default resource provisioning logic                  | Register custom callbacks via extension APIs                     |
| Support multiple hardware variants                            | Use target-specific config directories `Configs/target-specific` |
| Add post-processing or validation logic                       | Implement extension modules                                      |

## Examples: Adding Custom Resources

### Example 1: I/O Scheduler

#### 1. Define the Resource in Config

Add to `Configs/ResourcesConfig.yaml`:

```yaml
ResourceConfigs:
  - ResType: "0x80"                    # 0x80 reserved for Custom resources
    ResID: "0x0001"                    # Custom ID
    Name: "RES_DISK_IO_SCHEDULER"
    Path: "/sys/block/sda/queue/scheduler"
    Supported: true
    HighThreshold: 3                   # Max scheduler index
    LowThreshold: 0                    # Min scheduler index
    Permissions: "third_party"
    Policy: "pass_through"             # Always apply
    # Scheduler mapping: 0=noop, 1=deadline, 2=cfq, 3=bfq
```

#### 2. (Optional) Register Custom Handler

In your extension module:

```cpp
#include <Urm/Extensions.h>

int32_t applyCustomIoScheduler(void* res) {
    // Custom logic to apply I/O scheduler
    // Map numeric value to scheduler name
    const char* schedulers[] = {"noop", "deadline", "cfq", "bfq"};
    // Write scheduler name to sysfs
    // ...
    return 0;
}

int32_t teardownCustomIoScheduler(void* res) {
    // Custom logic to restore default I/O scheduler
    // Reset to system default scheduler
    // ...
    return 0;
}

// Register the custom handlers
URM_REGISTER_RES_APPLIER_CB(0x00800001, applyCustomIoScheduler);
URM_REGISTER_RES_TEAR_CB(0x00800001, teardownCustomIoScheduler);
```

#### 3. Use in Client Code

```cpp
#include <Urm/UrmAPIs.h>

SysResource resource;
resource.mResCode = 0x00060001;  // RES_DISK_IO_SCHEDULER
resource.mResInfo = 0;
resource.mNumValues = 1;
resource.mResValue.value = 2;    // Set scheduler to 'cfq'
int64_t duration = 5000; /*milli seconds*/
int32_t properties = 0;
int32_t numRes = 1;

int64_t handle = tuneResources(duration, properties, numRes, &resource);
```

### Example 2: KGSL GPU

#### 1. Define the Resource in Config

Add to `Configs/ResourcesConfig.yaml`:

```yaml
ResourceConfigs:
  - ResType: "0x80"                    # GPU resource type
    ResID: "0x0003"                    # Custom ID
    Name: "RES_KGSL_DEF_PWRLEVEL"
    Path: "/sys/class/kgsl/kgsl-3d0/default_pwrlevel"
    Supported: true
    HighThreshold: 6
    LowThreshold: 0
    Permissions: "third_party"
    Modes: ["display_on", "doze"]
    Policy: "lower_is_better"
```

#### 2. (Optional) Register Custom Handler

In your extension module:

```cpp
#include <Urm/Extensions.h>

int32_t applyCustomGpuPowerLevel(void* res) {
    // Custom logic to apply GPU power level
    // ...
    return 0;
}

int32_t teardownCustomGpuPowerLevel(void* res) {
    // Custom logic to restore default GPU power level
    // Reset to default power level
    // ...
    return 0;
}

// Register the custom handlers
URM_REGISTER_RES_APPLIER_CB(0x00800003, applyCustomGpuPowerLevel);
URM_REGISTER_RES_TEAR_CB(0x00800003, teardownCustomGpuPowerLevel);
```

#### 3. Use in Client Code

```cpp
#include <Urm/UrmAPIs.h>

SysResource resource;
resource.mResCode = 0x00050003;  // RES_KGSL_DEF_PWRLEVEL
resource.mResInfo = 0;
resource.mNumValues = 1;
resource.mResValue.value = 3;    // Set power level to 3
int64_t duration = 5000; /*milli seconds*/
int32_t properties = 0;
int32_t numRes = 1;

int64_t handle = tuneResources(duration, properties, numRes, &resource);
```

## Documentation

For detailed documentation on:
- Available custom resources and signals
- Extension API reference
- Target-specific configurations
- Development guidelines

Please refer to: [`docs/README.md`](docs/README.md)

## Development

### Adding New Extensions

1. Define your custom resources/signals in the appropriate config files
2. Implement extension modules in the `Extensions/` directory
3. Register callbacks using the extension API
4. Test thoroughly on target hardware
5. Submit a pull request with documentation
