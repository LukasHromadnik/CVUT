//
//  HTTPManager.m
//  BI-iOS-2013_06-REST
//
//  Created by Jakub Hladík on 29.10.13.
//  Copyright (c) 2013 Flowknight s.r.o. All rights reserved.
//

#import "HTTPManager.h"
#import <ACKategories.h>

#define kBaseURLString @"http://private-00d21-biioshomework2.apiary-mock.com/"

@implementation HTTPManager
{
    AFHTTPSessionManager *_imageManager;
}

/*
 pri prvnim zavolani to vytvori GCD frontu ktera zpracovava tasky na pozadi
 */
- (dispatch_queue_t)backgroundCompletionQueue
{
    static dispatch_queue_t _bg_queue;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _bg_queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0);
    });
    
    return _bg_queue;
}

+ (instancetype)sharedManager
{
    /*
     zakomentovany kod dela to same jako to makro
     */
//    static HTTPManager *_sharedInstance;
//    static dispatch_once_t onceToken;
//    dispatch_once(&onceToken, ^{
//        _sharedInstance = [[self alloc] initWithBaseURL:nil];
//    });
//    return _sharedInstance;
    
    DEFINE_SHARED_INSTANCE_USING_BLOCK(^{
        NSURL *url = [NSURL URLWithString:kBaseURLString];
        return [[self alloc] initWithBaseURL:url];
    });
}

- (instancetype)initWithBaseURL:(NSURL *)url
{
    self = [super initWithBaseURL:url];
    if (self) {
        ;
        /*
         druhy sessionManader ktery umi deserializovat obrazky
         */
        NSURL *url = [NSURL URLWithString:kBaseURLString];
        _imageManager = [[AFHTTPSessionManager alloc] initWithBaseURL:url];
        _imageManager.requestSerializer = [AFHTTPRequestSerializer serializer];
        _imageManager.responseSerializer = [AFImageResponseSerializer serializer];
        /*
         callbacky budou bezet na pozadi ve fronte
         */
        [_imageManager setCompletionQueue:[self backgroundCompletionQueue]];
    self.responseSerializer = [AFJSONResponseSerializer new];
    }
    return self;
}

- (NSURLSessionDataTask *)GET:(NSString *)URLString
                   parameters:(NSDictionary *)parameters
                      success:(void (^)(NSURLSessionDataTask *, id))success
                      failure:(void (^)(NSURLSessionDataTask *, NSError *))failure
{
    return [super GET:URLString
           parameters:parameters
              success:^(NSURLSessionDataTask *task, id responseObject){
                  TRC_LOG(@"%ld, %@ %@", (long)((NSHTTPURLResponse *)task.response).statusCode, task.originalRequest.HTTPMethod, task.originalRequest.URL)
                  
                  success(task, responseObject);
                  
                  
              } failure:^(NSURLSessionDataTask *task, NSError *error){
                  TRC_LOG(@"%ld, %@ %@", (long)((NSHTTPURLResponse *)task.response).statusCode, task.originalRequest.HTTPMethod, task.originalRequest.URL)
                  failure(task, error);
              }];
}


@end
