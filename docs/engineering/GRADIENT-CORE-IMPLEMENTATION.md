# Gradient core implementation

## Implemented
- Portable C++ core, independent of the AE SDK and separate from the original ECW example.
- Fixed-capacity 10-stop model with minimum two stops and stable IDs.
- Add/move/delete/color editing; non-destructive visibility distinct from deletion.
- Linear, smoothstep and constant interpolation; segment midpoint and stop opacity.
- Luminance/R/G/B/alpha source mapping and reverse ramp.
- Input clamping and finite-value validation; source alpha preserved by pixel application.

## Validation
Compiled with g++ C++17 using -Wall -Wextra -Werror. Unit executable passed interpolation, insertion, visibility, deletion minimum, alpha preservation, reverse and constant-mode assertions.

## Boundary
This is the portable gradient engine only. It is not a completed AE Effect Controls panel: Drawbot drawing/event routing, ARB serialization callbacks, PiPL integration, AE undo/redo and in-host testing remain separate tasks. Original ECW_Gradient sources are not modified or copied here.

## Adapter contract
The future AE adapter should edit a value copy, validate it, persist it as ARB data with correct flatten/compare/copy/interp callbacks, and pass immutable snapshots to rendering. Do not mutate render state from UI callbacks.
