//
//  GenerateTestUserSig.cpp
//  TXLiteAVTestUserSig
//
//  Created by gavinwjwang on 2021/12/2.
//
#include "GenerateTestUserSig.h"
#include "GenerateTestUserSigOC.h"
#include <stdio.h>
const char* GenerateTestUserSig::genTestUserSig(const char* identifier, int sdkAppId, const char* secretKey){

    NSString * strIdentifier = [NSString stringWithFormat:@"%s", identifier];
    NSString * strSecretKey = [NSString stringWithFormat:@"%s", secretKey];
    NSString * strLine = [GenerateTestUserSigOC genTestUserSig:strIdentifier:sdkAppId:strSecretKey];
    //    const char* rm = [GenerateTestUserSigOC genTestUserSig(strIdentifier,sdkAppId,strSecretKey);
    return [strLine UTF8String];
}
