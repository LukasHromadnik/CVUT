//
//  AddItemController.m
//  Expense
//
//  Created by Lukáš Hromadník on 20.01.15.
//  Copyright (c) 2015 Lukáš Hromadník. All rights reserved.
//

#import "AddItemController.h"

@interface AddItemController ()

@end

@implementation AddItemController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.title = @"New";
}

- (void)doneItemAction:(id)sender
{
    if ( ! [self validate]) {
        return;
    }
    
    AppDelegate *appDelegate = [[UIApplication sharedApplication] delegate];
    NSManagedObjectContext *context = [appDelegate managedObjectContext];
    NSManagedObject *object = [NSEntityDescription insertNewObjectForEntityForName:@"MyRecord" inManagedObjectContext:context];
    [object setValue:self.priceCellValue forKey:@"value"];
    [object setValue:[NSNumber numberWithLong:self.typeSwitchValue] forKey:@"type"];
    [object setValue:self.dateCellValue forKey:@"date"];
    [object setValue:self.categoryValue forKey:@"category"];
    [object setValue:self.noteCellValue forKey:@"note"];
    
    NSError *error = nil;
    if ( ! [context save:&error]) {
        NSLog(@"%@%@", error, [error userInfo]);
        return;
    }
    
    [self.categoryValue addRecordsObject:(MyRecord *)object];
    [self.categoryValue save];
    
    [self.navigationController popViewControllerAnimated:YES];
}

@end
