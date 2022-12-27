//
//  RealisticMissileStrategy.swift
//  AngryFIT
//
//  Created by Lukáš Hromadník on 13.12.16.
//  Copyright © 2016 Lukáš Hromadník. All rights reserved.
//

import UIKit
import SpriteKit

class RealisticMissileStrategy: MissileStrategy {
    
    func move(_ missile: Missile, angle: GameProperty.Value, force: GameProperty.Value) {
        let x = cos(angle.degreesToRadians)
        let y = sin(angle.degreesToRadians)
        
        Model.instance.run(SKAction(), on: missile) { node in
            node.physicsBody?.affectedByGravity = true
            node.physicsBody?.applyImpulse(CGVector(dx: Double(force) * x, dy: Double(force) * y))
        }
    }
    
}
