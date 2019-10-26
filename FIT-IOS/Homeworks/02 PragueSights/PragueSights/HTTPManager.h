//
//  HTTPManager.h
//  BI-iOS-2013_06-REST
//
//  Created by Jakub Hladík on 29.10.13.
//  Copyright (c) 2013 Flowknight s.r.o. All rights reserved.
//

#import "AFHTTPSessionManager.h"

@interface HTTPManager : AFHTTPSessionManager

+ (instancetype)sharedManager;


@end
