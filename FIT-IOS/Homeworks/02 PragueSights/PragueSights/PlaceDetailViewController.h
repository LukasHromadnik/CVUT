//
//  PlaceDetailViewController.h
//  PragueSights
//
//  Created by Lukáš Hromadník on 05.12.14.
//  Copyright (c) 2014 Lukáš Hromadník. All rights reserved.
//

#import "ViewController.h"

@interface PlaceDetailViewController : UIViewController <UIWebViewDelegate>

- (instancetype)initWithURLString:(NSString *)url;

@end
