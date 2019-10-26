//
//  AppDelegate.swift
//  AngryFIT
//
//  Created by Lukáš Hromadník on 29.11.16.
//  Copyright © 2016 Lukáš Hromadník. All rights reserved.
//

import UIKit

@UIApplicationMain
class AppDelegate: UIResponder, UIApplicationDelegate {

    var window: UIWindow?
    var memento: Memento?

    func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey: Any]?) -> Bool {
        window = UIWindow(frame: UIScreen.main.bounds)
        let rootController = StartViewController()
        window?.rootViewController = UINavigationController(rootViewController: rootController)
        window?.makeKeyAndVisible()
        
        return true
    }

}
