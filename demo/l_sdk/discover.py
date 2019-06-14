﻿#-*-coding:utf-8-*-
"""
///////////////////////////////////////////////////////////////////////////
//  Copyright(c) 2019, 武汉舜立软件, All Rights Reserved
//  Created: 2019/06/14
//
/// @file    discover.py
/// @brief   网络发现接口
/// @version 0.1
/// @warning 没有警告
///////////////////////////////////////////////////////////////////////////
"""

import json             # json.dumps json.loads 
import l_sdk_py         # 从l_sdk_py.pyd文件加载接口


def discover_open(*, cfg = {}):
    """
    /// @brief 打开广播服务模块
    /// @param [in] cfg     字典形式的json配置数据
    """
    config = ''
    try:
        config = json.dumps(cfg)
    except Exception as e:
        pass

    try:
        l_sdk_py.discover_open(config)
    except Exception as e:
        pass


def discover_close():
    """
    /// @brief 关闭广播服务模块
    """
    l_sdk_py.discover_close()


def discover_run(run = True):
    """
    /// @brief 打开/关闭持续搜索
    /// @param [in] run     True.开启搜索; False.关闭搜索
    """
    l_sdk_py.discover_run(run)


def discover_get_devs():
    """
    /// @brief 打开搜索之后, 获取当前网络中的设备
    /// @return {},[] 设备列表
    """
    devs = l_sdk_py.discover_get_devs()

    ret = {}
    try:
        ret = json.loads(devs)
    except Exception as e:
        pass

    return ret
