//
//  DataCell.m
//  Expense
//
//  Created by Lukáš Hromadník on 04.12.14.
//  Copyright (c) 2014 Lukáš Hromadník. All rights reserved.
//

#import "DataCell.h"

@interface DataCell ()
@property (nonatomic, strong) UIColor *defaultTextColor;
@end

@implementation DataCell

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:UITableViewCellStyleValue1 reuseIdentifier:reuseIdentifier];
    
    if (self) {
        self.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
        self.defaultTextColor = self.textLabel.textColor;
    }
    
    return self;
}

- (void)layoutSubviews
{
    [super layoutSubviews];

    /*
    int padding = 2;
    int imageWidth = 3;
    int imageHeight = self.frame.size.height - (2 * padding);
    self.imageView.frame = CGRectMake(0, padding, imageWidth, imageHeight);
     */
}

- (void)prepareForReuse
{
    self.textLabel.textColor = self.defaultTextColor;
}

@end
