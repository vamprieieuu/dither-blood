# Website Research — DITHER BLOOD Integration Matrix

| Candidate | Inspiration | Proposed native area | AE controls / notes | Priority |
|---|---|---|---|---|
| Error diffusion, ordered and blue-noise dither | Filtr + Photo Effects Lab | `core/effects/dither_*` | Method, levels, scale, palette, gamma | A |
| Palette mapping | Filtr | `core/color/` | Built-in and custom palettes | A |
| Effect stack | Photo Effects Lab | `core/pipeline/` | Reorder, enable, duplicate, opacity, blend | A |
| Deterministic randomness | Both | `core/random/` | Explicit seed and frame policy | A |
| Duotone/tritone | Photo Effects Lab | `core/effects/duotone.*` | Shadow/mid/highlight colors | B |
| Halftone | Both | `core/geometry/halftone.*` | Shape, spacing, angle, dot scale | B |
| Stipple/crosshatch | Both | `core/geometry/` | Density, spacing, layers, angle, seed | B |
| Pixel sort | Both | `core/effects/pixel_sort.*` | Direction, metric, threshold interval, max run | B |
| Glitch/channel shift | Both | `core/effects/glitch.*` | Slices, shift, channel offset, seed | B |
| Scanlines/VHS/CRT | Both | `core/effects/retro_post.*` | Start with spatial controls; temporal later | B |
| Grain/vignette/bloom/chromatic | Both | `core/effects/post/` | Explicit toggles/intensity | B |
| Edge/contour/blockify | Filtr | `core/effects/` | Algorithm, threshold, levels, block controls | B |
| ASCII/Matrix Rain | Filtr + Photo Effects Lab | `core/effects/glyph/` | Glyph rendering/output mode | C |
| SVG geometry export | Photo Effects Lab | Geometry API | Not an initial AE pixel-effect milestone | C |
| Video/GIF/webcam | Filtr | Future temporal pipeline | AE frames are host-driven, not browser input | C |

## Acceptance criteria for every integrated effect
1. Pure core implementation with no AE SDK dependency.
2. Explicit parameter struct, validated ranges and documented defaults.
3. Defined alpha and color-space behavior.
4. Deterministic output when randomness is involved.
5. Tests for 1×1, odd dimensions, alpha edges and extreme parameters.
6. Numerical or image reference tests.
7. AE adapter maps parameters without changing core semantics.
8. Preview and final render agree within a documented tolerance.