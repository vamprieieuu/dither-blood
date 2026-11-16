# Phase 02 — Custom UI / After Effects UI Research Audit

**Status:** archive reviewed; native AE UI approach identified; port candidates and constraints documented. No third-party implementation source copied into the product.

**Source folder in supplied archive:** `dither/costuom ui/`

## 1. Executive result

For DITHER BLOOD, the strongest direct path is **After Effects Effect Control Window custom UI using Drawbot**, not embedding another general-purpose UI toolkit into the effect. The archive contains three purpose-built AE effect examples (Gallery, Gradient, Histogram) demonstrating real `PF_Cmd_EVENT` workflows. They align with our target: an AE effect plug-in whose controls live inside Effect Controls.

The existing project also contains a React/TypeScript web studio. That is useful as a design/prototyping/reference surface, but it is not itself a native AE Effect Controls panel. We should keep the processing core independent and decide separately which controls are native AE parameters and which benefit from an ECW canvas.

## 2. AE_ECW_Examples — strongest implementation reference

The examples README describes working C++ After Effects effects using Drawbot, with custom UI canvases represented by `PF_ADD_NULL` parameters and dispatched by `event_extra->effect_win.index`.

### 2.1 ECW_Gallery — reusable widgets
Source paths in the supplied archive:
- `AE_ECW_Examples-main/ECW_Gallery/src/ECW_Gallery.cpp`
- `AE_ECW_Examples-main/ECW_Gallery/src/ECW_Gallery.h`
- `AE_ECW_Examples-main/ECW_Gallery/src/ECW_Gallery_UI.cpp`
- `AE_ECW_Examples-main/ECW_Gallery/src/ECW_Gallery_Arb.cpp`
- `AE_ECW_Examples-main/ECW_Gallery/CMakeLists.txt`

Widgets and techniques:
- Drawbot primitives and text: lines, rectangles, circles, arcs, triangles, cubic Bezier paths, stroke/fill and UTF-16 labels.
- `NewImageFromBuffer` + `DrawImage` for raster UI blits; RGB/ARGB channel ordering can be tested with colored bands.
- XY pad mapping mouse coordinates to two float parameters.
- Rotary knob using `atan2`.
- Clickable button backed by a checkbox parameter.
- ARB-backed curve editor with draggable points, add/delete interactions, cubic-Hermite interpolation and a 256-entry LUT used by the render.
- Multiple canvases routed by `effect_win.index`.
- Shared Drawbot helper functions for backgrounds, labels, shapes and UTF conversion.

**DITHER BLOOD uses:** curve editor for tone/response shaping; XY pad for spatial angle/center controls; knob for angle/intensity; image-blit for palette/texture thumbnails; buttons for reset/randomize actions; shared drawing helpers.

### 2.2 ECW_Gradient — multi-stop gradient editor
Source paths:
- `AE_ECW_Examples-main/ECW_Gradient/src/ECW_Gradient.cpp`
- `AE_ECW_Examples-main/ECW_Gradient/src/ECW_Gradient.h`
- `AE_ECW_Examples-main/ECW_Gradient/src/ECW_Gradient_UI.cpp`
- `AE_ECW_Examples-main/ECW_Gradient/src/ECW_Gradient_Arb.cpp`
- `AE_ECW_Examples-main/ECW_Gradient/CMakeLists.txt`

Observed behavior:
- Gradient ramp drawn as a per-pixel bar with triangular stop handles.
- Drag to move stops, clamped between neighbors; click to add a stop sampled from the existing ramp.
- Double-click opens AE color picker; Alt/Option-click or drag below the bar deletes a stop, retaining at least two.
- Stop list is ARB arbitrary data, so it persists with the project and supports undo/redo.
- Render maps luminance/R/G/B/alpha through the gradient and supports blending with original.
- Example reports 8/16/32-bit support and MFR-safe render design.
- Alpha is deliberately passed through; per-stop alpha is not implemented.

**DITHER BLOOD uses:** custom palette editor, multi-ink risograph palette, gradient/duotone mapping, palette stop persistence and undo/redo.

### 2.3 ECW_Histo — async live histogram and preview
Source paths:
- `AE_ECW_Examples-main/ECW_Histo/src/ECW_Histo.cpp`
- `AE_ECW_Examples-main/ECW_Histo/src/ECW_Histo.h`
- `AE_ECW_Examples-main/ECW_Histo/src/ECW_Histo_UI.cpp`
- `AE_ECW_Examples-main/ECW_Histo/CMakeLists.txt`

Observed behavior:
- Requests the upstream frame asynchronously from `PF_Event_DRAW` using the RenderAsyncManager/context async manager pattern.
- Draws the most recently completed histogram while a new request is pending; the host redraws when the frame is ready.
- Shows a live histogram plus a procedural noise thumbnail.
- Uses `NewImageFromBuffer` for smooth raster preview inside Drawbot.
- Provides a Levels control set (input black/white, gamma, output black/white), channel choice and a noise group.
- Demonstrates SmartFX pre-render/render, parameter checkout/checkin, coordinate scaling and 8/16/32-bit processing.
- The README explicitly marks runtime UI behavior as unverified in After Effects; compile/PiPL/install status is reported separately.

**DITHER BLOOD uses:** optional source histogram for levels/threshold tuning, preview thumbnails, palette analysis and input-aware controls. Async frame checkout is a later feature after a basic Drawbot panel is stable.

## 3. After Effects SDK knowledge base — relevant areas

