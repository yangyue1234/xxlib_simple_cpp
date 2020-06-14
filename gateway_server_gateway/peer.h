﻿#pragma once
#include "xx_epoll.h"
#include "xx_datareader.h"
#include <unordered_set>
namespace EP = xx::Epoll;

// 预声明
struct Server;

// 继承 默认 连接覆盖收包函数
struct Peer : EP::TcpPeer {
    // 是否已关闭. true: 拒收数据, 且断线时不再次执行 Dispose  ( 主用于 延迟掐线 )   // todo: 用 shutdown 语句令 fd 只写
    bool closed = false;

    // 继承构造函数
    using EP::TcpPeer::TcpPeer;

    // 拿到 server 上下文引用, 以方便写事件处理代码
    Server &GetServer() const;

    // 收到数据. 切割后进一步调用 OnReceivePackage 和 OnReceiveCommand
    void OnReceive() override;

    // 收到正常包
    virtual void OnReceivePackage(char* const& buf, size_t const& len) = 0;

    // 收到内部指令
    virtual void OnReceiveCommand(char* const& buf, size_t const& len) = 0;

    // 构造内部指令包. LEN + ADDR + cmd string + args...
    template<typename...Args>
    void SendCommand(Args const &... cmdAndArgs) {
        xx::Data d;
        d.Reserve(32);
        d.len = sizeof(uint32_t);
        d.WriteFixed(0xFFFFFFFFu);
        xx::Write(d, cmdAndArgs...);
        *(uint32_t *) d.buf = (uint32_t) (d.len - sizeof(uint32_t));
        this->Send(std::move(d));
    }
};
