#pragma once

struct _lv_display_t;
using lv_display_t = _lv_display_t;

namespace Garbox::LvglTheme {

/**
 * Install the application theme used by both the embedded display and host
 * framebuffer. Generated GUIs rely on these zero-default styles, so using the
 * stock LVGL theme changes their geometry as well as their appearance.
 */
void Install(lv_display_t* display);

} // namespace Garbox::LvglTheme
