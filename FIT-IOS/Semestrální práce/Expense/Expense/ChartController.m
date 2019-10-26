//
//  ViewController.m
//  Expense
//
//  Created by Lukáš Hromadník on 04.12.14.
//  Copyright (c) 2014 Lukáš Hromadník. All rights reserved.
//

#import "ChartController.h"
#import "DataCell.h"
#import "DetailController.h"
#import "Image.h"
#import "AddItemController.h"
#import "AppDelegate.h"
#import "App.h"

@interface ChartController ()

@property (nonatomic, strong) NSMutableArray *data;
@property (nonatomic, strong) NSMutableArray *sections;
@property BOOL needsUpdate;
@property (nonatomic, strong) NSIndexPath *indexPathForUpdate;

@end

@implementation ChartController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.title = @"Expenses";
    self.navigationItem.rightBarButtonItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemAdd target:self action:@selector(addNewItem:)];
    
    self.tableView.tableFooterView = [[UIView alloc] initWithFrame:CGRectZero];
    [self.tableView registerClass:[DataCell class] forCellReuseIdentifier:@"ListCell"];
    
    self.needsUpdate = NO;
    
    [self loadItems];

    // Only for testing
    if ([self.data count] == 0) {
        [self createTestItems];
        [self loadItems];
    }

    if ([self.data count] > 0) {
        [self prepareForView];
    }
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    if (self.needsUpdate) {
        [self loadItems];
        [self prepareForView];
        [self.tableView reloadData];
        self.needsUpdate = NO;
    }
}

