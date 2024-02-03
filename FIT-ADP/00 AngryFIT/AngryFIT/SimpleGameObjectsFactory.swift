//
//  SimpleGameObjectsFactory.swift
//  AngryFIT
//
//  Created by Lukáš Hromadník on 13.12.16.
//  Copyright © 2016 Lukáš Hromadník. All rights reserved.
//

import Foundation

class SimpleGameObjectsFactory: GameObjectsFactory {
    
    func createMissile() -> Missile {
        return Missile(strategy: SimpleMissileStrategy())
    }
    
    func createEnemy() -> Enemy {
        return Enemy()
    }
    
}
