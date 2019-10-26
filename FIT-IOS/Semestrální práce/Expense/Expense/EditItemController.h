//
//  EditItemController.h
//  Expense
//
//  Created by Lukáš Hromadník on 19.01.15.
//  Copyright (c) 2015 Lukáš Hromadník. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "CategoryController.h"
#import "Model/MyCategory.h"
#import "Model/MyRecord.h"

@class EditItemController;

@interface EditItemController : UITableViewController <UITableViewDataSource, UITableViewDelegate, UITextFieldDelegate, CategoryControllerDelegate>
@property (nonatomic, strong) MyRecord *record;
@property (nonatomic, strong) UITableViewCell *typeCell;
@property (nonatomic, strong) UISegmentedControl *typeSwitch;
@property (nonatomic) NSInteger typeSwitchValue;
@property (nonatomic, strong) UITableViewCell *priceCell;
@property (nonatomic, strong) UITextField *priceCellField;
@property (nonatomic, strong) NSDecimalNumber *priceCellValue;
@property (nonatomic, strong) UITableViewCell *categoryCell;
@property (nonatomic, strong) MyCategory *categoryValue;
@property (nonatomic, strong) UITableViewCell *dateCell;
@property (nonatomic, strong) UITextField *dateCellField;
@property (nonatomic, strong) NSDate *dateCellValue;
@property (nonatomic, strong) UITableViewCell *noteCell;
@property (nonatomic, strong) NSString *noteCellValue;

- (BOOL)validate;
@end
