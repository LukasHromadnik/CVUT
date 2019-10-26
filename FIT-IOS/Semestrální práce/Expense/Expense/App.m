//
//  Application.m
//  Expense
//
//  Created by Lukáš Hromadník on 20.01.15.
//  Copyright (c) 2015 Lukáš Hromadník. All rights reserved.
//

#import "App.h"

@implementation App

+ (NSDateFormatter *)dateFormatter
{
    NSDateFormatter *formatter = [NSDateFormatter new];
    [formatter setDateFormat:@"dd. MM. YYYY"];
    [formatter setLocale:[NSLocale currentLocale]];
    
    return formatter;
}

+ (UIColor *)colorWithRed:(int)red green:(int)green blue:(int)blue
{
    UIColor *color = [[UIColor alloc] initWithRed:red/255.0 green:green/255.0 blue:blue/255.0 alpha:1];
    
    return color;
}

+ (NSNumberFormatter *)numberFormatterWithSymbol:(NSString *)symbol
{
    NSNumberFormatter *formatter = [[NSNumberFormatter alloc] init];
    [formatter setNumberStyle:NSNumberFormatterCurrencyStyle];
    [formatter setLocale:[NSLocale currentLocale]];
    
    if (symbol) {
        [formatter setCurrencySymbol:symbol];
    }
    
    return formatter;
}
@end
