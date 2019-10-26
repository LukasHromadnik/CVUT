//
//  ViewController.m
//  PragueSights
//
//  Created by Lukáš Hromadník on 04.12.14.
//  Copyright (c) 2014 Lukáš Hromadník. All rights reserved.
//

#import "ViewController.h"
#import "APIWrapper.h"
#import "Place.h"
#import "SVProgressHUD.h"
#import <Masonry.h>
#import <QuartzCore/QuartzCore.h>
#import "PlaceDetailViewController.h"
#import "SVWebViewController.h"

@interface ViewController ()
@property (nonatomic, strong) MKMapView *mapView;
@property (nonatomic, strong) CLLocationManager *manager;
@property (nonatomic, strong) MKPointAnnotation *anotation;
@property (nonatomic) BOOL centerMap;
@property (nonatomic) BOOL firstInit;
@end

@implementation ViewController

#pragma mark - View lifecycle

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.centerMap = YES;
    self.firstInit = NO;
    
    self.title = @"PragueSights";
    
    [self setBarButtons];

    [self initMap];
    
    [self setMapCenterButton];
    
    UIPanGestureRecognizer *pan = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(didDragMap:)];
    [pan setDelegate:self];
    [self.mapView addGestureRecognizer:pan];
    
    [self networkOn];
    
    int delay = 4;
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, delay * NSEC_PER_SEC), dispatch_get_main_queue(), ^{
        [self loadPlaces];
    });
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    [self.navigationController setToolbarHidden:YES animated:NO];
}

#pragma mark - Custom methods

- (void)setBarButtons
{
    self.navigationItem.rightBarButtonItem =
    [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemRefresh
                                                  target:self
                                                  action:@selector(refreshPlaces)];
    
    self.navigationItem.backBarButtonItem =
    [[UIBarButtonItem alloc] initWithTitle:@"Zpět"
                                     style:UIBarButtonItemStylePlain
                                    target:nil
                                    action:nil];

}

- (void)didDragMap:(UIGestureRecognizer *)gestureRecognizer
{
    if (gestureRecognizer.state == UIGestureRecognizerStateBegan) {
        self.centerMap = NO;
    }
}

- (void)initMap
{
    self.manager = [CLLocationManager new];
    self.manager.delegate = self;
    [self.manager requestWhenInUseAuthorization];
    [self.manager requestAlwaysAuthorization];
    
    self.manager.distanceFilter = kCLDistanceFilterNone;
    self.manager.desiredAccuracy = kCLLocationAccuracyBest;
    [self.manager startUpdatingLocation];
    
    self.mapView = [[MKMapView alloc] initWithFrame:self.view.bounds];
    self.mapView.showsUserLocation = YES;
//    [self.mapView setUserTrackingMode:MKUserTrackingModeNone];
    self.mapView.mapType = MKMapTypeStandard;
    self.mapView.delegate = self;
    [self.view addSubview:self.mapView];
}

- (void)setMapCenterButton
{
    UIButton *centerButton = [[UIButton alloc] initWithFrame:CGRectZero];
    centerButton.backgroundColor = [UIColor whiteColor];
    [[centerButton layer] setBorderWidth:1.0f];
    [[centerButton layer] setBorderColor:[UIColor blackColor].CGColor];
    UIImage *image = [UIImage imageNamed:@"CenterMap"];
    [centerButton setImage:image forState:UIControlStateNormal];
    [self.view addSubview:centerButton];
    [centerButton mas_makeConstraints:^(MASConstraintMaker *make) {
        make.bottom.equalTo(@-10);
        make.right.equalTo(@-10);
        make.width.equalTo(@40);
        make.height.equalTo(@40);
    }];
    [centerButton addTarget:self
                     action:@selector(centerMapToUserLocation)
           forControlEvents:UIControlEventTouchUpInside];
}

- (void)centerMapToUserLocation
{
    [self.mapView setCenterCoordinate:self.mapView.userLocation.location.coordinate animated:YES];
    self.centerMap = YES;
}

