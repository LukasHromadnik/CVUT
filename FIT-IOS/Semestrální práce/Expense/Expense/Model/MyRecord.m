//
//  MyRecord.m
//  Expense
//
//  Created by Lukáš Hromadník on 21.01.15.
//  Copyright (c) 2015 Lukáš Hromadník. All rights reserved.
//

#import "MyRecord.h"

@implementation MyRecord

@dynamic date;
@dynamic note;
@dynamic type;
@dynamic value;
@dynamic category;

- (NSString *)title
{
    return [self categoryName];
}

- (NSString *)price
{
    return [self preparePriceWithCurrencySymbol:nil];
}

- (NSString *)priceValue
{
    return [self preparePriceWithCurrencySymbol:@""];
}

- (NSString *)preparePriceWithCurrencySymbol:(NSString *)symbol
{
    NSNumberFormatter *formatter = [App numberFormatterWithSymbol:symbol];
    
    NSDecimalNumber *price = self.value;
    
    if ([self.type isEqual:@0]) {
        NSDecimalNumber *negative = [[NSDecimalNumber alloc] initWithString:@"-1"];
        price = [price decimalNumberByMultiplyingBy:negative];
    }
    
    return [formatter stringFromNumber:price];
}

- (UIColor *)priceColor
{
    UIColor *red = [UIColor redColor];
    UIColor *green = [App colorWithRed:0 green:200 blue:0];
    
    return ([self.type isEqual:@0]) ? red : green;
}

- (NSString *)formattedDate
{
    NSDateFormatter *dateFormatter = [App dateFormatter];
    
    return [dateFormatter stringFromDate:self.date];
}

- (void)save
{
    NSError *error = nil;
    NSManagedObjectContext *managedObjectContext = [self managedObjectContext];
    
    if (managedObjectContext) {
        if ([managedObjectContext hasChanges] && ! [managedObjectContext save:&error]) {
            NSLog(@"Unresolved error: %@, %@", error, [error userInfo]);
            abort();
        }
    }
}

- (NSString*)categoryName
{
    NSString *name = @"No category";
    
    if ([self.category managedObjectContext] != nil && ! [self.category isDeleted]) {
        name = self.category.name;
    }
    
    return name;
}

@end
