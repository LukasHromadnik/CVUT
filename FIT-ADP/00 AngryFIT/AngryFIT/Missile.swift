//
//  Missile.swift
//  AngryFIT
//
//  Created by Lukáš Hromadník on 12.12.16.
//  Copyright © 2016 Lukáš Hromadník. All rights reserved.
//

import UIKit
import SpriteKit

class Missile: GameObject {
    
    let imageName = "missile"
    
    var position: CGPoint = .zero
    
    let strategy: MissileStrategy
    
    init(strategy: MissileStrategy) {
        self.strategy = strategy
    }
    
    func move(with angle: GameProperty.Value, force: GameProperty.Value) {
        strategy.move(self, angle: angle, force: force)
    }
    
    // MARK: - GameObjectVisitable
    
    func accept(_ visitor: GameObjectVisitor) {
        visitor.visit(self)
    }
    
}
