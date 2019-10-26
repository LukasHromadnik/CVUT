//
//  PlaceDetailViewController.m
//  PragueSights
//
//  Created by Lukáš Hromadník on 05.12.14.
//  Copyright (c) 2014 Lukáš Hromadník. All rights reserved.
//

#import "PlaceDetailViewController.h"
#import "SVProgressHUD.h"

@interface PlaceDetailViewController ()
@property (nonatomic, strong) NSURLRequest *request;
@end

@implementation PlaceDetailViewController

#pragma mark - Initialization

- (instancetype)initWithURLString:(NSString *)URLString
{
    self = [super init];
    
    if (self) {
        NSURL *url = [[NSURL alloc] initWithString:URLString];
        self.request = [[NSURLRequest alloc] initWithURL:url];
    }
    
    return self;
}

#pragma mark - View lifecycle

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self.navigationController setToolbarHidden:NO animated:NO];

//    NSMutableArray *items = [NSMutableArray new];
    
    [SVProgressHUD show];
    [[UIApplication sharedApplication] setNetworkActivityIndicatorVisible:YES];

    UIWebView *webView = [[UIWebView alloc] initWithFrame:self.navigationController.view.frame];
    webView.delegate = self;
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^(void)
    {
        [webView loadRequest:self.request];
        [self.view addSubview:webView];
    });
}

#pragma mark - Webview Delegate

- (void)webViewDidFinishLoad:(UIWebView *)webView
{
    [SVProgressHUD dismiss];
    [[UIApplication sharedApplication] setNetworkActivityIndicatorVisible:NO];
}

- (void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error
{
    NSLog(@"Something went wrong!");
}

@end
