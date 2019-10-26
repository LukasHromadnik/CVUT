//
//  DetailController.h
//  Expense
//
//  Created by Lukáš Hromadník on 04.12.14.
//  Copyright (c) 2014 Lukáš Hromadník. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Model/MyRecord.h"
#import "Model/MyCategory.h"
#import "EditItemController.h"

@interface DetailController : UITableViewController <UITableViewDelegate, UITableViewDataSource>
@property (nonatomic, strong) MyRecord *record;
@end
