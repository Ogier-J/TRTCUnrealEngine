//  QTSimpleDemo
//
//  Copyright © 2020 tencent. All rights reserved.
//

#ifndef QTMACDEMO_BASE_DEFS_H_
#define QTMACDEMO_BASE_DEFS_H_

/**
 * 仅仅for test 
 */
static const int testSDKAppID = 1400582850;
static const char *testUserId = "a9715d2fe80d6dda38db9af2dd7621ad";
static const char *testUserSig = "eJwtzc0OgjAQBOB36dlgWygtJB40US4ciL-nwrZkgwKhjZoY310EjvvNTPZDzvkpeJqBpIQHlKymG8G0Hi1OrBPJBHBrFIUYQIcKykRbDiBjzjQsGweN7nsEkrKIUqG4EnROPD7MqHEoE8ZUFM1q3j0Of5dc0aXpsB4fXnBvX9nR*TtGJT*svcvarhCsaLrmGubbpJQ7396qrKo35PsDN4M4yA__";
static const char *testStrRoomId = "110";

/**
 * 腾讯云 SDKAppId，需要替换为您自己账号下的 SDKAppId。
 *
 * 进入腾讯云实时音视频[控制台](https://console.cloud.tencent.com/rav) 创建应用，即可看到 SDKAppId，
 * 它是腾讯云用于区分客户的唯一标识。
 */
static const int SDKAppID = 1400188366;

/**
 * 计算签名用的加密密钥，获取步骤如下：
 *
 * step1. 进入腾讯云实时音视频[控制台](https://console.cloud.tencent.com/rav)，如果还没有应用就创建一个，
 * step2. 单击您的应用，并进一步找到“快速上手”部分。
 * step3. 点击“查看密钥”按钮，就可以看到计算 UserSig 使用的加密的密钥了，请将其拷贝并复制到如下的变量中
 *
 * 注意：该方案仅适用于调试Demo，正式上线前请将 UserSig 计算代码和密钥迁移到您的后台服务器上，以避免加密密钥泄露导致的流量盗用。
 * 文档：https://cloud.tencent.com/document/product/647/17275#Server
 */
static const char *SECRETKEY = "217a9b4a174649a8a41ea7166faa8666e0973a3312ef9b20ad1ad52e9bbb5e94";

/**
 *  签名过期时间，建议不要设置的过短
 *
 *  时间单位：秒
 *  默认时间：7 x 24 x 60 x 60 = 604800 = 7 天
 */
static const int EXPIRETIME = 604800;

#endif  // QTMACDEMO_BASE_DEFS_H_
