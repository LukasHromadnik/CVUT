//
//  BaseViewController.swift
//  AngryFIT
//
//  Created by Lukáš Hromadník on 12.12.16.
//  Copyright © 2016 Lukáš Hromadník. All rights reserved.
//

import UIKit

class BaseViewController: UIViewController {
    
    // MARK: - Controller lifecycle
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        navigationItem.backBarButtonItem = UIBarButtonItem(title: "", style: .plain, target: nil, action: nil)
    }
    
    // MARK: - Components setup
    
    override func loadView() {
        super.loadView()
        
        view.backgroundColor = .white
    }
    
    deinit {
        let className = String(describing: type(of: self))
        print("\(className) deinitialized")
    }
    
}
