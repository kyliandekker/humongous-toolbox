# Humongous Toolbox

A file archive viewer for Humongous Entertainment game data files. Supports parsing, browsing, and previewing assets from games built on the Humongous Engine (SCUMM variant) titles such as *Putt-Putt*, *Freddi Fish*, *Pajama Sam*, and *SpyFox*.

## Modules

| Module | Type | Description |
|---|---|---|
| `htb_lib` | Static library | Core parsing library for file I/O, data streams, chunk tree parsing, wave loading (cross-platform) |
| `htb_lib_win32` | Static library | Win32 specifics like window, DX11 |
| `htb_patcher` | Win32 executable | GUI application that is used for patching SpyFox 3 to Dutch |
| `htb_app` | Win32 executable | GUI application that is used for visual preview of archives, not meant for replacing |
| `htb_cli` | Console executable | CLI tool for batch operations (cross-platform) |

## Building

### Requirements

- CMake 3.20+
- A C++20 compiler (MSVC 2022, GCC 10+ or Clang 12+)

`htb_lib` and `htb_cli` build on Windows, Linux and macOS. The GUI modules (`htb_lib_win32`, `htb_patcher`, `htb_app`) are only configured on Windows and require Visual Studio 2022.

### Build

This project uses CMake. On any platform:

```sh
cmake -S . -B build
cmake --build build --target htb_cli
```

On Windows you can instead run ```generate.ps1```, which generates a Visual Studio solution in `build/` and opens it.

## Known Supported Formats

| Extension | Type | Contents |
|---|---|---|
| `.HE0` | Index file | Room names, file directory tables |
| `.HE2` | Talk bank | Voice clips |
| `.HE4` | Song file | Music tracks |
| `.(A)` | Resource archive | Rooms, scripts, images, audio, costumes |

Other formats might be supported but have not been tested.

## Third-Party Libraries

- [Dear ImGui](https://github.com/ocornut/imgui) Immediate-mode GUI (with docking)
- [ImPlot](https://github.com/epezent/implot) Plotting extension for ImGui
- [NanoSVG](https://github.com/memononen/nanosvg) SVG parser and rasterizer
- [stb_image](https://github.com/nothings/stb) Image loading and saving
- [RapidJSON](https://github.com/Tencent/rapidjson) JSON parser/serializer

## License

See individual third-party libraries for their respective licenses.

## Future plans

- Make Linux library for patching and inspecting on Linux.
- Replace images and backgrounds.
- Look into costumes and character sprites.
- Do more research on the rest of the extensions for archives such as HE1, HE3, HE6, HE8 and HE9.