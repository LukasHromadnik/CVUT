//
//  RealisticEnemy.swift
//  AngryFIT
//
//  Created by Lukáš Hromadník on 13.12.16.
//  Copyright © 2016 Lukáš Hromadník. All rights reserved.
//

import Foundation

class RealisticEnemy: Enemy {
    
    weak var timer: Timer?
    
    override init() {
        super.init()
        
        let timerTick = (arc4random() % 5) + 3
        
        self.timer = Timer.scheduledTimer(withTimeInterval: TimeInterval(timerTick), repeats: true) { _ in
            Model.instance.changePosition(for: self)
        }
    }
    
    deinit {
        timer?.invalidate()
    }
    
}
