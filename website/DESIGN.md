# DESIGN — SenlightAI "Aurora Glass"

Design system for the SenlightAI site (v3). Supersedes the previous xAI-derived dark-canvas spec. The identity: **deep-space canvas, aurora light, frosted glass** — "sentient light." Layout: fixed glass **sidebar** rail + content area.

## Tokens (src/styles/global.css)

### Canvas & lines
| Token | Value | Use |
|---|---|---|
| `--canvas` | `#05060c` | Page background (set on `html`; body is transparent) |
| `--canvas-soft` | `rgba(255,255,255,.04)` | Soft fills, chips |
| `--canvas-mid` | `#3a4158` | Slider gradient mids, muted art |
| `--hairline` | `rgba(255,255,255,.08)` | Glass borders, table rules |
| `--hairline-strong` | `rgba(255,255,255,.16)` | Emphasized borders |

### Aurora accents
| Token | Value |
|---|---|
| `--accent-amber` | `#ffa940` |
| `--accent-cyan` | `#ffc25e` |
| `--accent-breeze` | `#ffb14e` |
| `--accent-sunset` | `#ff7a17` |
| `--accent-dusk` | `#ff8a1e` |
| `--accent-fuchsia` | `#f97316` |
| `--accent-ember` | `#e85f00` |
| `--accent-sunset-soft` | `#ffd9a6` |
| `--accent-twilight` | `#ffe4bd` |

Warm **orange + amber** ramp — the palette is theme-driven orange, not the former violet/cyan aurora.

### Text
`--ink #ffffff` · `--ink-hover #fff4ea` · `--body #cbb8a3` · `--body-mid #8d7a66`

### Glass & glow
- `--glass-bg`: `linear-gradient(180deg, rgba(255,255,255,.06), rgba(255,255,255,.02))`
- `--glass-blur`: `blur(16px) saturate(130%)`
- `--glow-dusk`: `0 0 32px rgba(255,138,30,.28)`
- Canvas: `--canvas #0c0805` (warm near-black) · `--canvas-mid #4a3a26`

### Shape & spacing
`--radius-sm 16px` (cards) · `--radius-lg 24px` (panels) · `--radius-pill 9999px` · 4px spacing scale `--sp-xxs…--sp-4xl` · easing `cubic-bezier(.22,1,.36,1)`

## System rules

1. **Ambient aurora**: `body::before` fixed layer with 4 radial aurora blooms (violet top-right, cyan top-left, sunset low-right, fuchsia bottom-left). `body::after` is a fixed SVG film grain at 5% opacity. Sections must not use opaque backgrounds — glass needs the aurora behind it to read as glass.
2. **Section seams**: `.band--section` and `.footer` separate sections with a centered gradient glow line (violet → cyan), not hairlines.
3. **Glass surfaces**: `.card`, `.pipeline-step`, `.table-wrap`, `.hero-stats` = translucent gradient fill + 1px light border + `backdrop-filter` + inset top seam `inset 0 1px 0 rgba(255,255,255,.08)` + deep drop shadow. Hover: border brightens, lifts `translateY(-2px)`, optional dusk glow.
4. **Typography**: Inter (Google, 300–700) at weight **500** for display, tracking `-0.03em`; sizes `display-xl`→`xs` via `clamp()`. Geist Mono for eyebrows, stats, chips (uppercase, 1.2–1.4px tracking). Eyebrows render in `--accent-breeze`.
5. **Gradient text** (`.gradient-text`): aurora ramp `breeze → twilight → fuchsia → sunset-soft`, background-clip.
6. **Buttons**: `.pill` = glass pill; `.pill--filled` = the only saturated surface — `dusk → fuchsia → sunset` gradient with white top seam and dusk glow. Used sparingly (one per view).
7. **Sidebar**: fixed left glass rail (`--sidebar-w: 248px`, bottom 0, blur 20). Brand mark = 30px rounded-square with violet inner glow around the aurora "S" logo. Links are stacked rows with mono 01–04 indexes; the current page gets `.nav-link[aria-current="page"]`. On the research page, home-section anchors are prefixed with `/`. Under 1100px the rail becomes a static full-width top bar (links hide under 940px). `main` and `.footer` shift right via `--sidebar-w` on desktop.
8. **Hero**: layered aurora (`::before` static blooms + `::after` drifting blurred blobs, 16s alternate). Kicker is a glass pill with pulsing cyan→violet dot. Stats sit in a glass strip.
9. **Pipeline**: vertical glowing rail (violet → cyan → sunset) behind glass step cards; `step-index` = glowing numbered node centered on the rail.
10. **VAD plot**: glass viewport with violet inner bloom; canvas art in cool grays; state point = white core with `rgba(139,92,246,.85)` glow + twilight ring; slider thumbs glow violet.
11. **Motion**: `--ease` everywhere; `aurora-drift` and `dot-pulse` ambient animations; all killed under `prefers-reduced-motion`.
12. **Focus**: `:focus-visible` = 2px `--accent-breeze` outline, 2px offset.

## Logo

Stroked "S" path (light-as-language) carrying the aurora spectrum `cyan → dusk → sunset`, terminating in a white spark node:
- `public/senlight-logo.svg` — transparent, blurred glow duplicate behind the crisp stroke (site/brand use)
- `public/favicon.svg` — same S on `#05060c` rounded badge with violet radial bloom + 1px white seam (tab icon)
- Inline copies in `Nav.astro` (`#sg`) and `Footer.astro` (`#sfg`) keep unique gradient IDs

## Files

- `src/styles/global.css` — full token set + all component styles
- `src/layouts/Layout.astro` — fonts, `theme-color #05060c`, favicon links
- Components: `Nav`, `Hero`, `Pipeline`, `VadModel`, `Plutchik`, `Matrix`, `Footer`
- Pages: `index.astro`, `research.astro` (both consume the same token contract)
