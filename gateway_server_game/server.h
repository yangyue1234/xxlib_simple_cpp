﻿#pragma once

#include "xx_epoll.h"
#include <unordered_map>

namespace EP = xx::Epoll;

// 预声明
struct GListener;
struct GPeer;
struct Dialer;
struct LPeer;

// 服务本体
struct Server : EP::Context {
    // 等待 gateway 接入的监听器
    std::shared_ptr<GListener> gatewayListener;

    // gateway peers
    std::unordered_map<uint32_t, std::shared_ptr<GPeer>> gps;

    // 连到 lobby 的拨号器
    std::shared_ptr<Dialer> lobbyDialer;

    // 保存 lobby 连接
    std::shared_ptr<LPeer> lobbyPeer;

    // 继承构造函数
    using EP::Context::Context;

    // 根据 config 进一步初始化各种成员. 并于退出时清理
    int Run() override;

    // 帧逻辑：遍历 dialerPeers 检查 Peer 状态并自动拨号
    int FrameUpdate() override;
};
