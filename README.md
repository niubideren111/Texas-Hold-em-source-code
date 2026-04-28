# 🃏 德州扑克源码 | 德州俱乐部源码 | 德州扑克服务器引擎

**德州源码 · 德州联盟源码 · 德州私人局 · 德州朋友局源码**  
实时多人**德州扑克**高并发服务器引擎（C++ + WebSocket），支持私人局/朋友局/俱乐部/联盟模式

**Texas Hold'em Multiplayer Poker Source Code** | **Real-time Multiplayer Poker Server Engine** | Scalable Game Backend

[![Stars](https://img.shields.io/github/stars/pokerdeveloper/dezhou-poker-source-code.svg)](https://github.com/pokerdeveloper/dezhou-poker-source-code/stargazers)
[![Forks](https://img.shields.io/github/forks/pokerdeveloper/dezhou-poker-source-code.svg)](https://github.com/pokerdeveloper/dezhou-poker-source-code/network/members)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

---

## ✨ 项目核心亮点

- **服务器权威架构**：所有游戏逻辑在服务端执行，有效防止作弊和外挂
- **高并发实时通信**：基于 WebSocket 低延迟同步，支持高并发多人对战
- **完整德州支持**：经典德州（9人/6人桌）、私人局、朋友局、俱乐部/联盟模式
- **AI Bot 支持**：内置智能机器人，用于测试或自动填充桌位
- **模块化高扩展**：C++ 代码结构清晰，易于二次开发和功能扩展
- **数据持久化**：支持手牌记录、玩家行为日志，便于反作弊和数据分析

> **⚠️ 重要声明**：本项目**仅供学习和研究使用**，严格禁止用于任何真实货币赌博。商业使用请自行遵守当地法律法规，作者不承担任何法律责任。

---

## 📸 项目截图（真实界面展示）
![德州扑克游戏大厅](screenshots/01.jpg)

![牌桌实时对战](screenshots/02.jpg)

![大厅多种玩法](screenshots/03.jpg)

![俱乐部与联盟模式](screenshots/04.jpg)

![MTT 锦标赛界面](screenshots/05.jpg)

![短牌玩法展示](screenshots/06.jpg)

![AI Bot 测试](screenshots/08.jpg)

![游戏对战场景](screenshots/10.jpg)

![房间管理界面](screenshots/11.jpg)

![后台与数据统计](screenshots/12.jpg)

![更多功能展示](screenshots/13.jpg)


## 🚀 快速开始

**Docker 一键启动**（推荐新手）

git clone https://github.com/pokerdeveloper/dezhou-poker-source-code.git
cd dezhou-poker-source-code
docker-compose up -d

源码编译运行：
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j8
./poker_server

##
详细编译、配置和协议文档请查看 docs/ 目录
## 🎮 核心功能

| 模块 | 功能说明 |
| :--- | :--- |
| **金币大厅** | 坐满即玩、快速匹配、多级别桌、每日比赛 |
| **俱乐部/私人局** | 好友约局、联盟创建、俱乐部赛、战绩统计 |
| **锦标赛系统** | 多桌MTT、SNG、 bounty锦标赛、卫星赛 |
| **特色玩法** | 德州、**短牌**、奥马哈等 |
| **社交与安全** | 好友系统、聊天表情、**智能反作弊系统**、日志审计 |

## 📈 未来路线图

支持更多扑克变体（奥马哈、短牌等）
增加 SNG/MTT 锦标赛完整流程
优化分布式部署与高并发性能
提供 Unity / Web 示例客户端

欢迎 Issue 反馈和 Pull Request 贡献！

## 📞 联系方式

Telegram：@fox_lovemyself

欢迎技术交流、部署咨询或合作讨论。

## 📦 技术交付物

*   **客户端源码**：Unity 2019+，C# 编写，支持打包 Android/iOS/PC。
*   **服务端源码**：C++ 高并发框架，Linux环境，支持水平扩展。
*   **数据库**：MySQL + Redis 结构，含全部表结构文档。
*   **配套工具**：后台管理系统、机器人配置工具、游戏日志查看器。
*   **部署服务**：可选 **协助部署到你的服务器** 服务。

---

## 🚀 为什么选择我们？

1.  **经过市场验证**：源码来自**已停服的成熟商业项目**，绝非Demo。
2.  **代码质量可靠**：结构清晰，注释完善，**众多技术开发者评价“易于二开”**。
3.  **服务保障**：提供1年技术支持和答疑，帮你度过上线初期。
4.  **合规建议**：我们了解各地棋牌政策，可提供合规运营建议。

## ⭐ 支持项目
如果你觉得这个德州扑克源码有帮助，欢迎 Star 支持！


再次声明：本仓库为开源学习项目，不包含任何支付或真实赌博功能。请遵守当地法律法规，合理使用。




This project focuses on real-time game architecture and system design.

For advanced implementations or custom game backend systems, professional discussions are welcome.


