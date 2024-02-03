//
//  Cannon.swift
//  AngryFIT
//
//  Created by Lukáš Hromadník on 12.12.16.
//  Copyright © 2016 Lukáš Hromadník. All rights reserved.
//

import UIKit

class Cannon: GameObject {
    
    let imageName = "cannon"
    
    var position: CGPoint = .zero
    
    let force = GameProperty(name: "Force", value: 10, min: 0, max: 20)
    let angle = GameProperty(name: "Angle", value: 0, min: -45, max: 45)
    
    var state: CannonState = DoubleCannonState()
    
    func fire() {
        state.shoot(from: self)
    }
    
    // MARK: - GameObjectVisitable
    
    func accept(_ visitor: GameObjectVisitor) {
        visitor.visit(self)
    }
    
}
