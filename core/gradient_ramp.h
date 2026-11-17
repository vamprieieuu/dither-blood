#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
namespace dither_blood {
constexpr std::size_t kMaxGradientStops=10,kMinGradientStops=2;
enum class GradientInterpolation:std::uint8_t{Linear,Smooth,Constant};
enum class GradientSource:std::uint8_t{Luminance,Red,Green,Blue,Alpha};
struct RGBA{float r{0},g{0},b{0},a{1};};
struct GradientStop{float position{0};RGBA color{};float midpoint{.5f},opacity{1};bool visible{true};std::uint32_t stable_id{0};};
struct Gradient{std::array<GradientStop,kMaxGradientStops> stops{};std::size_t count{2};GradientInterpolation interpolation{GradientInterpolation::Linear};GradientSource source{GradientSource::Luminance};bool reverse{false};};
struct GradientResult{RGBA color{};bool valid{false};};
bool validate_gradient(Gradient&) noexcept;bool add_stop(Gradient&,float,RGBA,std::uint32_t) noexcept;bool move_stop(Gradient&,std::uint32_t,float) noexcept;bool set_stop_visible(Gradient&,std::uint32_t,bool) noexcept;bool delete_stop(Gradient&,std::uint32_t) noexcept;bool set_stop_color(Gradient&,std::uint32_t,RGBA) noexcept;GradientResult sample_gradient(const Gradient&,float) noexcept;float source_value(const RGBA&,GradientSource) noexcept;RGBA apply_gradient(const Gradient&,const RGBA&) noexcept;
}
