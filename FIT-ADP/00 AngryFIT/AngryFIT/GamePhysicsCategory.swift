//
//  GamePhysicsCategory.swift
//  AngryFIT
//
//  Created by Lukáš Hromadník on 13.12.16.
//  Copyright © 2016 Lukáš Hromadník. All rights reserved.
//

import Foundation

struct GamePhysicsCategory {
    static let None: UInt32 = 0
    static let All: UInt32 = UInt32.max
    static let Enemy: UInt32 = 0b1
    static let Missile: UInt32 = 0b10
    static let Boundary: UInt32 = 0b100
}
