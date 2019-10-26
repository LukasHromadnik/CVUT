//
//  AddItemController.m
//  Expense
//
//  Created by Lukáš Hromadník on 14.01.15.
//  Copyright (c) 2015 Lukáš Hromadník. All rights reserved.
//

#import "EditItemController.h"
#import "App.h"

@interface EditItemController ()
@property BOOL activeTextField;
@end

@implementation EditItemController

#pragma mark - Initialization

- (id)init
{
    self = [super initWithStyle:UITableViewStyleGrouped];
    
    if (self) {
        self.title = @"Edit";
        self.activeTextField = NO;
    }

    return self;
}

#pragma mark - Controller lifecycle

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    // Navigation controller
    self.navigationItem.rightBarButtonItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone
                                                                                           target:self
                                                                                           action:@selector(doneItemAction:)];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    [self.tableView reloadData];
}

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
    
    [self.priceCellField becomeFirstResponder];
}

#pragma mark - UITableViewDelegate

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 2;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    switch (section) {
        case 0: return 4;
        case 1: return 1;
    }
    
    return 0;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    switch (indexPath.section) {
        case 0:
            switch (indexPath.row) {
                case 0: return [self generateTypeCell];
                case 1: return [self generatePriceCell];
                case 2: return [self generateCategoryCell];
                case 3: return [self generateDateCell];
            }
        case 1:
            switch (indexPath.row) {
                case 0: return [self generateNoteCell];
            }
    }
    
    return nil;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = [self.tableView cellForRowAtIndexPath:indexPath];
    if ([cell isEqual:self.categoryCell]) {
        CategoryController *categories = [CategoryController new];
        categories.delegate = self;
        categories.selectedCategory = self.categoryValue;
        [self.navigationController pushViewController:categories animated:YES];
    }
}

- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
    if (section == 1) {
        return @"Note";
    }
    
    return nil;
}

#pragma mark - UITextFieldDelegate

- (void)textFieldDidBeginEditing:(UITextField *)textField
{
    self.activeTextField = YES;
    NSNumberFormatter *formatter = [App numberFormatterWithSymbol:@""];
    formatter.usesGroupingSeparator = NO;
    self.priceCellField.text = [formatter stringFromNumber:self.priceCellValue];
}

- (void)textFieldDidEndEditing:(UITextField *)textField
{
    self.activeTextField = NO;
    if ([textField.text length] > 0) {
        self.priceCellValue = [[NSDecimalNumber alloc] initWithString:textField.text];
    } else {
        self.priceCellValue = nil;
    }
    [self typeChanged];
}

#pragma mark - CategoryControllerDelegate

- (void)categoryController:(CategoryController *)controller choosedCategory:(MyCategory *)category
{
    self.categoryCell.detailTextLabel.text = category.name;
    self.categoryValue = category;
}

#pragma mark - Custom methods

- (NSArray *)loadItemOptions
{
    return @[@"Outcome", @"Income"];
}

- (void)initFormValues
{
    self.priceCellValue = [[NSDecimalNumber alloc] initWithString:@"0"];
}

- (BOOL)validate
{
    NSString *title = @"Error";
    NSString *message = nil;
    
    if ( ! self.priceCellValue) {
        message = @"Price must not be empty";
    } else if ([self.priceCellValue integerValue] == 0) {
        message = @"Price must be greater than 0";
    } else if ( ! self.categoryValue) {
        message = @"You have to choose Category";
    }
    
    if (message) {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:title message:message delegate:self cancelButtonTitle:@"OK" otherButtonTitles:nil];
        [alert show];
        
        return NO;
    }
    
    return YES;
}

#pragma mark - Custom events

