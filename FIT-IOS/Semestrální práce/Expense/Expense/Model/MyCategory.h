//
//  MyCategory.h
//  Expense
//
//  Created by Lukáš Hromadník on 21.01.15.
//  Copyright (c) 2015 Lukáš Hromadník. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>

@class MyRecord;

@interface MyCategory : NSManagedObject

@property (nonatomic, retain) NSString * name;
@property (nonatomic, retain) NSSet *records;
@end

@interface MyCategory (CoreDataGeneratedAccessors)

- (void)addRecordsObject:(MyRecord *)value;
- (void)removeRecordsObject:(MyRecord *)value;
- (void)addRecords:(NSSet *)values;
- (void)removeRecords:(NSSet *)values;
- (void)save;

@end