- (void)loadPlaces
{
    [APIWrapper placesSuccess:^(NSArray *places) {
        [self.mapView removeAnnotations:self.mapView.annotations];
        [self.mapView addAnnotations:places];
        [self networkOff];
    } failure:^(void) {
        NSLog(@"Something went wrong!");
    }];
}

- (void)networkOn
{
    [SVProgressHUD show];
    [[UIApplication sharedApplication] setNetworkActivityIndicatorVisible:YES];
}

- (void)networkOff
{
    [SVProgressHUD dismiss];
    [[UIApplication sharedApplication] setNetworkActivityIndicatorVisible:NO];
}

- (void)refreshPlaces
{
    [self networkOn];
//    [self.mapView removeAnnotations:self.mapView.annotations];
    
    [self loadPlaces];
}

#pragma mark - MapViewDelegate

- (void)mapView:(MKMapView *)mapView didUpdateUserLocation:(MKUserLocation *)userLocation
{
    if (self.centerMap) {
        MKCoordinateRegion region;
        MKCoordinateSpan span;
        if ( ! self.firstInit) {
            span.latitudeDelta = 0.01;
            span.longitudeDelta = 0.01;
            self.firstInit = YES;
        } else {
            span = mapView.region.span;
        }
        CLLocationCoordinate2D location;
        location.latitude = self.mapView.userLocation.coordinate.latitude;
        location.longitude = self.mapView.userLocation.coordinate.longitude;
        region.span = span;
        region.center = location;
        [mapView setRegion:region animated:YES];
    }
}

- (MKAnnotationView *)mapView:(MKMapView *)mapView viewForAnnotation:(id<MKAnnotation>)annotation
{
    MKPinAnnotationView *pin = (MKPinAnnotationView *) [self.mapView dequeueReusableAnnotationViewWithIdentifier:@"myPin"];
    if([annotation isKindOfClass:[MKUserLocation class]])
        return nil;
    
    if (pin == nil) {
        pin = [[MKPinAnnotationView alloc] initWithAnnotation:annotation reuseIdentifier:@"myPin"];
    } else {
        pin.annotation = annotation;
    }
    
    UIButton *rightButton = [UIButton buttonWithType:UIButtonTypeDetailDisclosure];
    [rightButton setTitle:annotation.title forState:UIControlStateNormal];
    pin.rightCalloutAccessoryView = rightButton;
    
    pin.animatesDrop = NO;
    pin.canShowCallout = YES;
    
    return pin;
}

- (void)mapView:(MKMapView *)mapView didSelectAnnotationView:(MKAnnotationView *)view
{
    if ([view.annotation isKindOfClass:[Place class]]) {
        Place *place = (Place *)view.annotation;
        CLLocation *pinLocation = [[CLLocation alloc] initWithLatitude:place.coordinate.latitude longitude:place.coordinate.longitude];
        CLLocationDistance distance = [pinLocation distanceFromLocation:self.mapView.userLocation.location];
        
        if (distance < 1000) {
            place.subtitle = [NSString stringWithFormat:@"%.0f m", distance];
        } else {
            place.subtitle = [NSString stringWithFormat:@"%.1f km", distance / 1000];
        }
    }
}

- (void)mapView:(MKMapView *)mapView annotationView:(MKAnnotationView *)view calloutAccessoryControlTapped:(UIControl *)control
{
    if ([view.annotation isKindOfClass:[Place class]]) {
        Place *place = (Place *)view.annotation;
        SVWebViewController *webView = [[SVWebViewController alloc] initWithAddress:place.link];
        webView.title = place.title;
        [self.navigationController pushViewController:webView animated:YES];
//        PlaceDetailViewController *detail = [[PlaceDetailViewController alloc] initWithURLString:place.link];
//        [self.navigationController pushViewController:detail animated:YES];
    }
}

#pragma mark - UIGestureRecognizerDelegate

- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer
{
    return YES;
}

@end
