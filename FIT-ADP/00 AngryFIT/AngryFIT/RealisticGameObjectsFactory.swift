//
//  RealisticGameObjectsFactory.swift
//  AngryFIT
//
//  Created by Lukáš Hromadník on 13.12.16.
//  Copyright © 2016 Lukáš Hromadník. All rights reserved.
//

import Foundation

class RealisticGameObjectsFactory: GameObjectsFactory {
    
    func createMissile() -> Missile {
        return Missile(strategy: RealisticMissileStrategy())
    }
    
    func createEnemy() -> Enemy {
        return RealisticEnemy()
    }
    
}
