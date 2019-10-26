//
//  APIWrapper.m
//  BI-IOS-REST
//
//  Created by Dominik Vesely on 03/11/14.
//  Copyright (c) 2014 Ackee s.r.o. All rights reserved.
//

#import "APIWrapper.h"
#import "Place.h"
#import "HTTPManager.h"

@implementation APIWrapper

+ (void)placesSuccess:(void (^)(NSArray *))success failure:(void (^)())failure
{
    NSParameterAssert(success);
    NSParameterAssert(failure);
    
    [[HTTPManager sharedManager] GET:@"points"
                          parameters:nil
                             success:^(NSURLSessionDataTask *task, id responseObject)
    {
        NSArray *array = (NSArray*) responseObject[@"points"];
        if ( ! array) {
            failure();
        }
        
        NSMutableArray *data = [NSMutableArray array];
        for (NSDictionary* object in array) {
            [data addObject:[[Place alloc] initWithJSONObject:object]];
        }
        
        success(data);
    } failure:^(NSURLSessionDataTask *task, NSError *error) {
        NSLog(@"Something went wrong!");
    }
    ];
    
}

@end
