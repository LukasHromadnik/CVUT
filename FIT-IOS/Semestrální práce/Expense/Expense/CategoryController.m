//
//  CategoryController.m
//  Expense
//
//  Created by Lukáš Hromadník on 19.01.15.
//  Copyright (c) 2015 Lukáš Hromadník. All rights reserved.
//

#import "CategoryController.h"
#import "AddNewCategoryController.h"
#import "AppDelegate.h"

@interface CategoryController ()
@property (nonatomic, strong) NSMutableArray *data;
@property (nonatomic, strong) UITableViewCell *addNew;
@end

@implementation CategoryController

- (id)init
{
    self = [super initWithStyle:UITableViewStyleGrouped];
    
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.title = @"Categories";
//    self.navigationItem.rightBarButtonItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemAdd target:self action:@selector(addNewCategory)];
    
    self.tableView.tableFooterView = [[UIView alloc] initWithFrame:CGRectZero];
    
    UITableViewCell *addNew = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:nil];
    addNew.textLabel.text = @"Add new category...";
    self.addNew = addNew;
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [self loadCategories];
    [self.tableView reloadData];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    switch (indexPath.section) {
        case 0: {
            MyCategory *item = self.data[indexPath.row];
            UITableViewCell *cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"CategoryCell"];
            cell.textLabel.text = item.name;
            if ([item isEqual:self.selectedCategory]) {
                [cell setAccessoryType:UITableViewCellAccessoryCheckmark];
            }
            return cell;
        }
        case 1: return self.addNew;
    }
    
    return nil;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.section == 1) {
        [self addNewCategory];
    } else {
        if ([self.delegate respondsToSelector:@selector(categoryController:choosedCategory:)]) {
            [self.delegate categoryController:self choosedCategory:self.data[indexPath.row]];
        }
        [self.navigationController popViewControllerAnimated:YES];
    }
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 2;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    switch (section) {
        case 0: return [self.data count];
        case 1: return 1;
    }
    
    return 0;
}

- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
{
    return YES;
}

- (UITableViewCellEditingStyle)tableView:(UITableView *)tableView editingStyleForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = [self.tableView cellForRowAtIndexPath:indexPath];

    if (cell.accessoryType == UITableViewCellAccessoryCheckmark) {
        return UITableViewCellEditingStyleNone;
    }
    
    return UITableViewCellEditingStyleDelete;
}

- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (editingStyle == UITableViewCellEditingStyleDelete) {
        MyCategory *item = self.data[indexPath.row];
        AppDelegate *appDelegate = [[UIApplication sharedApplication] delegate];
        NSManagedObjectContext *context = [appDelegate managedObjectContext];
        
        [context deleteObject:item];
        for (MyRecord *record in item.records) {
            record.category = nil;
        }
        
        NSError *error = nil;
        if ( ! [context save:&error]) {
            NSLog(@"%@%@", error, [error userInfo]);
            return;
        }
        
        [self.data removeObjectAtIndex:indexPath.row];
        [self.tableView beginUpdates];
        if ([self.data count] == 0) {
            [self.tableView deleteSections:[NSIndexSet indexSetWithIndex:indexPath.section] withRowAnimation:UITableViewRowAnimationFade];
            self.data = nil;
        }
        [self.tableView deleteRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationFade];
        [self.tableView endUpdates];
    }
}

- (void)addNewCategory
{
    AddNewCategoryController *controller = [AddNewCategoryController new];
    [self.navigationController pushViewController:controller animated:YES];
}

- (void)loadCategories
{
    AppDelegate *appDelegate = [[UIApplication sharedApplication] delegate];
    NSManagedObjectContext *context = [appDelegate managedObjectContext];
    NSEntityDescription *entity = [NSEntityDescription entityForName:@"MyCategory" inManagedObjectContext:context];
    NSFetchRequest *request = [NSFetchRequest new];
    [request setEntity:entity];
    
    NSError *error;
    
    NSArray *result = [context executeFetchRequest:request error:&error];
    
    self.data = [NSMutableArray new];
    for (int i = 0; i < (int)[result count]; i++) {
        [self.data addObject:result[i]];
    }
}
@end