The supplied `after-effects-sdk-kb-main` reference contains:
- `wave-3/04-effect-ui-custom-drawing.md`: custom UI flags, event routing, Drawbot suites, click/drag/hit testing, HiDPI, ARB UI state and performance practices.
- `wave-2/03-mfr-compute-cache.md`: MFR threading model, sequence-data constraints, compute cache and thread-safety pitfalls.
- `wave-1/02-effect-plugins.md`: effect lifecycle and parameter/render architecture.
- `wave-1/07-data-types-structures.md`: pixel/world and SDK data types.
- `wave-1/08-error-handling-debugging.md`: error handling and diagnostics.

Key rules extracted from the reference:
- Custom UI is enabled through `PF_OutFlag_CUSTOM_UI`; a parameter canvas uses `PF_PUI_CONTROL` and the appropriate UI dimensions/flags.
- `PF_Cmd_EVENT` handles `PF_Event_DRAW`, `PF_Event_DO_CLICK`, `PF_Event_DRAG`, cursor and context events.
- Drawbot drawing belongs only in `PF_Event_DRAW`; do not draw inside click/drag handlers.
- UI event selectors execute on the UI thread. Render selectors may run concurrently with UI activity under MFR.
- Treat render data as immutable/read-only where required; do not mutate shared global/static state or sequence data from concurrent render calls.
- Do not trigger an endless repaint loop by calling `PF_RefreshAllWindows` from DRAW. The ECW Gallery README documents a real self-refresh loop failure; if animation is ever needed, gate it on recent real user interaction and stop after a short idle timeout.
- Drawbot is for UI, not the actual frame render buffer.

These notes are a design guide; the current installed SDK headers and official Adobe documentation remain authoritative when implementing.

## 4. Dear ImGui and NanoVG — license and fit

### Dear ImGui
- Present in `imgui-master/`; the supplied license is MIT (copyright 2014–2026 Omar Cornut).
- General immediate-mode UI toolkit with platform/renderer backends.
- The archive includes many backends (Win32, DX11/12, OpenGL, Vulkan, SDL, Metal, etc.), but no After Effects Effect Controls / Drawbot backend.
- It could be useful for a separate native companion tool or a custom host window only if a supported window/render/input integration is designed.
- It is **not a drop-in replacement** for AE's ECW custom UI and should not be wired into render callbacks.

### NanoVG
- Present in `nanovg-master/`; supplied license is zlib.
- Antialiased vector drawing API modeled after HTML Canvas.
- The supplied implementation uses OpenGL 2 / GLES2 / GL3 / GLES3 and requires an OpenGL context with stencil buffer.
- AE's Drawbot surface does not automatically provide that OpenGL context. Therefore NanoVG is not the direct ECW route for this plug-in.
- Could be evaluated later for a standalone companion app or a deliberately engineered GPU surface, but it adds a renderer/context lifecycle that Drawbot avoids.

### Decision
Use native Drawbot for the actual After Effects Effect Controls UI. Keep ImGui and NanoVG as optional reference technologies, not current runtime dependencies. Their permissive licenses do not remove the need to preserve notices and check bundled subcomponent/font licenses if they are ever vendored.

## 5. UI layout proposal for DITHER BLOOD

**First native ECW milestone**
1. A compact header/toolbar canvas: effect enable state, reset, randomize and preset indicator.
2. A palette/gradient canvas: editable color stops, selected stop, add/delete and AE color picker.
3. A curve canvas: tonal response curve with editable control points and LUT-based render.
4. A halftone/dither preview thumbnail: use `NewImageFromBuffer` only for UI visualization.
5. Optional histogram canvas after async upstream frame support is proven.

Keep ordinary AE sliders/popups/checkboxes for exact numeric parameters, keyframability and accessibility. Use canvases only where spatial or visual interaction materially improves control. Do not hide every parameter inside a custom canvas.

## 6. Porting and validation sequence
1. Verify the target SDK headers expose the exact Drawbot/custom UI suites and event signatures used by the examples.
2. Start with one harmless Drawbot canvas and verify draw, resize, HiDPI and event routing in AE.
3. Add one click/drag control backed by normal AE parameters.
4. Add ARB state only for genuinely variable-length structures (gradient stops/curve points); implement flatten/copy/compare/interp correctly for undo/project persistence.
5. Test undo/redo, save/reopen, duplicate effect instances and multiple canvases.
6. Test UI and render concurrently under MFR; no shared mutable state.
7. Verify 8/16/32-bpc and alpha behavior independently.
8. Add async histogram/live preview only after the basic UI is stable.
9. Test on the actual supported AE versions and Windows builds. The example READMEs say compile status is verified but AE runtime UI behavior is unverified.

## 7. Licensing and source-copy decision
- **AE_ECW_Examples:** no license file was present in the supplied archive. The README says widgets are intended to be cherry-picked, but that alone is not a formal license. Keep it as a reference; obtain/verify the upstream license before copying its implementation into DITHER BLOOD.
- **After Effects SDK knowledge base:** its own compilation/organization is stated to be MIT-licensed, while Adobe API signatures and excerpts remain subject to their original source terms. Use as a navigation/reference aid and retain notices if its compilation text is redistributed.
- **Dear ImGui:** MIT; preserve LICENSE.txt if vendored.
- **NanoVG:** zlib; preserve LICENSE.txt and mark modified source if changed.
- **Fonts/examples bundled with NanoVG:** separate licenses apply (e.g. OFL/CC BY-SA/Apache as noted by its README); do not assume the library license covers them.

## 8. Phase result
Custom UI sources are reviewed and mapped to DITHER BLOOD use cases. The strongest implementation candidates are Drawbot Gallery widgets, Gradient editor and (later) async Histogram. No third-party UI code was copied into the plug-in because the AE example source archive has no explicit license and ImGui/NanoVG are not direct ECW backends. Next: consolidate the reviewed website + custom UI requirements into the plug-in architecture, then begin implementing the native C++ core/UI in small tested increments.