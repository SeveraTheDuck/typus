# C++ Project Template

By **SeveraTheDuck** 🦆

A production-grade C++ project template with deterministic environments,
modular CMake infrastructure, and a full CI/CD pipeline.

![C++ Standard](https://img.shields.io/badge/C%2B%2B-23-blue.svg)
![CMake](https://img.shields.io/badge/CMake-3.28%2B-green.svg)
![Nix](https://img.shields.io/badge/Nix-Flakes-orange.svg)
![License](https://img.shields.io/badge/license-Unlicense-lightgrey.svg)

---

## Key Features

- **Reproducible Environment** — Nix Flakes ensures every developer and CI
  runner uses identical compiler versions, tools, and libraries.
- **Zero-Install Workflow** — integrated with `direnv`. Enter the project
  directory and the environment is ready automatically.
- **Modular CMake** — each concern lives in its own `cmake/*.cmake` module.
  Library type, C++ standard, sanitizers, LTO — all configurable without
  touching source files.
- **Flexible Library Types** — switch between `STATIC`, `SHARED`, and
  `HEADER_ONLY` via a single CMake cache variable.
- **Quality Gates** — pre-commit hooks run `clang-format`, `clang-tidy`,
  `cmake-format`, and `nixfmt` before every commit.
- **Targeted CI** — test matrix is driven by `tests/pipelines/*.json`.
  Each test target declares exactly which build configurations it needs.
- **Profiling** — `perf` + FlameGraph integration for Linux. Record and
  visualize performance with a single script.
- **Documentation** — Doxygen with Doxygen Awesome theme, auto-deployed to
  GitHub Pages on every push to `main`.

---

## Tech Stack

| Tool | Version | Role |
| :--- | :--- | :--- |
| CMake | 3.28+ | Build system |
| Ninja | latest | Build generator |
| Clang | 21+ | Compiler + tooling |
| GCC | 15+ | Compiler |
| GTest | 1.15+ | Unit testing |
| clang-tidy | 21+ | Static analysis |
| clang-format | 21+ | Code formatting |
| CodeQL | — | Security analysis |
| Doxygen | latest | Documentation |
| perf + FlameGraph | — | Profiling (Linux only) |

---

## Project Structure

```text
.
├── .github/
│   ├── workflows/
│   │   ├── pipeline.yaml     # Matrix build & test
│   │   ├── docs.yaml         # Doxygen deploy (main only)
│   │   └── codeql.yaml       # Security analysis (main + dev)
│   └── dependabot.yaml       # Action version updates
├── cmake/                    # Modular CMake configuration
│   ├── Options.cmake         # All project options & cache variables
│   ├── TargetHelpers.cmake   # add_project_library, target_install_package
│   ├── TestHelpers.cmake     # add_project_test
│   ├── WorkloadHelpers.cmake # add_project_workload
│   ├── StandardFlags.cmake   # Compiler warnings, LTO, hardening
│   ├── Sanitizers.cmake      # ASan, UBSan, TSan
│   ├── StaticAnalysis.cmake  # clang-tidy integration
│   ├── Dependencies.cmake    # FetchContent / find_package helpers
│   ├── PackageHelpers.cmake  # CMake package config generation
│   ├── CompilerCheck.cmake   # Minimum compiler version enforcement
│   └── Ccache.cmake          # ccache integration
├── nix/
│   ├── commonPackages.nix    # Shared tool dependencies
│   ├── mkCppShell.nix        # Dev shell factory
│   └── systems.nix           # Supported platforms
├── profiling/
│   ├── flamegraph/           # Submodule: brendangregg/FlameGraph
│   ├── scripts/
│   │   ├── record.sh         # perf record wrapper
│   │   ├── flamegraph.sh     # Generate SVG from perf.data
│   │   └── run.sh            # record + flamegraph in one step
│   └── results/              # gitignored output directory
├── source/
│   └── project_name/         # Library source (flat: .hpp and .cpp together)
├── tests/
│   ├── pipelines/            # CI test matrix definitions (*.json)
│   └── group_name/           # Test group (one target per directory)
├── workloads/                # Profiling workloads (built with profile preset)
├── flake.nix                 # Nix environment
├── CMakeLists.txt            # Root build file
├── CMakePresets.json         # All build/test/CI presets
├── .clang-format             # Code style
├── .clang-tidy               # Static analysis rules
└── .pre-commit-config.yaml   # Pre-commit hooks
```

---

## Quick Start

### Prerequisites

Install the [Nix package manager](https://nixos.org/download) with Flakes
enabled, and [direnv](https://direnv.net/).

### 1. Clone & Enter the Environment

```bash
git clone --recurse-submodules https://github.com/you/project_name
cd project_name
direnv allow   # loads the Nix environment automatically
```

Or without direnv:

```bash
nix develop    # default (Clang)
nix develop .#gcc
```

### 2. Build & Test

```bash
cmake --preset dev
cmake --build --preset dev -j$(nproc)
ctest --preset dev
```

### 3. Available Presets

| Preset | Type | Description |
| :--- | :--- | :--- |
| `dev` | Debug | Default development build |
| `dev-clang` | Debug | Explicit Clang |
| `dev-gcc` | Debug | Explicit GCC |
| `asan` | Debug | ASan + UBSan |
| `asan-clang` | Debug | ASan + UBSan (Clang) |
| `tsan` | Debug | Thread Sanitizer |
| `profile` | RelWithDebInfo | Profiling workloads enabled |
| `release` | Release | LTO enabled |
| `release-clang` | Release | Release with Clang |
| `release-shared` | Release | Shared library build |

CI presets (`ci-debug`, `ci-asan`, `ci-tsan`, `ci-release`) are used by the
build pipeline and can also be used locally to reproduce CI builds exactly.

---

## Using This Template

After cloning, replace `project_name` with your project name and `severa`
with your namespace in the following places:

- `CMakeLists.txt` — `project()` call and `PRJ_NAMESPACE`
- `CMakePresets.json` — all `project_name_*` cache variables
- `tests/pipelines/*.json` — target names
- `source/project_name/` — directory name
- `tests/` — target names in `CMakeLists.txt` files and test files

## Configuration

All options are prefixed with the project name and set via CMake cache
variables or presets.

| Option | Default | Description |
| :--- | :--- | :--- |
| `<name>_LIBRARY_TYPE` | `STATIC` | Library type: `STATIC`, `SHARED`, `HEADER_ONLY` |
| `<name>_CXX_STANDARD` | `23` | C++ standard |
| `<name>_ENABLE_ASAN` | `OFF` | Address Sanitizer |
| `<name>_ENABLE_UBSAN` | `OFF` | Undefined Behavior Sanitizer |
| `<name>_ENABLE_TSAN` | `OFF` | Thread Sanitizer |
| `<name>_ENABLE_LTO` | `OFF` | Link Time Optimization |
| `<name>_ENABLE_CCACHE` | `ON` | ccache compiler cache |
| `<name>_ENABLE_CLANG_TIDY` | `OFF` | Run clang-tidy at build time |
| `<name>_ENABLE_WORKLOADS` | `OFF` | Build profiling workloads |

Example — build as shared library with clang-tidy:

```bash
cmake --preset dev \
  -D<name>_LIBRARY_TYPE=SHARED \
  -D<name>_ENABLE_CLANG_TIDY=ON
```

---

## Adding a New Module

**1. Create source files** in `source/project_name/`:

```text
source/project_name/
  module/
    module.hpp
    module.cpp
```

Files are globbed automatically — no changes to `source/CMakeLists.txt`
needed.

**2. Write tests** in a new subdirectory:

```text
tests/
  module/
    CMakeLists.txt     # add_project_test(project_name_module_tests)
    test_module.cpp
```

Register in `tests/CMakeLists.txt`:

```cmake
add_subdirectory(module)
```

**3. Add to CI** in `tests/pipelines/*.json`:

```json
{
  "tests": [
    {
      "targets": ["project_name_module_tests"],
      "presets": ["ci-debug", "ci-asan", "ci-release"]
    }
  ]
}
```

---

## Profiling (Linux only)

Profiling requires the FlameGraph submodule:

```bash
git submodule update --init profiling/flamegraph
```

**1. Build a workload** in `workloads/`:

```text
workloads/
  my_workload/
    CMakeLists.txt     # add_project_workload(project_name_my_workload)
    workload.cpp
```

Register in `workloads/CMakeLists.txt`:

```cmake
add_subdirectory(my_workload)
```

**2. Build and record:**

```bash
cmake --preset profile
cmake --build --preset profile --target project_name_my_workload
./profiling/scripts/run.sh build/profile/workloads/my_workload/project_name_my_workload
```

**3. Open the result:**

```bash
# SVG flamegraph saved to profiling/results/perf.svg
xdg-open profiling/results/perf.svg
```

---

## CI/CD Architecture

```text
push to any branch
  └── Build & Test (build.yaml)
        ├── prepare: reads tests/pipelines/**/*.json → generates matrix
        └── matrix: compiler × preset × target (parallel)

push to main or dev
  └── CodeQL Analysis (codeql.yaml)

push to main
  └── Docs Deploy (docs.yaml) → GitHub Pages
```

Test targets and their configurations are declared in
`tests/pipelines/**/*.json`. Adding a new test group requires no changes to
the workflow files.

---

## Platform Support

Linux and macOS are supported. Windows is not supported.
Clang and GCC are supported. MSVC is not supported.

> **Note:** Profiling (`perf` + FlameGraph) is Linux only.

---

## License

[The Unlicense](LICENSE) — public domain. Use freely.
