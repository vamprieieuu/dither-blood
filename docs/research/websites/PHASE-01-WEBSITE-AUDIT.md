# Phase 01 — Website Effects Research Audit

**Status:** reviewed; feature catalogue and integration plan recorded. No captured third-party implementation or assets copied.

**Supplied archive sources inspected**
- `dither/analyzied webs/index.html`
- `saveweb2zip-com-filtr-org-uk.zip` — offline capture of Filtr (`filtr.org.uk`)
- `saveweb2zip-com-ultimatedesigntools-com.zip` — offline capture of Ultimate Design Tools Photo Effects Lab

This is a behavioral/architecture audit, not a license determination. Do not copy minified bundles, CSS, fonts, icons or images unless their license expressly permits it.

## A. Filtr — observed feature catalogue

The captured landing page describes image/video/GIF/webcam inputs and real-time retro effects. The application bundle exposes:
- **Dither:** Floyd–Steinberg, Atkinson, Bayer and blue-noise; intensity, levels, gamma, sharpening, color mode and palette.
- **Halftone/dots:** shape, dot scale, spacing, angle, grid type, inversion and color mode.
- **Pixel sort:** direction, brightness/hue/saturation metric, threshold and streak length/intensity.
- **Contours and edges:** fill mode, levels, line thickness, edge algorithm, threshold and line width.
- **Crosshatch/blockify/threshold:** density, layers, angle, randomness, block size/style/border, threshold point and optional dithering.
- **Procedural noise/Voronoi:** noise type, scale, intensity, octaves, speed, cell size and randomization.
- **Matrix Rain:** glyph set, cell size, speed, trail length, direction, glow and color.
- **VHS/CRT/post:** distortion, noise, color bleed, scanlines, tracking error, bloom, grain, chromatic offset, vignette, CRT curvature and phosphor tint.
- **Presets:** built-ins, saved presets, JSON import/export and shareable URL state. Palette examples include grayscale, Game Boy, phosphor green, purple, newspaper, pink, cyberpunk and warm/sepia.

The captured app uses a WebGL2 shader preview path. This is a useful performance reference, but not a reason to begin with GPU code in the AE plug-in.

## B. Photo Effects Lab — observed feature catalogue

The captured page includes a CPU-side image engine with the following registered effects:
1. Color Adjust — brightness, contrast, saturation, hue shift, posterize, invert.
2. Duotone/Tritone — mode and shadow/mid/highlight colors.
3. Threshold — level and invert.
4. Dither — method, levels, color mode, scale.
5. Halftone — cell, angle, ink/background, invert; geometric/vector output.
6. ASCII Art — cell size, colored/mono, ink/background.
7. Crosshatch — cell size and colors; geometric/vector output.
8. Stipple — spacing, density, dot size, colors, invert, seed; geometric/vector output.
9. Edge Detect — strength, ink/background, invert.
10. Mosaic/Pixelate — block size.
11. Pixel Sort — direction, low/high thresholds, maximum run, reverse.
12. Scanlines — spacing, thickness, intensity.
13. Chromatic Aberration — amount, falloff, focal point, inward/outward.
14. Glitch — slices, maximum shift, channel shift, seed.
15. Noise/Grain — amount, size, mono/color, seed.
16. Vignette — amount and softness.
17. Color Overlay — overlay color.

### Stack, preset and render behavior
- Effects form an ordered, non-destructive stack; each effect receives the preceding effect's output. Reordering changes the result.
- Layers conceptually support parameters, master intensity/opacity and blend mode.
- Presets are editable recipes/stacks (examples named Zine/Riso and Newsprint), not just baked images.
- Randomized effects use a deterministic seeded PRNG.
- Halftone, stipple and crosshatch generate geometry primitives and can export SVG; pixel-domain effects remain raster.
- Preview is rendered at a smaller maximum dimension; export re-renders from source at a larger bound, scaling spatial parameters to preserve appearance.
- JSON stack import/export and PNG/SVG export are documented.

## C. DITHER BLOOD integration priorities

### Priority A — foundation
- Non-destructive effect stack with stable layer IDs, enabled state, parameters, opacity and blend mode.
- Shared parameter metadata/schema and range validation.
- Deterministic seed policy.
- Reusable palette/color-mapping subsystem.
- Separate interactive preview quality from final full-resolution render.
- Versioned JSON preset import/export and migration.

### Priority B — effect expansion
- Duotone/tritone; threshold/posterize.
- Halftone, stipple and crosshatch.
- Pixel sorting with threshold-bounded runs.
- Glitch slices/channel displacement.
- Scanlines, VHS/CRT, chromatic aberration, grain/noise, vignette and bloom.
- Edge detection, contour/levels and blockify/mosaic.
- ASCII and Matrix Rain as optional stylized modes.

### Priority C — later
- Video/GIF temporal processing and animated noise.
- GPU acceleration after CPU reference behavior and tests are stable.
- Vector export is not a first milestone for an AE pixel effect.

## D. Proposed native architecture

These are proposed destinations, not files already present:
- `core/effects/` — isolated C++ effect implementations.
- `core/pipeline/` — ordered stack, blending and compositing.
- `core/color/` — transforms, luma, transfer functions and palette mapping.
- `core/geometry/` — reusable halftone/stipple/crosshatch primitives.
- `core/random/` — deterministic PRNG and seed policy.
- `core/presets/` — versioned schema, validation and migration.
- `tests/reference/` — synthetic inputs and golden outputs.
- AE adapter maps AE parameters, pixel formats and render lifecycle to the independent core.

## E. Porting constraints
- Preserve alpha; do not force alpha opaque.
- Explicitly define channel order and color-space behavior; test AE 8/16/32-bpc paths before claiming support.
- Avoid per-pixel allocation; reuse buffers and respect AE render-threading constraints.
- Random effects must be deterministic for a fixed seed/frame and must not share mutable RNG state across threads.
- Guard dimensions, integer overflow, invalid parameters and out-of-bounds sampling.
- Reimplement from independently documented behavior/math; retain required notices for any separately licensed code later adopted.
- Establish reference tests before optimization or GPU work.

## F. Phase result
Website captures reviewed and findings catalogued. No website source code/assets copied. Next phase, as requested: inspect `dither/costuom ui/` (Custom UI) for AE SDK examples, UI patterns, dependencies, licenses and integration candidates.