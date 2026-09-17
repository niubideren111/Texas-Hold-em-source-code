# 德州私人局源码｜朋友局、Unity 场景与服务器资料

[简体中文](README.zh-CN.md) · [繁體中文](README.zh-TW.md) · [English](README.en.md) · [产品页面](https://niubideren111.github.io/Texas-Hold-em-source-code/zh-cn/)

围绕私人局、朋友局和俱乐部牌桌的德州扑克代码资料。公开仓库包含 Unity 登录与大厅场景、C++ 订单和路由服务文件、配置示例，以及牌局流程图与产品截图。

**德州私人局源码 · 德州朋友局源码 · 德州扑克源码 · Unity德州场景**

## ✨ 项目亮点

- **🎯 服务器权威架构**：所有游戏逻辑在服务端执行，有效防止作弊和外挂，保证游戏公平性。
- **⚡ 高并发实时通信**：基于 WebSocket 协议，实现低延迟、高并发的多人实时对战体验。
- **🧠 内置 AI Bot**：内置智能机器人，可用于功能测试或自动填充空闲桌位，方便单机调试。
- **🧩 模块化设计**：C++ 代码结构清晰，业务逻辑与网络层分离，易于二次开发和功能扩展。
- **💾 数据持久化**：完整记录手牌历史、玩家行为日志，便于反作弊分析和数据统计。
- **🐳 一键部署**：提供 Docker 镜像和 docker-compose 配置，新手也能快速启动完整服务。

---

## 🎮 功能特性

| 模块 | 功能说明 |
| :--- | :--- |
| **经典德州扑克** | 支持 6 人桌 / 9 人桌，完整的下注、加注、弃牌、摊牌逻辑 |
| **私人局 / 朋友局** | 好友约局、创建私人房间，支持密码保护和观战模式 |
| **俱乐部 / 联盟系统** | 创建或加入俱乐部，进行内部排名赛，支持俱乐部战绩统计 |
| **锦标赛系统** | 支持多桌锦标赛 (MTT)、坐满即玩 (SNG)、赏金赛、卫星赛 |
| **特色玩法** | 支持**短牌 (Short Deck)**、奥马哈 (Omaha) 等多种扑克变体 |
| **社交与安全** | 内置好友系统、表情聊天，以及智能反作弊风控模块和操作日志审计 |

---

📖 详细的编译选项、配置文件说明和协议文档，请查看项目根目录下的 docs/ 文件夹。

##🏗️ 技术架构

    服务端：C++ 11/14/17，基于高性能网络框架，支持 Linux 环境水平扩展

    客户端：提供 Unity 2019+ (C#) 示例客户端，可打包 Android / iOS / PC 平台

    通信协议：WebSocket + 自定义二进制协议，保证数据传输效率

    数据存储：MySQL 存储持久化数据，Redis 缓存热数据和高频访问信息

    配套工具：包含后台管理系统、机器人配置工具、游戏日志分析查看器

## 项目重点

### 私人牌桌与俱乐部入口

截图覆盖牌桌菜单、加入俱乐部和俱乐部列表，强调熟人组局场景。

### Unity 场景资料

Login.unity、Hall.unity、Splash.unity 展示客户端场景文件的组织。

### 服务器与玩法文档

订单、路由文件与私人局/SNG 流程图提供代码阅读线索。

## 资料阅读与核对方式

1. **先确认产品形态**：依次查看截图和图注，确认产品类型与可见功能流程。
2. **再核对文件证据**：直接打开下方列出的源码或文档，不只依赖功能描述。
3. **检查可构建范围**：确认准备运行的部分是否具备依赖、资源、配置和启动脚本。
4. **确认授权**：阅读仓库许可；商业素材及完整工程交付应另行取得书面授权。

## 产品截图

![德州私人局多人牌桌](docs/assets/seo/texas-hold-em-source-code-01.jpg)

![德州牌桌设置与菜单](docs/assets/seo/texas-hold-em-source-code-02.jpg)

![德州加入俱乐部界面](docs/assets/seo/texas-hold-em-source-code-03.jpg)

![德州俱乐部列表与入口](docs/assets/seo/texas-hold-em-source-code-04.jpg)

## 公开源码与资料

| 文件 | 说明 |
|---|---|
| [Login.unity](Login.unity) | Unity 登录场景 |
| [Hall.unity](Hall.unity) | Unity 大厅场景 |
| [OrderServantImp.cpp](OrderServantImp.cpp) | 订单服务实现片段 |
| [RouterServer.h](RouterServer.h) | 路由服务器头文件 |
| [Doc/游戏玩法/GamePlay(Private).png](Doc/%E6%B8%B8%E6%88%8F%E7%8E%A9%E6%B3%95/GamePlay(Private).png) | 私人局玩法流程图 |

## 开始阅读

```bash
git clone https://github.com/niubideren111/Texas-Hold-em-source-code.git
cd Texas-Hold-em-source-code
```

## 常见问题

### 与俱乐部源码项目如何选择？

本项目突出私人牌桌、朋友组局、Unity 场景和玩法流程；另一项目突出俱乐部服务接口与构建资料。

### 是否提供 Docker 一键启动？

当前公开文件中没有完整的 Docker Compose 部署工程，本页不提供一键启动承诺。

## 后续资料完善方向

补充场景所需脚本和资源依赖说明；将 Router 配置整理为脱敏示例，并添加私人局状态流转文档。 后续更新还应加入版本化依赖清单、经过验证的构建或导入步骤、简明架构/产品流程图，以及能对应真实文件变化的版本记录。大型授权资源可放入 GitHub Releases 并提供校验值，不能提交密钥、生产地址或用户数据。

## 相关项目

- [dezhou-poker-club-source-code](https://github.com/niubideren111/dezhou-poker-club-source-code)
- [Texas-Holdem-Game-Source-Code](https://github.com/niubideren111/Texas-Holdem-Game-Source-Code)

## 资料范围与许可

公开内容是场景、服务端片段和设计资料的集合；不将其作为已验证可直接构建的完整 Unity 工程。 公开内容以实际文件、依赖和许可为准，不承诺搜索排名、直接上线或固定性能结果。

- Telegram: [@fox_lovemyself](https://t.me/fox_lovemyself)
- GitHub: [Texas-Hold-em-source-code](https://github.com/niubideren111/Texas-Hold-em-source-code)
