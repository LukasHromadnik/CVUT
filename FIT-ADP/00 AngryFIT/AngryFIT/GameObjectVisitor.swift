//
//  GameObjectVisitor.swift
//  AngryFIT
//
//  Created by Lukáš Hromadník on 13.12.16.
//  Copyright © 2016 Lukáš Hromadník. All rights reserved.
//

import Foundation

protocol GameObjectVisitor {
    func visit(_ cannon: Cannon)
    func visit(_ enemy: Enemy)
    func visit(_ missile: Missile)
}
