//
//  SimpleMissileStrategy.swift
//  AngryFIT
//
//  Created by Lukáš Hromadník on 13.12.16.
//  Copyright © 2016 Lukáš Hromadník. All rights reserved.
//

import UIKit
import SpriteKit

class SimpleMissileStrategy: MissileStrategy {
    
    func move(_ missile: Missile, angle: GameProperty.Value, force: GameProperty.Value) {
        let x = cos(angle.degreesToRadians)
        let y = sin(angle.degreesToRadians)
        let direction = CGPoint(x: x, y: y)
        let normalizedDirection = direction.normalized()
        let destination = CGPoint(x: normalizedDirection.x * 1000 + missile.position.x, y: normalizedDirection.y * 1000 + missile.position.y)
        
        let actionMove = SKAction.move(to: destination, duration: 2.0)
        let actionMoveDone = SKAction.removeFromParent()
        let action = SKAction.sequence([actionMove, actionMoveDone])
        
        Model.instance.run(action, on: missile)
    }
    
}
