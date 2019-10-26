//
//  Place.h
//  PragueSights
//
//  Created by Lukáš Hromadník on 04.12.14.
//  Copyright (c) 2014 Lukáš Hromadník. All rights reserved.
//

#import <Foundation/Foundation.h>

@import MapKit;

@interface Place : NSObject <MKAnnotation>

@property (nonatomic, copy) NSString *title;
@property (nonatomic, copy) NSString *subtitle;
@property (nonatomic, assign) CLLocationCoordinate2D coordinate;
@property (nonatomic, strong) NSString *link;

- (instancetype)initWithJSONObject:(id)object;

@end
