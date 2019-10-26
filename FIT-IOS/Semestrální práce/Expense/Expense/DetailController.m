//
//  DetailController.m
//  Expense
//
//  Created by Lukáš Hromadník on 04.12.14.
//  Copyright (c) 2014 Lukáš Hromadník. All rights reserved.
//

#import "DetailController.h"
#import "App.h"

@interface DetailController ()

@property (nonatomic, strong) UITableViewCell *priceCell;
@property (nonatomic, strong) UITableViewCell *categotyCell;
@property (nonatomic, strong) UITableViewCell *dateCell;
@property (nonatomic, strong) UITableViewCell *noteCell;

@end

@implementation DetailController

#pragma mark - Initialization

- (id)init
{
    self = [super initWithStyle:UITableViewStyleGrouped];
    
    return self;
}

#pragma mark - Controller lifecycle

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    // Navigation controller
    self.navigationItem.rightBarButtonItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemEdit target:self action:@selector(editItem)];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    [self.tableView reloadData];
    self.title = [self.record title];
}

#pragma mark - TableView Delegate

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 2;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    switch (section) {
        case 0: return 3;
        case 1: return 1;
    }

    return 0;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    long section = indexPath.section;
    long row = indexPath.row;
    
    switch (section) {
        case 0:
            switch (row) {
                case 0: return [self loadPriceCell];
                case 1: return [self loadCategoryCell];
                case 2: return [self loadDateCell];
            }
        case 1:
            switch (row) {
                case 0: return [self loadNoteCell];
            }
    }
    
    return nil;
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
    if (section == 1) {
        return @"Note";
    }
    
    return nil;
}

- (void)editItem
{
    EditItemController *controller = [EditItemController new];
    controller.record = self.record;
    
    [self.navigationController pushViewController:controller animated:YES];
}

- (UITableViewCell *)loadPriceCell
{
    UITableViewCell *priceCell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleValue1 reuseIdentifier:@"DetailCell"];
    priceCell.textLabel.text = @"Price";
    priceCell.detailTextLabel.text = [self.record price];
    priceCell.detailTextLabel.textColor = [self.record priceColor];
    self.priceCell = priceCell;
    
    return priceCell;
}

- (UITableViewCell *)loadCategoryCell
{
    UITableViewCell *categoryCell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleValue1 reuseIdentifier:@"DetailCell"];
    categoryCell.textLabel.text = @"Category";
    categoryCell.detailTextLabel.text = [self.record categoryName];
    self.categotyCell = categoryCell;
    
    return categoryCell;
}

- (UITableViewCell *)loadDateCell
{
    UITableViewCell *dateCell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleValue1 reuseIdentifier:@"DetailCell"];
    dateCell.textLabel.text = @"Date";
    dateCell.detailTextLabel.text = [self.record formattedDate];
    self.dateCell = dateCell;
    
    return dateCell;
}

- (UITableViewCell *)loadNoteCell
{
    UITableViewCell *noteCell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:nil];
    NSString *note = self.record.note;
    if ([note length] == 0) {
        UIColor *greyText = [UIColor colorWithRed:0.556863 green:0.556863 blue:0.576471 alpha:1];
        note = @"Note";
        noteCell.textLabel.textColor = greyText;
    }
    noteCell.textLabel.text = note;
    self.noteCell = noteCell;
    
    return noteCell;
}
@end
