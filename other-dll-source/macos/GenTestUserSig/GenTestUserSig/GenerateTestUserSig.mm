#import "GenerateTestUserSig.h"
#import <CommonCrypto/CommonCrypto.h>
#import <zlib.h>
#import <Foundation/Foundation.h>

/** TRTC的bizid的appid用于转推直播流，https://console.cloud.tencent.com/rav 点击【应用】【帐号信息】
 * 在【直播信息】中可以看到bizid和appid，分别填到下面这两个符号
 */
#define TX_BIZID 0

#define TX_APPID 0

/**
 *  签名过期时间，建议不要设置的过短
 *
 *  时间单位：秒
 *  默认时间：7 x 24 x 60 x 60 = 604800 = 7 天
 */
static const int _EXPIRETIME = 604800;

NSString *hmac(NSString *plainText, NSString *key){
    const char *cKey  = [key cStringUsingEncoding:NSUTF8StringEncoding];
    const char *cData = [plainText cStringUsingEncoding:NSUTF8StringEncoding];

    unsigned char cHMAC[CC_SHA256_DIGEST_LENGTH];

    CCHmac(kCCHmacAlgSHA256, cKey, strlen(cKey), cData, strlen(cData), cHMAC);

    NSData *HMACData = [[NSData alloc] initWithBytes:cHMAC length:sizeof(cHMAC)];
    return [HMACData base64EncodedStringWithOptions:0];
}

NSString *base64URL(NSData *data)
{
    NSString *result = [data base64EncodedStringWithOptions:0];
    NSMutableString *final = [[NSMutableString alloc] init];
    const char *cString = [result cStringUsingEncoding:NSUTF8StringEncoding];
    for (int i = 0; i < result.length; ++ i) {
        char x = cString[i];
        switch(x){
            case '+':
                [final appendString:@"*"];
                break;
            case '/':
                [final appendString:@"-"];
                break;
            case '=':
                [final appendString:@"_"];
                break;
            default:
                [final appendFormat:@"%c", x];
                break;
        }
    }
    return final;
}

const char* GenerateTestUserSig::genTestUserSig(const char* identifier, int sdkAppId, const char* key){
    CFTimeInterval current = CFAbsoluteTimeGetCurrent() + kCFAbsoluteTimeIntervalSince1970;
    long TLSTime = floor(current);
    NSMutableDictionary *obj = [@{@"TLS.ver": @"2.0",
                                  @"TLS.identifier": [NSString stringWithUTF8String:identifier],
                                  @"TLS.sdkappid": @(sdkAppId),
                                  @"TLS.expire": @(_EXPIRETIME),
                                  @"TLS.time": @(TLSTime)} mutableCopy];
    NSMutableString *stringToSign = [[NSMutableString alloc] init];
    NSArray *keyOrder = @[@"TLS.identifier",
                          @"TLS.sdkappid",
                          @"TLS.time",
                          @"TLS.expire"];
    for (NSString *key in keyOrder) {
        [stringToSign appendFormat:@"%@:%@\n", key, obj[key]];
    }
    NSLog(@"%@", stringToSign);
    //NSString *sig = [self sigString:stringToSign];
    NSString *sig = hmac(stringToSign, [NSString stringWithUTF8String:key]);

    obj[@"TLS.sig"] = sig;
    NSLog(@"sig: %@", sig);
    NSError *error = nil;
    NSData *jsonToZipData = [NSJSONSerialization dataWithJSONObject:obj options:0 error:&error];
    if (error) {
        NSLog(@"[Error] json serialization failed: %@", error);
        return [@"" UTF8String];
    }

    const Bytef* zipsrc = (const Bytef*)[jsonToZipData bytes];
    uLongf srcLen = jsonToZipData.length;
    uLong upperBound = compressBound(srcLen);
    Bytef *dest = (Bytef*)malloc(upperBound);
    uLongf destLen = upperBound;
    int ret = compress2(dest, &destLen, (const Bytef*)zipsrc, srcLen, Z_BEST_SPEED);
    if (ret != Z_OK) {
        NSLog(@"[Error] Compress Error %d, upper bound: %lu", ret, upperBound);
        free(dest);
        return [@"" UTF8String];
    }
    NSString *result = base64URL([NSData dataWithBytesNoCopy:dest length:destLen]);
    return [result UTF8String];
};





