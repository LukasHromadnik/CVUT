//
//  Application.h
//  Expense
//
//  Created by Lukáš Hromadník on 20.01.15.
//  Copyright (c) 2015 Lukáš Hromadník. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface App : UIApplication

+ (NSDateFormatter *)dateFormatter;
+ (UIColor *)colorWithRed:(int)red green:(int)green blue:(int)blue;
+ (NSNumberFormatter *)numberFormatterWithSymbol:(NSString *)symbol;

@end
