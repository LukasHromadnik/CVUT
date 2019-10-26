//
//  MyRecord.h
//  Expense
//
//  Created by Lukáš Hromadník on 21.01.15.
//  Copyright (c) 2015 Lukáš Hromadník. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>
#import "App.h"
#import "MyCategory.h"

@class MyCategory;

@interface MyRecord : NSManagedObject

@property (nonatomic, retain) NSDate * date;
@property (nonatomic, retain) NSString * note;
@property (nonatomic, retain) NSNumber * type;
@property (nonatomic, retain) NSDecimalNumber * value;
@property (nonatomic, retain) MyCategory *category;

- (NSString *)title;
- (NSString *)price;
- (NSString *)priceValue;
- (UIColor *)priceColor;
- (NSString *)formattedDate;
- (void)save;
- (NSString *)categoryName;

@end
