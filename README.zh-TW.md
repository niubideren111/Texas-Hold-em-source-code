# 德州私人局原始碼｜朋友局、Unity 場景與伺服器資料

[简体中文](README.zh-CN.md) · [繁體中文](README.zh-TW.md) · [English](README.en.md) · [产品页面](https://niubideren111.github.io/Texas-Hold-em-source-code/zh-tw/)

圍繞私人局、朋友局和俱樂部牌桌的德州撲克程式碼資料。公開儲存庫包含 Unity 登入與大廳場景、C++ 订單和路由服務文件、設定示例，以及牌局流程圖與產品截圖。

**德州私人局原始碼 · 德州朋友局原始碼 · 德州撲克原始碼 · Unity德州場景**

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
    

## 專案重點

### 私人牌桌與俱樂部進入點

截圖覆盖牌桌菜單、加入俱樂部和俱樂部列表，强調熟人組局場景。

### Unity 場景資料

Login.unity、Hall.unity、Splash.unity 展示用戶端場景文件的組織。

### 伺服器與玩法文件

订單、路由文件與私人局/SNG 流程圖提供程式碼閱讀線索。

## 資料閱讀與核對方式

1. **先確認產品形態**：依序檢視截圖與圖說，確認產品類型和可見功能流程。
2. **再核對檔案證據**：直接開啟下方列出的原始碼或文件，不只依賴功能描述。
3. **檢查可建置範圍**：確認欲執行的部分是否具備相依套件、資源、設定與啟動腳本。
4. **確認授權**：閱讀儲存庫授權；商業素材及完整工程交付應另行取得書面授權。

## 產品截圖

![德州私人局多人牌桌](docs/assets/seo/texas-hold-em-source-code-01.jpg)

![德州牌桌設置與菜單](docs/assets/seo/texas-hold-em-source-code-02.jpg)

![德州加入俱樂部介面](docs/assets/seo/texas-hold-em-source-code-03.jpg)

![德州俱樂部列表與進入點](docs/assets/seo/texas-hold-em-source-code-04.jpg)

## 公開原始碼與資料

| 文件 | 说明 |
|---|---|
| [Login.unity](Login.unity) | Unity 登入場景 |
| [Hall.unity](Hall.unity) | Unity 大廳場景 |
| [OrderServantImp.cpp](OrderServantImp.cpp) | 订單服務實作片段 |
| [RouterServer.h](RouterServer.h) | 路由伺服器头文件 |
| [Doc/游戏玩法/GamePlay(Private).png](Doc/%E6%B8%B8%E6%88%8F%E7%8E%A9%E6%B3%95/GamePlay(Private).png) | 私人局玩法流程圖 |

## 開始閱讀

```bash
git clone https://github.com/niubideren111/Texas-Hold-em-source-code.git
cd Texas-Hold-em-source-code
```

## 常見問題

### 與俱樂部原始碼專案如何選擇？

本專案突出私人牌桌、朋友組局、Unity 場景和玩法流程；另一專案突出俱樂部服務介面與建置資料。

### 是否提供 Docker 一鍵啟動？

當前公開文件中没有完整的 Docker Compose 部署工程，本頁不提供一鍵啟動承诺。

## 後續資料完善方向

补充場景所需腳本和資源依赖說明；将 Router 設定整理為脱敏示例，並添加私人局狀態流转文件。 後續更新還應加入版本化相依清單、經過驗證的建置或匯入步驟、簡明架構／產品流程圖，以及能對應真實檔案變更的版本記錄。大型授權資源可放入 GitHub Releases 並提供校驗值，不能提交密鑰、生產位址或使用者資料。

## 相關專案

- [dezhou-poker-club-source-code](https://github.com/niubideren111/dezhou-poker-club-source-code)
- [Texas-Holdem-Game-Source-Code](https://github.com/niubideren111/Texas-Holdem-Game-Source-Code)

## 資料範圍與授權

公開內容是場景、伺服器端片段和設計資料的集合；不将其作為已驗證可直接建置的完整 Unity 工程。 公開內容以實際檔案、相依套件與授權為準，不承諾搜尋排名、直接上線或固定效能結果。

- Telegram: [@fox_lovemyself](https://t.me/fox_lovemyself)
- GitHub: [Texas-Hold-em-source-code](https://github.com/niubideren111/Texas-Hold-em-source-code)
