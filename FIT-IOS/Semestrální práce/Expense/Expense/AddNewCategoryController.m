//
//  AddNewCategoryController.m
//  Expense
//
//  Created by Lukáš Hromadník on 20.01.15.
//  Copyright (c) 2015 Lukáš Hromadník. All rights reserved.
//

#import "AddNewCategoryController.h"

@interface AddNewCategoryController ()
@property (nonatomic, strong) UITableViewCell *nameCell;
@property (nonatomic, strong) UITextField *nameField;
@end

@implementation AddNewCategoryController

- (id)init
{
    self = [super initWithStyle:UITableViewStyleGrouped];
    
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.title = @"Add category";
    
    self.navigationItem.rightBarButtonItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector(addNewCategoryAction:)];
    
    UITableViewCell *nameCell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:nil];
    UITextField *nameField = [[UITextField alloc] initWithFrame:CGRectMake(16, 0, nameCell.contentView.bounds.size.width + 32, nameCell.contentView.bounds.size.height)];
    nameField.placeholder = @"Name";
    [nameCell.contentView addSubview:nameField];
    self.nameCell = nameCell;
    self.nameField = nameField;
}

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
    
    [self.nameField becomeFirstResponder];
}

#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return 1;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    switch (indexPath.row) {
        case 0: return self.nameCell;
    }
    
    return nil;
}

- (void)addNewCategoryAction:(id)sender
{
    if ([self.nameField.text length] > 0) {
        AppDelegate *appDelegate = [[UIApplication sharedApplication] delegate];
        NSManagedObjectContext *context = [appDelegate managedObjectContext];
        NSManagedObject *object = [NSEntityDescription insertNewObjectForEntityForName:@"MyCategory" inManagedObjectContext:context];
        [object setValue:self.nameField.text forKey:@"name"];
        
        NSError *error;
        [context save:&error];
    }
    
    [self.navigationController popViewControllerAnimated:YES];
}

@end
