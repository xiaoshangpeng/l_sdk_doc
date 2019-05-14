﻿#include <stdio.h>
#include "l_sdk.h"
#include "proto/l_nspp.h"
#include "proto/l_net.h"
#include "proto/l_media.h"
#include "proto/l_md_buf.h"


#ifdef __L_WIN__
#include <windows.h>
static int wsa_startup()
{
    //windows上使用socket, 需要对WS2_32.DLL库进行初始化
    WSADATA wsa;
    if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
    {
        return 1;
    }

    return 0;
}
#else
static int wsa_startup() { return 0; }
#endif


// https://github.com/lishaoliang/l_sdk_doc/blob/master/protocol/auth.md
#define T_LOGIN_STR     "{\"ip\":\"%s\",\"port\":%d,\"cmd\":\"login\",\"login\":{\"username\":\"%s\",\"passwd\":\"%s\"}}"

// https://github.com/lishaoliang/l_sdk_doc/blob/master/protocol/stream.md
#define T_REQ_STREAM_STR   "{\"cmd\":\"open_stream\",\"open_stream\":{\"chnn\":%d,\"idx\":%d}}"


static int cb_sdk_media(void* p_obj, int protocol, int id, int chnn, int idx, int md_id, l_md_buf_t* p_data)
{
    // p_obj 是调用 l_sdk_md_add_listener函数的第三个参数指针, 这里没有用到
    // protocol 为 proto/l_nspp.h 文件中定义主子协议类型
    // id 为登录ID
    // chnn 设备通道
    // idx 流序列号 参见 "proto/l_media.h" l_md_idx_e

    int proto_main = L_NET_PROTO_MAIN(protocol);
    int proto_sub = L_NET_PROTO_SUB(protocol);

    if (p_data->ver == L_MD_F_VER)
    {
        printf("fmt:%d,type:%d,len:%d\n", p_data->f_v1.fmt, p_data->f_v1.v_type, p_data->f_v1.len);

        if (L_FMT_H264 == p_data->f_v1.fmt)
        {
            if (L_MVT_I == p_data->f_v1.v_type)
            {
                // 关键帧
            }
            else
            {
                // 非关键帧
            }

            // H264
            int h264_len = p_data->f_v1.len;
            unsigned char* p_h264 = p_data->p_buf + p_data->start;

            printf("h264 [%x,%x,%x,%x,%x]\n", p_h264[0], p_h264[1], p_h264[2], p_h264[3], p_h264[4]);
        }
        else if (L_FMT_H265 == p_data->f_v1.fmt)
        {

        }
        else if (L_FMT_AUDIO_B < p_data->f_v1.fmt && p_data->f_v1.fmt < L_FMT_AUDIO_E)
        {

        }
        else if (L_FMT_PIC_B < p_data->f_v1.fmt && p_data->f_v1.fmt < L_FMT_PIC_E)
        {

        }
    }

    return 0;
}

static int request_stream(id, chnn, idx)
{
    // 请求码流
    char req_stream[128] = { 0 };
    snprintf(req_stream, 124, T_REQ_STREAM_STR, chnn, idx);

    char* p_res = NULL;
    int ret = l_sdk_request(id, req_stream, &p_res);
    printf("(%s.%d)ret=%d,res=%s\n", __FILE__, __LINE__, ret, (NULL != p_res) ? p_res : "error!");

    if (NULL != p_res)
    {
        l_sdk_free(p_res);
    }
    return 0;
}

int t_stream_main(int argc, char *argv[])
{
    // win socket环境
    wsa_startup();

    // sdk初始化
    int ret = l_sdk_init("");
    printf("(%s.%d)sdk init,ret=%d\n", __FILE__, __LINE__, ret);


    // 添加媒体数据监听者
    l_sdk_md_add_listener("my listener 1", cb_sdk_media, NULL);


    // 请求登录
    char req_login[128] = { 0 };
    snprintf(req_login, 124, T_LOGIN_STR, "192.168.1.247", 80, "admin", "123456");

    int id = 0;
    ret = l_sdk_login(&id, req_login);
    printf("(%s.%d)sdk login,ret=%d,id=%d\n", __FILE__, __LINE__, ret, id);

    if (0 == ret)
    {
        // 登录成功, 后请求流
        request_stream(id, 0, 0);

        // 
        for (int i = 0; i < 10; i++)
        {
            Sleep(1000);
        }
    }

    // 请求登出
    l_sdk_logout(id);

    // sdk退出
    l_sdk_quit();
    return 0;
}
