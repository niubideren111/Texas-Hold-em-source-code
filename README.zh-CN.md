# 德州扑克源码｜私人局、德州俱乐部、Unity 场景与服务器资料

[简体中文](README.zh-CN.md) · [繁體中文](README.zh-TW.md) · [English](README.en.md) · [产品页面](https://niubideren111.github.io/Texas-Hold-em-source-code/zh-cn/)

围绕私人局、朋友局和俱乐部牌桌的德州扑克代码资料。公开仓库包含 Unity 登录与大厅场景、C++ 订单和路由服务文件、配置示例，以及牌局流程图与产品截图。

**德州源码 · 德州扑克源码 · 德州私人局源码 · 德州俱乐部 · Unity 德州场景**

## 项目亮点

- **私人局与朋友局产品形态**：围绕熟人组局、牌桌菜单和房间流程展示经典德州扑克玩法。
- **德州俱乐部入口**：提供加入俱乐部、俱乐部列表和俱乐部牌桌等产品界面参考。
- **Unity 客户端场景**：公开登录、大厅、启动和升级等 Unity 场景文件，便于理解客户端组织方式。
- **C++ 服务端资料**：包含订单、路由和 Push 等服务代码片段，以及可核对的配置与运维脚本。
- **玩法流程图**：收录私人局、快速游戏和 SNG 流程资料，便于梳理房间和比赛业务。
- **多语言文档**：提供简体中文、繁體中文、English README 与图文产品页面。

## 功能特性

| 模块 | 功能说明 |
|---|---|
| 经典德州扑克 | 展示多人牌桌、操作区域与牌局相关界面 |
| 私人局 / 朋友局 | 面向好友组局的建桌、进桌和牌局流程资料 |
| 德州俱乐部 | 展示俱乐部入口、加入流程、列表与牌桌场景 |
| 登录与大厅 | 包含 Unity 登录、测试登录、大厅、启动和升级场景 |
| 快速游戏 / SNG | 提供快速游戏及 SNG 玩法流程图，方便理解业务步骤 |
| 社交互动 | 产品截图呈现牌桌菜单及熟人牌局相关交互 |
| 订单与路由 | 公开 C++ Order、Router、Push 相关实现片段和配置 |
| 配置与运维 | 包含 RouterServer 配置以及启动、停止、复制脚本 |

## 技术架构

| 层级 | 公开资料 |
|---|---|
| Unity 客户端 | `Splash.unity`、`Upgrade.unity`、`Login.unity`、`LoginTest.unity`、`Hall.unity` |
| 玩法资料 | 私人局、快速游戏、SNG 流程图及产品截图 |
| C++ 服务端 | Order、Router、Push 等服务实现与接口片段 |
| 配置层 | `config/RouterServer.conf` 及应用配置示例 |
| 运维脚本 | `script/start.sh`、`script/stop.sh`、`script/copy.sh` |

从资料关系看，客户端通过登录与大厅进入房间或牌桌，并由路由、订单等服务承接相应业务。功能表描述的是产品展示与公开资料范围；能否独立构建和运行，应以仓库实际依赖、资源及配置是否齐全为准。

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
| [Doc/游戏玩法/GamePlay(Private).png](Doc/%E6%B8%B8%E6%88%8F%E7%8E%A9%E6%B3%95/GamePlay%28Private%29.png) | 私人局玩法流程图 |

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
