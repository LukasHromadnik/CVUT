//
//  CategoryController.h
//  Expense
//
//  Created by Lukáš Hromadník on 19.01.15.
//  Copyright (c) 2015 Lukáš Hromadník. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Model/MyCategory.h"
#import "Model/MyRecord.h"

@class CategoryController;

@protocol CategoryControllerDelegate <NSObject>
- (void)categoryController:(CategoryController *)controller choosedCategory:(MyCategory *)category;
@end

@interface CategoryController : UITableViewController <UITableViewDelegate, UITableViewDataSource>
@property (nonatomic, weak) id <CategoryControllerDelegate> delegate;
@property (nonatomic, strong) MyCategory *selectedCategory;
@end
