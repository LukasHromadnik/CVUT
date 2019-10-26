//
//  Image.m
//  Expense
//
//  Created by Lukáš Hromadník on 14.01.15.
//  Copyright (c) 2015 Lukáš Hromadník. All rights reserved.
//

#import "Image.h"

@implementation Image

+ (UIImage *)imageWithColor:(UIColor *)color
{
    CGRect rect = CGRectMake(0, 0, 1, 1);
    UIGraphicsBeginImageContextWithOptions(rect.size, NO, 0);
    [color setFill];
    UIRectFill(rect);
    UIImage *image = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    return image;
}

@end
