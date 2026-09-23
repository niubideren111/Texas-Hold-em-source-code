# Private Table Texas Holdem Source Code | Unity Scenes and C++ Server Materials

[简体中文](README.zh-CN.md) · [繁體中文](README.zh-TW.md) · [English](README.en.md) · [Product page](https://niubideren111.github.io/Texas-Hold-em-source-code/en/)

Texas Holdem materials for private and friend tables, including public Unity login and lobby scenes, selected C++ order and router files, configuration examples and gameplay diagrams.

**private poker source code · Texas Holdem source code · Unity poker source code · friend table poker**

## Project highlights

- **Private and friend tables:** product screens and workflow materials focused on games among invited players.
- **Poker club entry points:** references for joining a club, browsing club lists and entering club tables.
- **Unity client scenes:** public login, lobby, splash and upgrade scenes that illustrate client organization.
- **C++ server materials:** selected Order, Router and Push service files with configuration and operation scripts.
- **Gameplay diagrams:** private-table, quick-game and SNG workflows for studying room and tournament behavior.
- **Multilingual documentation:** Simplified Chinese, Traditional Chinese and English READMEs plus illustrated Pages content.

## Features

| Area | Included product or repository material |
|---|---|
| Classic Texas Holdem | Multiplayer table, action area and hand-related product screens |
| Private / friend tables | Table creation, entry and gameplay workflow references for invited groups |
| Poker clubs | Club entry, join flow, club list and table screens |
| Login and lobby | Unity login, test-login, lobby, splash and upgrade scenes |
| Quick game / SNG | Quick-game and SNG workflow diagrams |
| Social interaction | Table menus and friend-game interactions visible in product screenshots |
| Order and routing | Selected C++ Order, Router and Push implementation and configuration files |
| Configuration and operations | RouterServer configuration plus start, stop and copy scripts |

## Technical architecture

| Layer | Public material |
|---|---|
| Unity client | `Splash.unity`, `Upgrade.unity`, `Login.unity`, `LoginTest.unity`, `Hall.unity` |
| Gameplay documentation | Private-table, quick-game and SNG diagrams plus product screenshots |
| C++ services | Selected Order, Router and Push service implementations and interfaces |
| Configuration | `config/RouterServer.conf` and application configuration examples |
| Operations | `script/start.sh`, `script/stop.sh`, `script/copy.sh` |

The material describes a client flow from login and lobby to rooms or tables, supported by routing and order services. The feature table covers the visible product and public reference material. Verify dependencies, assets and configuration in the repository before treating any component as independently buildable.

## What this repository presents

### Private tables and clubs

Review table menus, club entry and club-list screens for friend-table products.

### Unity scene files

Inspect Login.unity, Hall.unity and Splash.unity as scene-organization references.

### Server and gameplay documents

Use order, router and gameplay diagram files as server-reading entry points.

## How to evaluate the material

1. **Confirm the product:** review the screenshots and captions to identify the product type and visible workflow.
2. **Inspect the evidence:** open the listed source files or documents instead of relying on feature claims alone.
3. **Check buildability:** verify that required dependencies, assets, configuration and startup scripts are present for the part you intend to run.
4. **Confirm licensing:** read the repository license and obtain written permission for any commercial assets or complete-project delivery.

## Product screenshots

![Private Texas Holdem multiplayer table](docs/assets/seo/texas-hold-em-source-code-01.jpg)

![Poker table settings and menu](docs/assets/seo/texas-hold-em-source-code-02.jpg)

![Join poker club screen](docs/assets/seo/texas-hold-em-source-code-03.jpg)

![Poker club list and entry screen](docs/assets/seo/texas-hold-em-source-code-04.jpg)

## Public source and documents

| File | Description |
|---|---|
| [Login.unity](Login.unity) | Public UNITY file: Login.unity. |
| [Hall.unity](Hall.unity) | Public UNITY file: Hall.unity. |
| [OrderServantImp.cpp](OrderServantImp.cpp) | Public CPP file: OrderServantImp.cpp. |
| [RouterServer.h](RouterServer.h) | Public H file: RouterServer.h. |
| [Doc/游戏玩法/GamePlay(Private).png](Doc/%E6%B8%B8%E6%88%8F%E7%8E%A9%E6%B3%95/GamePlay%28Private%29.png) | Private-table gameplay diagram. |

## Start reading

```bash
git clone https://github.com/niubideren111/Texas-Hold-em-source-code.git
cd Texas-Hold-em-source-code
```

## Questions

### How does this differ from the club repository?

This project emphasizes private tables, friend games and Unity scenes, while the club repository emphasizes club service interfaces and build material.

### Is there a verified Docker deployment?

The public repository does not include a complete verified Docker Compose deployment.

## Documentation roadmap

Future updates should add a versioned dependency list, a verified setup or import procedure, a concise architecture or product-flow diagram, and release notes tied to real file changes. Large authorized assets belong in GitHub Releases with checksums; secrets, production endpoints and user data must never be committed.

## Related repositories

- [dezhou-poker-club-source-code](https://github.com/niubideren111/dezhou-poker-club-source-code)
- [Texas-Holdem-Game-Source-Code](https://github.com/niubideren111/Texas-Holdem-Game-Source-Code)

## Scope and license

The public content combines scenes, selected server files and design documents; it is not presented as a complete verified Unity build. Public files should be evaluated against their actual paths, dependencies and license. No search ranking, production readiness or performance result is guaranteed.

- Telegram: [@fox_lovemyself](https://t.me/fox_lovemyself)
- GitHub: [Texas-Hold-em-source-code](https://github.com/niubideren111/Texas-Hold-em-source-code)
