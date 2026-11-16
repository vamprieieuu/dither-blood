# Custom UI Source Map — supplied archive

Source root: `dither/costuom ui/` in the supplied `dither.zip`.

| Source | Useful files | Intended use | Current decision |
|---|---|---|---|
| AE_ECW_Examples / ECW_Gallery | `src/ECW_Gallery_UI.cpp`, `src/ECW_Gallery.cpp`, `src/ECW_Gallery_Arb.cpp` | Drawbot primitives, text, image blit, XY pad, knob, button, curve editor | Strong port reference; verify license before copying |
| AE_ECW_Examples / ECW_Gradient | `src/ECW_Gradient_UI.cpp`, `src/ECW_Gradient.cpp`, `src/ECW_Gradient_Arb.cpp` | Palette/gradient stop editor with persistent ARB state | Strong port reference; verify license before copying |
| AE_ECW_Examples / ECW_Histo | `src/ECW_Histo_UI.cpp`, `src/ECW_Histo.cpp` | Async upstream histogram, live thumbnail, SmartFX preview | Later milestone; runtime status unverified per README |
| After Effects SDK KB | `wave-3/04-effect-ui-custom-drawing.md`, `wave-2/03-mfr-compute-cache.md`, `wave-1/02-effect-plugins.md`, `wave-1/07-data-types-structures.md`, `wave-1/08-error-handling-debugging.md` | API/event/MFR reference | Use as curated reference; check canonical SDK docs |
| Dear ImGui | `imgui.h`, `imgui.cpp`, `imgui_draw.cpp`, `imgui_widgets.cpp`, `backends/*`, `LICENSE.txt` | Separate native tool or future custom host window | MIT; not a Drawbot backend; not selected for ECW |
| NanoVG | `src/nanovg.h`, `src/nanovg.c`, `src/nanovg_gl.h`, `LICENSE.txt` | Vector UI when an OpenGL context is available | zlib; OpenGL/stencil requirement makes it non-direct for ECW |

## Notes from source READMEs
- Gallery and Gradient report compile/PiPL/install verified but AE render/UI behavior unverified.
- Histo similarly documents its async pattern; confirm actual host behavior before relying on it.
- Gallery README warns against an unbounded `PF_RefreshAllWindows` loop from DRAW; do not implement continuous repaint that way.
- NanoVG README says the project is not actively maintained.
