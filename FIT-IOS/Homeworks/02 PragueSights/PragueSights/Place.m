//
//  Place.m
//  PragueSights
//
//  Created by Lukáš Hromadník on 04.12.14.
//  Copyright (c) 2014 Lukáš Hromadník. All rights reserved.
//

#import "Place.h"

@implementation Place

- (instancetype)initWithJSONObject:(id)object
{
    self = [super init];
    
    if (self) {
        self.title = object[@"name"];
        CLLocationCoordinate2D coord;
        coord.latitude = [object[@"latitude"] doubleValue];
        coord.longitude = [object[@"longitude"] doubleValue];
        self.coordinate = coord;
        self.link = object[@"link"];
        self.subtitle = @"Ahoj";
    }
    
    return self;
}
@end
