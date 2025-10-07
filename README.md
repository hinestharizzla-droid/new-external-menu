# Sharp Menu Rewrite

A **simplified, sharp-edged menu system** for game overlays.

## Features

✅ **Sharp Design** - Zero rounding, clean edges, flat aesthetic  
✅ **Clean Tabs** - Button-based tab system with accent highlights  
✅ **Lightweight** - 67% less code than original (~500 vs ~1500 lines)  
✅ **No Animations** - Instant response, no performance overhead  
✅ **5 Themes** - Dark, Blue, Purple, Green, Red  
✅ **Full Featured** - Aimbot, ESP, Settings, Config, Debug pages  

## Quick Start

```cpp
#include "menu.h"

// In your main loop:
Menu::RenderMenu();
```

## Menu Pages

| Page | Description |
|------|-------------|
| **Aimbot** | Aim assistance settings (Settings/Advanced/Visual tabs) |
| **ESP** | Visual overlay features (Features/Colors tabs) |
| **Settings** | Theme, crosshair, stream proof |
| **Config** | Presets and custom config management |
| **Debug** | Player list and system information |

## Controls

- **INSERT** - Toggle menu visibility
- **Mouse** - Navigate and interact
- **Click** tabs to switch between sub-pages
- **Click** sidebar buttons to switch pages

## Theme Preview

```
DARK   - Neutral dark gray with blue accent
BLUE   - Dark blue with cyan accent  
PURPLE - Dark purple with magenta accent
GREEN  - Dark green with lime accent
RED    - Dark red with crimson accent
```

## Code Comparison

| Feature | Old | New |
|---------|-----|-----|
| Lines of code | ~1500 | ~500 |
| Rounding values | 8+ | 0 |
| Animation systems | 3 | 0 |
| Optimization layers | Many | None |
| Box renderers | 16 | 0 |
| Theme colors | 11 per theme | 6 per theme |
| Page transitions | Yes | No |
| Performance throttling | Yes | No |

## What's Different?

### Removed
- ❌ 4-box grid layout system
- ❌ Animated topbar window
- ❌ Hover animations
- ❌ Transition effects
- ❌ Gradient backgrounds
- ❌ Glow effects
- ❌ Complex caching
- ❌ Frame throttling

### Simplified
- ✨ Single sidebar + content area
- ✨ Tab buttons instead of custom TabBox
- ✨ Solid colors instead of gradients
- ✨ Direct rendering without optimization
- ✨ 6 colors per theme instead of 11

### Kept
- ✅ All functionality intact
- ✅ Config save/load/import/export
- ✅ Preset system
- ✅ All controls (sliders, toggles, etc.)
- ✅ Theme system

## Integration

See `INTEGRATION_GUIDE.md` for detailed integration instructions.

## Requirements

- **ImGui** - GUI framework
- **nlohmann/json** - Config serialization
- **Windows API** - File dialogs, window management
- **FontAwesome 6** - Icons

## Screenshots

```
┌──────────────────────────────────────┐
│ ──────────────────────────────────── │ ← Accent line
│ ┌────────┐ ┌──────────────────────┐ │
│ │ CODEX  │ │    AIMBOT SETTINGS   │ │
│ │────────│ │                      │ │
│ │ ⌖ Aim  │ │ [Settings][Advanced] │ │
│ │ 👁 ESP  │ │                      │ │
│ │ ⚙ Set  │ │ ☑ Enable Aimbot     │ │
│ │ 📁 Cfg  │ │ Smoothing X [====]  │ │
│ │ 🐛 Dbg  │ │ Smoothing Y [====]  │ │
│ │        │ │ Target Bone: Head ▼ │ │
│ │────────│ │                      │ │
│ │FPS: 144│ │                      │ │
│ │v2.1.0  │ │                      │ │
│ └────────┘ └──────────────────────┘ │
└──────────────────────────────────────┘
```

## License

Use freely in your projects.

---

**Sharp. Simple. Fast.**