- (void)createTestItems
{
    AppDelegate *appDelegate = [[UIApplication sharedApplication] delegate];
    
    NSManagedObjectContext *context = [appDelegate managedObjectContext];
    NSError *error;
    
    NSManagedObject *c1 = [NSEntityDescription insertNewObjectForEntityForName:@"MyCategory" inManagedObjectContext:context];
    [c1 setValue:@"Shopping" forKey:@"name"];
    
    [context save:&error];
    
    NSManagedObject *c2 = [NSEntityDescription insertNewObjectForEntityForName:@"MyCategory" inManagedObjectContext:context];
    [c2 setValue:@"Salary" forKey:@"name"];
    
    [context save:&error];
    
    NSManagedObject *c3 = [NSEntityDescription insertNewObjectForEntityForName:@"MyCategory" inManagedObjectContext:context];
    [c3 setValue:@"Entertainment" forKey:@"name"];
    
    [context save:&error];
    
    NSManagedObject *c4 = [NSEntityDescription insertNewObjectForEntityForName:@"MyCategory" inManagedObjectContext:context];
    [c4 setValue:@"Car" forKey:@"name"];
    
    NSManagedObject *c5 = [NSEntityDescription insertNewObjectForEntityForName:@"MyCategory" inManagedObjectContext:context];
    [c5 setValue:@"Income" forKey:@"name"];
    
    [context save:&error];
    
    NSManagedObject *i1 = [NSEntityDescription insertNewObjectForEntityForName:@"MyRecord" inManagedObjectContext:context];
    [i1 setValue:@41.01 forKey:@"value"];
    [i1 setValue:@"Groceries" forKey:@"note"];
    [i1 setValue:c1 forKey:@"category"];
    NSDateFormatter *formatter = [NSDateFormatter new];
    [formatter setDateFormat:@"dd.MM.yyyy"];
    NSDate *date = [formatter dateFromString:@"14.1.2015"];
    [i1 setValue:date forKey:@"date"];
    [i1 setValue:@0 forKey:@"type"];
    
    [context save:&error];
    
    NSManagedObject *i2 = [NSEntityDescription insertNewObjectForEntityForName:@"MyRecord" inManagedObjectContext:context];
    [i2 setValue:@538.39 forKey:@"value"];
    [i2 setValue:@"January" forKey:@"note"];
    [i2 setValue:c2 forKey:@"category"];
    formatter = [NSDateFormatter new];
    [formatter setDateFormat:@"dd.MM.yyyy"];
    date = [formatter dateFromString:@"14.1.2015"];
    [i2 setValue:date forKey:@"date"];
    [i2 setValue:@1 forKey:@"type"];
    
    [context save:&error];
    
    NSManagedObject *i3 = [NSEntityDescription insertNewObjectForEntityForName:@"MyRecord" inManagedObjectContext:context];
    [i3 setValue:@102.45 forKey:@"value"];
    [i3 setValue:@"Car servis" forKey:@"note"];
    [i3 setValue:c4 forKey:@"category"];
    formatter = [NSDateFormatter new];
    [formatter setDateFormat:@"dd.MM.yyyy"];
    date = [formatter dateFromString:@"15.1.2015"];
    [i3 setValue:date forKey:@"date"];
    [i3 setValue:@0 forKey:@"type"];
    
    [context save:&error];
    
    NSManagedObject *i4 = [NSEntityDescription insertNewObjectForEntityForName:@"MyRecord" inManagedObjectContext:context];
    [i4 setValue:@300.00 forKey:@"value"];
    [i4 setValue:@"Scholarship" forKey:@"note"];
    [i4 setValue:c5 forKey:@"category"];
    formatter = [NSDateFormatter new];
    [formatter setDateFormat:@"dd.MM.yyyy"];
    date = [formatter dateFromString:@"18.1.2015"];
    [i4 setValue:date forKey:@"date"];
    [i4 setValue:@1 forKey:@"type"];
    
    [context save:&error];
    
    NSManagedObject *i5 = [NSEntityDescription insertNewObjectForEntityForName:@"MyRecord" inManagedObjectContext:context];
    [i5 setValue:@21.05 forKey:@"value"];
    [i5 setValue:@"Cinema" forKey:@"note"];
    [i5 setValue:c3 forKey:@"category"];
    formatter = [NSDateFormatter new];
    [formatter setDateFormat:@"dd.MM.yyyy"];
    date = [formatter dateFromString:@"17.1.2015"];
    [i5 setValue:date forKey:@"date"];
    [i5 setValue:@0 forKey:@"type"];
    
    [context save:&error];
    
    NSSet *s1 = [[NSSet alloc] initWithArray:@[i1]];
    [c1 setValue:s1 forKey:@"records"];
    
    NSSet *s2 = [[NSSet alloc] initWithArray:@[i4]];
    [c2 setValue:s2 forKey:@"records"];
    
    NSSet *s3 = [[NSSet alloc] initWithArray:@[i5]];
    [c3 setValue:s3 forKey:@"records"];
    
    NSSet *s4 = [[NSSet alloc] initWithArray:@[i3]];
    [c4 setValue:s4 forKey:@"records"];
    
    NSSet *s5 = [[NSSet alloc] initWithArray:@[i2]];
    [c5 setValue:s5 forKey:@"records"];
    
    [context save:&error];
}

- (void)loadItems
{
    AppDelegate *appDelegate = [[UIApplication sharedApplication] delegate];
    NSManagedObjectContext *context = [appDelegate managedObjectContext];
    NSEntityDescription *entityDesc = [NSEntityDescription entityForName:@"MyRecord" inManagedObjectContext:context];
    
    NSFetchRequest *request = [[NSFetchRequest alloc] init];
    [request setEntity:entityDesc];
    
    NSError *error;
    NSArray *objects = [context executeFetchRequest:request
                                              error:&error];
    
    self.data = [NSMutableArray new];
    for (int i = 0; i < (int)[objects count]; i++) {
        [self.data addObject:objects[i]];
    }
}

