# Private Table Texas Holdem Source Code | Unity Scenes and C++ Server Materials

[简体中文](README.zh-CN.md) · [繁體中文](README.zh-TW.md) · [English](README.en.md) · [Product page](https://niubideren111.github.io/Texas-Hold-em-source-code/en/)

Texas Holdem materials for private and friend tables, including public Unity login and lobby scenes, selected C++ order and router files, configuration examples and gameplay diagrams.

**private poker source code · Texas Holdem source code · Unity poker source code · friend table poker**

## ✨ Project Highlights

- **🎯 Server-Authoritative Architecture**: All game logic is executed on the server, helping prevent cheating and unauthorized modifications while ensuring fair gameplay.
- **⚡ High-Concurrency Real-Time Communication**: WebSocket-based communication provides low-latency multiplayer gameplay and supports high concurrent traffic.
- **🧠 Built-in AI Bots**: Intelligent bots can be used for automated testing and filling empty seats, making local development and debugging easier.
- **🧩 Modular Design**: The C++ codebase separates business logic from the networking layer, simplifying secondary development and feature expansion.
- **💾 Data Persistence**: Complete hand histories and player activity logs are recorded for anti-cheat analysis, auditing, and operational statistics.
- **🐳 One-Click Deployment**: Docker images and Docker Compose configurations are included, allowing developers to start the complete service stack quickly.

---

## 🎮 Features

| Module | Description |
| :--- | :--- |
| **Classic Texas Hold'em** | Supports 6-max and 9-max tables with complete betting, raising, folding, and showdown logic |
| **Private / Friends Tables** | Create private rooms for friends, with password protection and spectator mode |
| **Club / Union System** | Create or join clubs, organize internal ranked games, and view club performance statistics |
| **Tournament System** | Supports Multi-Table Tournaments (MTT), Sit & Go (SNG), bounty tournaments, and satellite tournaments |
| **Poker Variants** | Supports **Short Deck**, Omaha, and other poker variants |
| **Social and Security Features** | Includes friends, emoji chat, anti-cheat risk control, and operation audit logs |

---

📖 For detailed build options, configuration references, and protocol documentation, see the `docs/` directory in the project root.

## 🏗️ Technical Architecture

```text
Server:
C++11/14/17, built on a high-performance networking framework with support for horizontal scaling on Linux

Client:
Unity 2019+ sample client written in C#, supporting Android, iOS, and PC builds

Communication Protocol:
WebSocket with a custom binary protocol for efficient real-time data transmission

Data Storage:
MySQL for persistent data and Redis for caching frequently accessed data

Supporting Tools:
Administration panel, bot configuration tools, and game log analysis viewer


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
| [Doc/游戏玩法/GamePlay(Private).png](Doc/%E6%B8%B8%E6%88%8F%E7%8E%A9%E6%B3%95/GamePlay(Private).png) | Public PNG file: GamePlay(Private).png. |

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
