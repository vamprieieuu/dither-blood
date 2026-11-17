# Source audit and preset extraction — verified repository snapshot

## Repositories actually accessible
- Product source: vamprieieuu/Dithering_MAJEED_build_ready (default branch main).
- DITHER BLOOD integration branch: vamprieieuu/dither-blood, research/websites-phase-01.
- The connected GitHub account can read/write the DITHER BLOOD repository. The product source repository is readable.
- The original ECW custom UI archive itself is not present in either connected repository snapshot inspected here; previous Custom UI reports describe the supplied archive but the raw ECW source files cannot be fetched from these repos.

## Files inspected directly
- core/dither.cpp (648 lines): fixed RGB palettes, Bayer matrices, IGN/hashed threshold, screen-pattern generator, 15 error diffusion kernels and render pipeline.
- core/majeed_core.h (core data contracts): RGBA float image, DitherParams, LinesParams, frame context and hash helpers.
- core/lines.cpp (34,654 bytes): Gaussian smoothing, multi-channel Sobel gradients, ridge suppression, contour tracing, subpixel contour sampling and strand rendering.
- ae/ymdithers_ae.cpp (616 lines): AE parameter setup, Smart Pre-Render and Smart Render adapter.
- src/engine/ditherAlgorithms.ts (49 catalog entries, diffusion kernels and threshold helpers).
- src/engine/ditherEngine.ts (731 lines): 8 palette modes and browser-side dither implementation.
- src/engine/paletteUtils.ts: hex conversion, Rec.709 luma and weighted RGB nearest-color metric.
- src/engine/processor.ts (741 lines): browser pixel pipeline, tonal/grade mapping, random coverage and SVG export.
- src/types.ts: DitherStudioParams, LevelsParams, LinesParams and Preset schema.
- src/App.tsx (1,058 lines): React control surface and simulator.
- CMakeLists.txt and package.json: Windows x64 AEX build and web tooling.

## Verified algorithm inventory
The TypeScript catalog contains 49 algorithm entries. The 15 diffusion entries are followed by ordered Bayer, blue-noise/IGN/white-noise, and 27 procedural screen/halftone patterns. Exact IDs/names/categories are in ../presets/native-palettes-and-algorithms.json.

## Preset extraction result
- Extracted all 8 fixed palette/mode definitions found in src/engine/ditherEngine.ts, preserving exact source colors as hex.
- The repository README advertises aesthetic recipes (Game Boy, Newspaper, Macintosh Atkinson, Risograph, Cyberpunk Terminal, CRT Slot-Mask, Xerox Zine), but no recipe objects/parameter payloads were found in the inspected App.tsx or guessed conventional preset module paths. These seven recipe names are therefore references only, not importable presets; do not invent their settings.
- src/types.ts defines a Preset interface, but that alone is not a preset collection.

## Concrete code-review findings
1. **Color-blend channel defect — fixed:** core/dither.cpp apply_color_blend() used qR for the green output. Patched the source repository main branch to use qG (commit 5ab4569a7cfda58fdd054dd628dde18a1e81826c). This one-line fix is committed; a full AE host render regression is still pending.
2. **Randomness parity gap:** src/engine/processor.ts contains Math.random() calls for noise/coverage; these are not reproducible from a seed and can make browser previews differ between runs. Replace with a per-frame seeded PRNG if deterministic parity is required.
3. **Pseudo-blue-noise naming:** the core helper called blue_noise_threshold() is a hash-plus-coordinate decorrelation function, not evidence of a precomputed blue-noise rank tile. Treat it as pseudo-blue-noise until spectral/visual validation.
4. **Frame-rate assumption:** FrameCtx::frame() converts time to frame index using a fixed 30 fps. This can desynchronize animated noise at other compositions' frame rates; pass the host frame/time scale instead.
5. **Performance allocations:** dither and lines paths allocate several std::vector buffers per render. Correctness may be acceptable for first integration, but repeated high-resolution frames can create allocator pressure; profile and consider host-safe scratch buffers/cache after thread-safety review.
6. **Threading:** parallel_rows() in the core header executes a single direct call; it is not parallel. Do not advertise the current core as internally parallel.
7. **AE/UI boundary:** the current AE adapter is a conventional SmartFX effect adapter; the fetched source has no PF_Cmd_EVENT / Drawbot / PF_Param_ARBITRARY_DATA gradient editor. A native ECW panel still needs its own event and persistent-ARB integration.

## Integration readiness
- Portable gradient core was added to DITHER BLOOD in this branch as separate files and compiled/tested independently.
- The top-level CMakeLists uses CONFIGURE_DEPENDS globbing for core/*.cpp, so core/gradient_ramp.cpp is picked up by the plugin target automatically.
- Gradient UI/Drawbot adapter, ARB serialization, AE parameter wiring, preset recipe payloads and host runtime validation remain open.