- (void)prepareForView
{
    [self.data sortUsingComparator:^NSComparisonResult(id obj1, id obj2) {
        MyRecord *p1 = obj1;
        MyRecord *p2 = obj2;
        
        return [p1.date compare:p2.date];
    }];
    
    self.data = [[[self.data reverseObjectEnumerator] allObjects] mutableCopy];
    
    NSMutableArray *temp = [NSMutableArray new];
    NSMutableArray *section = [NSMutableArray new];
    NSDateFormatter *formatter = [App dateFormatter];

    for (long i = 0; i < [self.data count]; i++) {
        MyRecord *item = self.data[i];

        if (i > 0) {
            MyRecord *prev = self.data[i - 1];
            NSString *prevDate = [formatter stringFromDate:prev.date];
            NSString *currDate = [formatter stringFromDate:item.date];
            
            if ( ! [prevDate isEqualToString:currDate]) {
                [temp addObject:section];
                section = [NSMutableArray new];
            }
        }
        
        [section addObject:item];
        
    }
    [temp addObject:section];
    
    self.data = temp;
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    if ([self.data count] > 0) {
        self.tableView.separatorStyle = UITableViewCellSeparatorStyleSingleLine;
        self.tableView.backgroundView = nil;
        return [self.data count];
    } else {
        UILabel *message = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, self.view.bounds.size.width, self.view.bounds.size.height)];
        message.text = @"No data is currently available. Please insert some!";
        message.textColor = [UIColor blackColor];
        message.numberOfLines = 0;
        message.textAlignment = NSTextAlignmentCenter;
        [message sizeToFit];
        
        self.tableView.backgroundView = message;
        self.tableView.separatorStyle = UITableViewCellSeparatorStyleNone;
    }
    
    return 0;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return [self.data[section] count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    MyRecord *item = self.data[indexPath.section][indexPath.row];

    UITableViewCell *cell = [self.tableView dequeueReusableCellWithIdentifier:@"ListCell"];
    cell.textLabel.text = [item title];
    if ([[item title] isEqualToString:@"No category"]) {
        cell.textLabel.textColor = [[UIColor alloc] initWithRed:0.556863 green:0.556863 blue:0.576471 alpha:1];
    }
    cell.detailTextLabel.text = [item price];
    cell.detailTextLabel.textColor = [item priceColor];
    
    /*
    NSArray *colors = @[[UIColor redColor], [UIColor blueColor], [UIColor greenColor], [UIColor yellowColor]];
    int random = arc4random() % [colors count];
    cell.imageView.image = [Image imageWithColor:[colors objectAtIndex:random]];
    */
    
    return cell;
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
    MyRecord *item = self.data[section][0];
    
    return [item formattedDate];
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    DetailController *controller = [DetailController new];
    controller.record = self.data[indexPath.section][indexPath.row];
    
    [self.navigationController pushViewController:controller animated:YES];
    
    self.needsUpdate = YES;
}

- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
{
    return YES;
}

- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (editingStyle == UITableViewCellEditingStyleDelete) {
        MyRecord *item = self.data[indexPath.section][indexPath.row];
        AppDelegate *appDelegate = [[UIApplication sharedApplication] delegate];
        NSManagedObjectContext *context = [appDelegate managedObjectContext];
        
        [context deleteObject:item];
        
        [item.category removeRecordsObject:item];
        
        NSError *error = nil;
        if ( ! [context save:&error]) {
            NSLog(@"%@%@", error, [error userInfo]);
            return;
        }
        
        [self.data[indexPath.section] removeObjectAtIndex:indexPath.row];
        [self.tableView beginUpdates];
        if ([self.data[indexPath.section] count] == 0) {
            [tableView deleteSections:[NSIndexSet indexSetWithIndex:indexPath.section] withRowAnimation:UITableViewRowAnimationFade];
            [self.data removeObjectAtIndex:indexPath.section];
        }
        [tableView deleteRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationFade];
        [self.tableView endUpdates];
    }
}

- (void)addNewItem:(id)sender
{
    AddItemController *controller = [AddItemController new];
    [self.navigationController pushViewController:controller animated:YES];
    self.needsUpdate = YES;
}

@end