- (void)doneItemAction:(id)sender
{
    [self.view endEditing:YES];
    
    if ( ! [self validate]) {
        [self.priceCellField becomeFirstResponder];
        return;
    }
    self.record.value = self.priceCellValue;
    self.record.type = [[NSNumber alloc] initWithLong:self.typeSwitchValue];
    self.record.date = self.dateCellValue;
    self.record.note = self.noteCellValue;
    
    if ( ! [self.record.category isEqual:self.categoryValue]) {
        [self.record.category removeRecordsObject:self.record];
        [self.categoryValue addRecordsObject:self.record];
    }
    self.record.category = self.categoryValue;

    [self.record save];

    [self.navigationController popViewControllerAnimated:YES];
}

- (void)typeChanged
{
    NSDecimalNumber *price = self.priceCellValue;
    NSDecimalNumber *negative = [[NSDecimalNumber alloc] initWithString:@"-1"];
    NSInteger selectedIndex = self.typeSwitch.selectedSegmentIndex;
    
    if (self.activeTextField || [self.priceCellField.text length] == 0) {
        return;
    }
    
    if ((selectedIndex == 0 && price > 0) || (selectedIndex == 1 && price < 0)) {
        price = [price decimalNumberByMultiplyingBy:negative];
    }
    
    NSNumberFormatter *formatter = [App numberFormatterWithSymbol:nil];
    self.priceCellField.text = [formatter stringFromNumber:price];
    self.typeSwitchValue = selectedIndex;
}

- (void)datePickerChanged:(id)sender
{
    UIDatePicker *picker = (UIDatePicker *)sender;
    
    self.dateCellField.text = [[App dateFormatter] stringFromDate:picker.date];
    self.dateCellValue = [picker date];
}

- (void)noteChanged:(id)sender
{
    UITextField *field = (UITextField *)sender;
    self.noteCellValue = field.text;
}

- (void)textFieldDidEditingChanged:(id)sender
{
    UITextField *textField = (UITextField *)sender;
    if ([textField.text length] > 0) {
        self.priceCellValue = [[NSDecimalNumber alloc] initWithString:textField.text];
    }
}


#pragma mark - Cell generating

- (UITableViewCell *)generateTypeCell
{
    // Initialization
    UITableViewCell *typeCell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleValue1 reuseIdentifier:@"ItemDetailCell"];
    
    // Set label
    typeCell.textLabel.text = @"Type";
    
    // Set options
    NSArray *options = [self loadItemOptions];
    
    // Init UISegmentedControl for options
    UISegmentedControl *typeSwitch = [[UISegmentedControl alloc] initWithItems:options];
    
    // Set active option
    NSString *selectedObject = @"Outcome";
    if (self.record) {
        selectedObject = [options objectAtIndex:[self.record.type integerValue]];
        self.typeSwitchValue = [self.record.type integerValue];
    }
    typeSwitch.selectedSegmentIndex = [options indexOfObject:selectedObject];
    
    // Set event when value changed
    [typeSwitch addTarget:self action:@selector(typeChanged) forControlEvents:UIControlEventValueChanged];
    
    // Add UISegmentedControl as accessoryView
    [typeCell setAccessoryView:typeSwitch];
    
    // Set variables as instance variables
    self.typeCell = typeCell;
    self.typeSwitch = typeSwitch;
    
    return typeCell;
}

- (UITableViewCell *)generatePriceCell
{
    // Initialization
    UITableViewCell *priceCell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleValue1 reuseIdentifier:@"ItemDetailCell"];
    
    // Set label
    priceCell.textLabel.text = @"Price";
    
    // Initialization of UITextField
    UITextField *priceCellField = [[UITextField alloc] initWithFrame:CGRectMake(0, 0, 150, 30)];
    
    // Set value changed event
    [priceCellField addTarget:self action:@selector(textFieldDidEditingChanged:) forControlEvents:UIControlEventEditingChanged];
    
    // Set placeholder and textAlignment
    NSNumberFormatter *formatter = [App numberFormatterWithSymbol:nil];
    NSDecimalNumber *placeholder = [[NSDecimalNumber alloc] initWithString:@"0"];
    priceCellField.placeholder = [formatter stringFromNumber:placeholder];
    priceCellField.textAlignment = NSTextAlignmentRight;
    priceCellField.keyboardType = UIKeyboardTypeDecimalPad;

    // Set value if provided
    if (self.record) {
        priceCellField.text = [self.record price];
        self.priceCellValue = self.record.value;
    }
    
    // Set the delegator
    priceCellField.delegate = self;
    
    // Add UITextField as accessoryView
    [priceCell setAccessoryView:priceCellField];
    
    // Set variables as instance variables
    self.priceCell = priceCell;
    self.priceCellField = priceCellField;
    
    return priceCell;
}

