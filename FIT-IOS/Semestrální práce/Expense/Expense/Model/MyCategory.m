//
//  MyCategory.m
//  Expense
//
//  Created by Lukáš Hromadník on 21.01.15.
//  Copyright (c) 2015 Lukáš Hromadník. All rights reserved.
//

#import "MyCategory.h"
#import "MyRecord.h"


@implementation MyCategory

@dynamic name;
@dynamic records;

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

@end
