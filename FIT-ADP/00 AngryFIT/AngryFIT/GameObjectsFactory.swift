//
//  GameObjectsFactory.swift
//  AngryFIT
//
//  Created by Lukáš Hromadník on 13.12.16.
//  Copyright © 2016 Lukáš Hromadník. All rights reserved.
//

import Foundation

protocol GameObjectsFactory {
    func createMissile() -> Missile
    func createEnemy() -> Enemy
}