- (UITableViewCell *)generateCategoryCell
{
    // Initialization
    UITableViewCell *categoryCell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleValue1 reuseIdentifier:@"ItemDetailCell"];
    
    // Add accessory type for better user expierence
    [categoryCell setAccessoryType:UITableViewCellAccessoryDisclosureIndicator];
    
    // Set label
    categoryCell.textLabel.text = @"Category";
    
    // Set active category if provided
    NSString *detailTextLabel = @"Choose...";
    if (self.record && ! self.categoryValue) {
        self.categoryValue = self.record.category;
    }
    if (self.categoryValue.managedObjectContext == nil || self.categoryValue.isDeleted) {
        self.categoryValue = nil;
    } else {
        detailTextLabel = self.categoryValue.name;
    }
    categoryCell.detailTextLabel.text = detailTextLabel;
    
    // Set variables as instance variables
    self.categoryCell = categoryCell;
    
    return categoryCell;
}

- (UITableViewCell *)generateDateCell
{
    // Initialization
    UITableViewCell *dateCell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleValue1 reuseIdentifier:@"ItemDetailCell"];
    
    // Set label
    dateCell.textLabel.text = @"Date";
    
    // Init UITextField for the date
    UITextField *date = [[UITextField alloc] initWithFrame:CGRectMake(0, 0, 150, 30)];
    
    // Set active date if provided
    NSDate *currentDate = (self.record) ? self.record.date : [NSDate date];
    self.dateCellValue = currentDate;
    date.text = [[App dateFormatter] stringFromDate:currentDate];
    
    // Align text in the cell to the right
    date.textAlignment = NSTextAlignmentRight;
    
    // Init datePicker
    UIDatePicker *datePicker = [[UIDatePicker alloc] init];
    
    // Set datePicker background color
    datePicker.backgroundColor = [UIColor whiteColor];
    
    // Set datePicker to pick only date
    [datePicker setDatePickerMode:UIDatePickerModeDate];
    
    // Add event when value changed
    [datePicker addTarget:self action:@selector(datePickerChanged:) forControlEvents:UIControlEventValueChanged];
    [datePicker setDate:currentDate];
    [datePicker setLocale:[NSLocale currentLocale]];
    
    // Add datePicker as input for textField
    [date setInputView:datePicker];
    
    // Add textField as accessoryView
    [dateCell setAccessoryView:date];
    
    // Set variables as instance variables
    self.dateCellField = date;
    self.dateCell = dateCell;
    
    return dateCell;
}

- (UITableViewCell *)generateNoteCell
{
    // Initialization
    UITableViewCell *noteCell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:nil];
    
    // Init UITextField
    UITextField *note = [[UITextField alloc] initWithFrame:CGRectMake(16, 0, noteCell.bounds.size.width + 32, noteCell.bounds.size.height)];
    
    // Set placeholder and some other things
    note.placeholder = @"Note";
    note.keyboardType = UIKeyboardTypeDefault;
    note.keyboardAppearance = UIKeyboardAppearanceDefault;
    
    [note addTarget:self action:@selector(noteChanged:) forControlEvents:UIControlEventEditingChanged];
    
    // Set active note if provided
    if (self.record) {
        note.text = self.record.note;
        self.noteCellValue = note.text;
    }
    
    // Add textField as contentView
    [noteCell.contentView addSubview:note];
    
    // Set variable as instance variable
    self.noteCell = noteCell;
    
    return noteCell;
}

@end
