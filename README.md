# JSON Detector

This is a small library to detect boundaries of JSON in a stream.

## Why?

In our implementation of a JSON-RPC server, we need to be able to detect the start and the end of a JSON message coming over a socket. A single connection can accept multiple JSON-RPC requests, and the JSON parser we are currently using lacks nice push parsing capabilities.

This library supports asynchronous push parsing and is implemented as a C++ generator. It extracts and `co_yield`'s the messages to the consumer. It does not validate the extracted JSON though, and it can yield a malformed JSON to the consumer. This is the expected behavior, as the proper parsing is handled by another library.

The supported messages are JSON arrays and objects; all other things result in errors, Again, this is the expected behavior because, according to the [JSON-RPC 2.0 specification](https://www.jsonrpc.org/specification), the request object must be either a JSON object or, in case of a batch request, array.

## Build

```sh
cmake -B build
cmake --build build
sudo cmake --install build
```

### Dependencies

  * [wwa-coro](https://github.com/sjinks/coro-cpp);
  * [GTest](https://github.com/google/googletest) (to build unit tests);
  * [benchmark](https://github.com/google/benchmark) (to build benchmarks).

All dependencies are installed with `vcpkg`; please make sure to run `git submodule update --init` after cloning this repository.

### Configuration Options

| Option Name              | Description                                                          | Default Value |
|--------------------------|----------------------------------------------------------------------|---------------|
| BUILD_SHARED_LIBS        | Build shared libraries                                               | OFF           |
| BUILD_TESTING            | Whether to enable tests                                              | ON            |
| BUILD_BENCHMARKS         | Whether to enable benchmarks                                         | ON            |
| INSTALL_JSON_DETECTOR    | Whether to enable install targets                                    | ON            |
| ENABLE_MAINTAINER_MODE   | Enable maintainer mode (noisy warnings and treat warnings as errors) | OFF           |
| USE_CLANG_TIDY           | Use `clang-tidy` for linting                                         | OFF           |

## Usage

```cmake
find_package(psb-json-detector CONFIG REQUIRED)
target_link_libraries("${PROJECT_NAME}" PRIVATE psb::json-detector)
```

```cpp
#include <iostream>
#include <string>
#include <psb/json_detector/json_detector.h>

int main()
{
    psb::json_detector detector;

    while (!std::cin.eof()) {
        std::string s;
        std::getline(std::cin, s);
        auto&& generator = detector.process(s);
        for (auto& chunk : generator) {
            std::cout << chunk << "\n";
        }
    }

    return detector.is_finished() ? 0 : 1;
}
```
